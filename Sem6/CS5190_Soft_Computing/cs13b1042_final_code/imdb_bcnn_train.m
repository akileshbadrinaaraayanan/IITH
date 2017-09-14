function imdb_bcnn_train(imdb, opts, varargin)
% Train a bilinear CNN model on a dataset supplied by imdb

% Copyright (C) 2015 Tsung-Yu Lin, Aruni RoyChowdhury, Subhransu Maji.
% All rights reserved.
%
% This file is part of BCNN and is made available 
% under the terms of the BSD license (see the COPYING file).
%
% This file modified from IMDB_CNN_TRAIN of MatConvNet

opts.lite = false ;
opts.numFetchThreads = 0 ;
opts.train.batchSize = opts.batchSize ;
opts.train.numEpochs = opts.numEpochs ;
opts.train.continue = true ;
opts.train.useGpu = false ;
opts.train.prefetch = false ;
opts.train.learningRate = [0.001*ones(1, 10) 0.001*ones(1, 10) 0.001*ones(1,10)] ;
opts.train.expDir = opts.expDir ;
opts.train.dataAugmentation = opts.dataAugmentation;
opts = vl_argparse(opts, varargin) ;


opts.inittrain.weightDecay = 0 ;
opts.inittrain.batchSize = 256 ;
opts.inittrain.numEpochs = 300 ;
opts.inittrain.continue = true ;
opts.inittrain.useGpu = false ;
opts.inittrain.prefetch = false ;
opts.inittrain.learningRate = 0.001 ;
opts.inittrain.expDir = fullfile(opts.expDir, 'init') ;
opts.inittrain.nonftbcnnDir = fullfile(opts.expDir, 'nonftbcnn');
 


if(opts.useGpu)
    opts.train.useGpu = opts.useGpu;
    opts.inittrain.useGpu = opts.useGpu;
end


% initialize the network
net = initializeNetwork(imdb, opts) ;
shareWeights = ~isfield(net, 'netc');
 
if(~exist(fullfile(opts.expDir, 'fine-tuned-model'), 'dir'))
    mkdir(fullfile(opts.expDir, 'fine-tuned-model'))
end
   
if(shareWeights)
    % fine-tuning the symmetric B-CNN
    fn = getBatchWrapper(net.normalization, opts.numFetchThreads) ;
    [net,info] = bcnn_train_sw(net, imdb, fn, opts.train, 'conserveMemory', true, 'scale', opts.bcnnScale, 'momentum', opts.momentum) ;
    
    net = vl_simplenn_move(net, 'cpu');
    saveNetwork(fullfile(opts.expDir, 'fine-tuned-model', 'final-model.mat'), net);

else
    % fine-tuning the asymmetric B-CNN
    norm_struct(1) = net.neta.normalization;
    norm_struct(2) = net.netb.normalization;
    fn = getBatchWrapper(norm_struct, opts.numFetchThreads) ;    
%     fn = getBatchWrapper(net.neta.normalization, opts.numFetchThreads) ;    
    [net,info] = bcnn_train(net, fn, imdb, opts.train, 'conserveMemory', true, 'scale', opts.bcnnScale, 'momentum', opts.momentum) ;
    
    net.neta = vl_simplenn_move(net.neta, 'cpu');
    net.netb = vl_simplenn_move(net.netb, 'cpu');
    saveNetwork(fullfile(opts.expDir, 'fine-tuned-model', 'final-model-neta.mat'), net.neta);
    saveNetwork(fullfile(opts.expDir, 'fine-tuned-model', 'final-model-netb.mat'), net.netb);
end

% -------------------------------------------------------------------------
function saveNetwork(fileName, net)
% -------------------------------------------------------------------------
layers = net.layers;
% 
% % Replace the last layer with softmax
% layers{end}.type = 'softmax';
% layers{end}.name = 'prob';

% Remove fields corresponding to training parameters
ignoreFields = {'filtersMomentum', ...
                'biasesMomentum',...
                'filtersLearningRate',...
                'biasesLearningRate',...
                'filtersWeightDecay',...
                'biasesWeightDecay',...
                'class'};
for i = 1:length(layers),
    layers{i} = rmfield(layers{i}, ignoreFields(isfield(layers{i}, ignoreFields)));
end
classes = net.classes;
normalization = net.normalization;
save(fileName, 'layers', 'classes', 'normalization', '-v7.3');


% -------------------------------------------------------------------------
function fn = getBatchWrapper(opts, numThreads)
% -------------------------------------------------------------------------
fn = @(imdb,batch,augmentation, scale) getBatch(imdb, batch, augmentation, scale, opts, numThreads) ;

% -------------------------------------------------------------------------
function [im,labels] = getBatch(imdb, batch, augmentation, scale, opts, numThreads)
% -------------------------------------------------------------------------
images = strcat([imdb.imageDir '/'], imdb.images.name(batch)) ;
im = imdb_get_batch_bcnn(images, opts, ...
                            'numThreads', numThreads, ...
                            'prefetch', nargout == 0, 'augmentation', augmentation, 'scale', scale);
labels = imdb.images.label(batch) ;
numAugments = numel(im{1})/numel(batch);
labels = reshape(repmat(labels, numAugments, 1), 1, numel(im{1}));



function [im,labels] = getBatch_bcnn_fromdisk(imdb, batch, opts, numThreads)
% -------------------------------------------------------------------------

im = cell(1, numel(batch));
for i=1:numel(batch)
    load(fullfile(imdb.imageDir, imdb.images.name{batch(i)}));
    im{i} = code;
end
im = cat(2, im{:});
im = reshape(im, 1, 1, size(im,1), size(im, 2));
labels = imdb.images.label(batch) ;



function net = initializeNetwork(imdb, opts)
% -------------------------------------------------------------------------

% get encoder setting
encoderOpts.type = 'bcnn';
encoderOpts.modela = [];
encoderOpts.layera = 14;
encoderOpts.modelb = [];
encoderOpts.layerb = 14;
encoderOpts.shareWeight = false;

encoderOpts = vl_argparse(encoderOpts, opts.encoders{1}.opts);

scal = 1 ;
init_bias = 0.1;
numClass = length(imdb.classes.name);
    
%% the case using two networks
if ~encoderOpts.shareWeight
    
    assert(~isempty(encoderOpts.modela) && ~isempty(encoderOpts.modelb), 'at least one of the network is not specified')
    
    % load the pre-trained models
    encoder.neta = load(encoderOpts.modela);
    encoder.neta.layers = encoder.neta.layers(1:encoderOpts.layera);
    encoder.netb = load(encoderOpts.modelb);
    encoder.netb.layers = encoder.netb.layers(1:encoderOpts.layerb);
    encoder.regionBorder = 0.05;
    encoder.type = 'bcnn';
    encoder.normalization = 'sqrt_L2';
    % move models to GPU
    if opts.useGpu
        encoder.neta = vl_simplenn_move(encoder.neta, 'gpu') ;
        encoder.netb = vl_simplenn_move(encoder.netb, 'gpu') ;
        encoder.neta.useGpu = true ;
        encoder.netb.useGpu = true ;
    else
        encoder.neta = vl_simplenn_move(encoder.neta, 'cpu') ;
        encoder.netb = vl_simplenn_move(encoder.netb, 'cpu') ;
        encoder.neta.useGpu = false ;
        encoder.netb.useGpu = false ;
    end
    % set the bcnn_net structure
    net.neta = encoder.neta;
    net.netb = encoder.netb;
    net.neta.normalization.keepAspect = opts.keepAspect;
    net.netb.normalization.keepAspect = opts.keepAspect;
    
    netc.layers = {};
    
    % get the bcnn feature dimension
    for i=numel(net.neta.layers):-1:1
        if strcmp(net.neta.layers{i}.type, 'conv')
            idx = i;
            break;
        end
    end
    ch1 = numel(net.neta.layers{idx}.biases);
    
    for i=numel(net.netb.layers):-1:1
        if strcmp(net.netb.layers{i}.type, 'conv')
            idx = i;
            break;
        end
    end
    ch2 = numel(net.netb.layers{idx}.biases);    
    dim = ch1*ch2;
    
    % randomly initialize the softmax layer
    initialW = 0.001/scal *randn(1,1,dim, numClass,'single');
    initialBias = init_bias.*ones(1, numClass, 'single');
    
        
    netc.layers = {};
    net.netc.layers = {};
    net.netc.layers{end+1} = struct('type', 'sqrt');
    net.netc.layers{end+1} = struct('type', 'l2norm');    
        
    netc.layers{end+1} = struct('type', 'conv', ...
        'filters', initialW, ...
        'biases', initialBias, ...
        'stride', 1, ...
        'pad', 0, ...
        'filtersLearningRate', 1000, ...
        'biasesLearningRate', 1000, ...
        'filtersWeightDecay', 0, ...
        'biasesWeightDecay', 0) ;


    netc.layers{end+1} = struct('type', 'softmaxloss') ;
    
    % logistic regression for the softmax layers
    if(opts.bcnnLRinit)
        if exist(fullfile(opts.expDir, 'initial_fc.mat'))
            load(fullfile(opts.expDir, 'initial_fc.mat'), 'netc') ;
            
        else     
            
            trainIdx = find(ismember(imdb.images.set, [1 2]));
            testIdx = find(ismember(imdb.images.set, 3));
            
            % compute and cache the bilinear cnn features
            if ~exist(opts.inittrain.nonftbcnnDir)
                mkdir(opts.inittrain.nonftbcnnDir)
                batchSize = 10000;
                for b=1:ceil(numel(trainIdx)/batchSize)
                    idxEnd = min(numel(trainIdx), b*batchSize);
                    idx = trainIdx((b-1)*batchSize+1:idxEnd);
                    codeCell = encoder_extract_for_images(encoder, imdb, imdb.images.id(idx), 'concatenateCode', false, 'scale', opts.bcnnScale);
                    for i=1:numel(codeCell)
                        code = codeCell{i};
                        savefast(fullfile(opts.inittrain.nonftbcnnDir, ['bcnn_nonft_', num2str(idx(i), '%05d')]), 'code');
                    end
                end
            end
            
            
            bcnndb = imdb;
            tempStr = sprintf('%05d\t', trainIdx);
            tempStr = textscan(tempStr, '%s', 'delimiter', '\t');
            bcnndb.images.name = strcat('bcnn_nonft_', tempStr{1}');
            bcnndb.images.id = bcnndb.images.id(trainIdx);
            bcnndb.images.label = bcnndb.images.label(trainIdx);
            bcnndb.images.set = bcnndb.images.set(trainIdx);
            bcnndb.imageDir = opts.inittrain.nonftbcnnDir;
            
            %train logistic regression
            [netc, info] = cnn_train(netc, bcnndb, @getBatch_bcnn_fromdisk, opts.inittrain, ...
                'batchSize', opts.inittrain.batchSize, 'weightDecay', opts.inittrain.weightDecay, ...
                'conserveMemory', true, 'expDir', opts.inittrain.expDir);
            
            save(fullfile(opts.expDir, 'initial_fc.mat'), 'netc', '-v7.3') ;
        end
    end
    
    for i=1:numel(netc.layers)
        net.netc.layers{end+1} = netc.layers{i};
    end

   
else
    %% the case with shared weights
    

    assert(strcmp(encoderOpts.modela, encoderOpts.modelb), 'neta and netb are required to be the same');    
    assert(~isempty(encoderOpts.modela), 'network is not specified');
        
    net = load(encoderOpts.modela); % Load model if specified
    net.normalization.keepAspect = opts.keepAspect;

    % truncate the network at the last layer whose output is combined with another
    % layer's output to form the bcnn feature
    maxLayer = max(encoderOpts.layera, encoderOpts.layerb);
    net.layers = net.layers(1:maxLayer);
    
    
    % get the bcnn feature dimension
    for i=encoderOpts.layera:-1:1
        if strcmp(net.layers{i}.type, 'conv')
            idx = i;
            break;
        end
    end
    mapSize1 = numel(net.layers{idx}.biases);
    
    
    for i=encoderOpts.layerb:-1:1
        if strcmp(net.layers{i}.type, 'conv')
            idx = i;
            break;
        end
    end
    mapSize2 = numel(net.layers{idx}.biases);
    
    
    % stack bilinearpool, normalization, and softmax layers
    if(encoderOpts.layera==encoderOpts.layerb)
        net.layers{end+1} = struct('type', 'bilinearpool');
    else
        net.layers{end+1} = struct('type', 'bilinearclpool', 'layer1', encoderOpts.layera, 'layer2', encoderOpts.layerb);
    end
    net.layers{end+1} = struct('type', 'sqrt');
    net.layers{end+1} = struct('type', 'l2norm');
    
    
    initialW = 0.001/scal * randn(1,1,mapSize1*mapSize2,numClass,'single');
    initialBias = init_bias.*ones(1, numClass, 'single');
    
    netc.layers = {};
    netc.layers{end+1} = struct('type', 'conv', ...
                'filters', initialW, ...
                'biases', initialBias, ...
                'stride', 1, ...
                'pad', 0, ...
                'filtersLearningRate', 1000, ...
                'biasesLearningRate', 1000, ...
                'filtersWeightDecay', 0, ...
                'biasesWeightDecay', 0) ;
            
            
    netc.layers{end+1} = struct('type', 'softmaxloss') ;
    
    % logistic regression for the softmax layers 
    if(opts.bcnnLRinit)
        netInit = net;
       
        
        if opts.train.useGpu
            netInit = vl_simplenn_move(netInit, 'gpu') ;
        end
        train = find(imdb.images.set==1|imdb.images.set==2);
        batchSize = 64;
        getBatchFn = getBatchWrapper(netInit.normalization, opts.numFetchThreads);
        
        
        if ~exist(opts.inittrain.nonftbcnnDir, 'dir')
        
            mkdir(opts.inittrain.nonftbcnnDir)
            
            % compute and cache the bilinear cnn features
            for t=1:batchSize:numel(train)
                fprintf('Initialization: extracting bcnn feature of batch %d/%d\n', ceil(t/batchSize), ceil(numel(train)/batchSize));
                batch = train(t:min(numel(train), t+batchSize-1));
                [im, labels] = getBatchFn(imdb, batch, opts.dataAugmentation{1}, opts.bcnnScale) ;
                if opts.train.prefetch
                    nextBatch = train(t+batchSize:min(t+2*batchSize-1, numel(train))) ;
                    getBatcFn(imdb, nextBatch, opts.dataAugmentation{1}, opts.bcnnScale) ;
                end
                im = im{1};
                im = cat(4, im{:});
                if opts.train.useGpu
                    im = gpuArray(im) ;
                end
                net.layers{end}.class = labels ;
                
                res = [] ;
                res = vl_bilinearnn(netInit, im, [], res, ...
                    'disableDropout', true, ...
                    'conserveMemory', true, ...
                    'sync', true) ;
                
                codeb = squeeze(gather(res(end).x));
                
                for i=1:numel(batch)
                    code = codeb(:,i);
                    savefast(fullfile(opts.inittrain.nonftbcnnDir, ['bcnn_nonft_', num2str(batch(i), '%05d')]), 'code');
                end
            end
        end
        
        clear code res netInit
        
        if exist(fullfile(opts.expDir, 'initial_fc.mat'), 'file')
            load(fullfile(opts.expDir, 'initial_fc.mat'), 'netc') ;
        else
            
            
            bcnndb = imdb;
            tempStr = sprintf('%05d\t', train);
            tempStr = textscan(tempStr, '%s', 'delimiter', '\t');
            bcnndb.images.name = strcat('bcnn_nonft_', tempStr{1}');
            bcnndb.images.id = bcnndb.images.id(train);
            bcnndb.images.label = bcnndb.images.label(train);
            bcnndb.images.set = bcnndb.images.set(train);
            bcnndb.imageDir = opts.inittrain.nonftbcnnDir;
            
            %train logistic regression
            [netc, info] = cnn_train(netc, bcnndb, @getBatch_bcnn_fromdisk, opts.inittrain, ...
                'batchSize', opts.inittrain.batchSize, 'weightDecay', opts.inittrain.weightDecay, ...
                'conserveMemory', true, 'expDir', opts.inittrain.expDir);
            
            save(fullfile(opts.expDir, 'initial_fc.mat'), 'netc', '-v7.3') ;
        end
        
    end
    
    for i=1:numel(netc.layers)
        net.layers{end+1} = netc.layers{i};
    end
	
      
    % Rename classes
    net.classes.name = imdb.classes.name;
    net.classes.description = imdb.classes.name;
    
end


function code = encoder_extract_for_images(encoder, imdb, imageIds, varargin)
% -------------------------------------------------------------------------
opts.batchSize = 128 ;
opts.maxNumLocalDescriptorsReturned = 500 ;
opts.concatenateCode = true;
opts.scale = 2;
opts = vl_argparse(opts, varargin) ;

[~,imageSel] = ismember(imageIds, imdb.images.id) ;
imageIds = unique(imdb.images.id(imageSel)) ;
n = numel(imageIds) ;

% prepare batches
n = ceil(numel(imageIds)/opts.batchSize) ;

batches = mat2cell(1:numel(imageIds), 1, [opts.batchSize * ones(1, n-1), numel(imageIds) - opts.batchSize*(n-1)]) ;
batchResults = cell(1, numel(batches)) ;

% just use as many workers as are already available
numWorkers = matlabpool('size') ;

for b = numel(batches):-1:1
  batchResults{b} = get_batch_results(imdb, imageIds, batches{b}, ...
                        encoder, opts.maxNumLocalDescriptorsReturned, opts.scale) ;
end


code = cell(size(imageIds)) ;
for b = 1:numel(batches)
  m = numel(batches{b});
  for j = 1:m
      k = batches{b}(j) ;
      code{k} = batchResults{b}.code{j};
  end
end

if opts.concatenateCode
    clear batchResults
   code = cat(2, code{:}) ;
end


function result = get_batch_results(imdb, imageIds, batch, encoder, maxn, scale)
% -------------------------------------------------------------------------
m = numel(batch) ;
im = cell(1, m) ;
task = getCurrentTask() ;
if ~isempty(task), tid = task.ID ; else tid = 1 ; end
for i = 1:m
  fprintf('Task: %03d: encoder: extract features: image %d of %d\n', tid, batch(i), numel(imageIds)) ;
  im{i} = imread(fullfile(imdb.imageDir, imdb.images.name{imdb.images.id == imageIds(batch(i))}));
  if size(im{i}, 3) == 1, im{i} = repmat(im{i}, [1 1 3]);, end; %grayscale image
end

if ~isfield(encoder, 'numSpatialSubdivisions')
    encoder.numSpatialSubdivisions = 1 ;
end
code_ = get_bcnn_features(encoder.neta, encoder.netb,...
    im, ...
    'regionBorder', encoder.regionBorder, ...
    'normalization', encoder.normalization, ...
    'scales', scale);
    
result.code = code_ ;
