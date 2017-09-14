function [bcnn_net, info] = bcnn_train(bcnn_net, getBatch, imdb, varargin)

% BNN_TRAIN   training an asymmetric BCNN 
%    BCNN_TRAIN() is an example learner implementing stochastic gradient
%    descent with momentum to train an asymmetric BCNN for image classification.
%    It can be used with different datasets by providing a suitable
%    getBatch function.

% INPUT
% bcnn_net: a bcnn networks structure
% getBatch: function to read a batch of images
% imdb: imdb structure of a dataset

% OUTPUT
% bcnn_net: an output of asymmetric bcnn network after fine-tuning
% info: log of training and validation

% An asymmetric BCNN network BCNN_NET consist of three parts:
% neta: Network A to extract features
% netb: Network B to extract features
% netc: consists of normalization layers and softmax layer to obtain the
%       classification error and loss based on bcnn features combined from neta
%       and netb

% Copyright (C) 2015 Tsung-Yu Lin, Aruni RoyChowdhury, Subhransu Maji.
% All rights reserved.
%
% This file is part of the BCNN and is made available under
% the terms of the BSD license (see the COPYING file).

% This function is modified from CNN_TRAIN of MatConvNet

% basic setting
opts.train = [] ;
opts.val = [] ;
opts.numEpochs = 300 ;
opts.batchSize = 256 ;
opts.useGpu = false ;
opts.learningRate = 0.0001 ;
opts.continue = false ;
opts.expDir = fullfile('data','exp') ;
opts.conserveMemory = false ;
opts.sync = true ;
opts.prefetch = false ;
opts.weightDecay = 0.0005 ;
opts.momentum = 0.3 ;
opts.errorType = 'multiclass' ;
opts.plotDiagnostics = false ;
opts.layera = 14;
opts.layerb = 14;
opts.regionBorder = 0.05;
opts.dataAugmentation = {'none', 'none', 'none'};
opts.scale = 2;

opts = vl_argparse(opts, varargin) ;

if ~exist(opts.expDir, 'dir'), mkdir(opts.expDir) ; end
if isempty(opts.train), opts.train = find(imdb.images.set==1) ; end
if isempty(opts.val), opts.val = find(imdb.images.set==2) ; end
if isnan(opts.train), opts.train = [] ; end

% -------------------------------------------------------------------------
%                                                    Network initialization
% -------------------------------------------------------------------------
% set hyperparameters of network A
neta = bcnn_net.neta;

for i=1:numel(neta.layers)
  if ~strcmp(neta.layers{i}.type,'conv'), continue; end
  neta.layers{i}.filtersMomentum = zeros(size(neta.layers{i}.filters), ...
    class(neta.layers{i}.filters)) ;
  neta.layers{i}.biasesMomentum = zeros(size(neta.layers{i}.biases), ...
    class(neta.layers{i}.biases)) ; 
  if ~isfield(neta.layers{i}, 'filtersLearningRate')
    neta.layers{i}.filtersLearningRate = 1 ;
  end
  if ~isfield(neta.layers{i}, 'biasesLearningRate')
    neta.layers{i}.biasesLearningRate = 1 ;
  end
  if ~isfield(neta.layers{i}, 'filtersWeightDecay')
    neta.layers{i}.filtersWeightDecay = 1 ;
  end
  if ~isfield(neta.layers{i}, 'biasesWeightDecay')
    neta.layers{i}.biasesWeightDecay = 1 ;
  end
end
% set hyperparameters of network A
netb = bcnn_net.netb;
for i=1:numel(netb.layers)
  if ~strcmp(netb.layers{i}.type,'conv'), continue; end
  netb.layers{i}.filtersMomentum = zeros(size(netb.layers{i}.filters), ...
    class(netb.layers{i}.filters)) ;
  netb.layers{i}.biasesMomentum = zeros(size(netb.layers{i}.biases), ...
    class(netb.layers{i}.biases)) ; 
  if ~isfield(netb.layers{i}, 'filtersLearningRate')
    netb.layers{i}.filtersLearningRate = 1 ;
  end
  if ~isfield(netb.layers{i}, 'biasesLearningRate')
    netb.layers{i}.biasesLearningRate = 1 ;
  end
  if ~isfield(netb.layers{i}, 'filtersWeightDecay')
    netb.layers{i}.filtersWeightDecay = 1 ;
  end
  if ~isfield(netb.layers{i}, 'biasesWeightDecay')
    netb.layers{i}.biasesWeightDecay = 1 ;
  end
end
% set hyperparameters of network A
netc = bcnn_net.netc;
for i=1:numel(netc.layers)
  if ~strcmp(netc.layers{i}.type,'conv'), continue; end
  netc.layers{i}.filtersMomentum = zeros(size(netc.layers{i}.filters), ...
    class(netc.layers{i}.filters)) ;
  netc.layers{i}.biasesMomentum = zeros(size(netc.layers{i}.biases), ...
    class(netc.layers{i}.biases)) ; 
  if ~isfield(netc.layers{i}, 'filtersLearningRate')
    netc.layers{i}.filtersLearningRate = 1 ;
  end
  if ~isfield(netc.layers{i}, 'biasesLearningRate')
    netc.layers{i}.biasesLearningRate = 1 ;
  end
  if ~isfield(netc.layers{i}, 'filtersWeightDecay')
    netc.layers{i}.filtersWeightDecay = 1 ;
  end
  if ~isfield(netc.layers{i}, 'biasesWeightDecay')
    netc.layers{i}.biasesWeightDecay = 1 ;
  end
end


% -------------------------------------------------------------------------
%                                                Move network to GPU or CPU
% -------------------------------------------------------------------------

if opts.useGpu
  neta.useGpu = true;
  neta = vl_simplenn_move(neta, 'gpu') ;
  for i=1:numel(neta.layers)
    if ~strcmp(neta.layers{i}.type,'conv'), continue; end
    neta.layers{i}.filtersMomentum = gpuArray(neta.layers{i}.filtersMomentum) ;
    neta.layers{i}.biasesMomentum = gpuArray(neta.layers{i}.biasesMomentum) ;
  end
  
  netb.useGpu = true;
  netb = vl_simplenn_move(netb, 'gpu') ;
  for i=1:numel(netb.layers)
    if ~strcmp(netb.layers{i}.type,'conv'), continue; end
    netb.layers{i}.filtersMomentum = gpuArray(netb.layers{i}.filtersMomentum) ;
    netb.layers{i}.biasesMomentum = gpuArray(netb.layers{i}.biasesMomentum) ;
  end
  
  netc.useGpu = true;
  netc = vl_simplenn_move(netc, 'gpu') ;
  for i=1:numel(netc.layers)
    if ~strcmp(netc.layers{i}.type,'conv'), continue; end
    netc.layers{i}.filtersMomentum = gpuArray(netc.layers{i}.filtersMomentum) ;
    netc.layers{i}.biasesMomentum = gpuArray(netc.layers{i}.biasesMomentum) ;
  end
else
  neta.useGpu = false;
  neta = vl_simplenn_move(neta, 'cpu') ;
  for i=1:numel(neta.layers)
    if ~strcmp(neta.layers{i}.type,'conv'), continue; end
    neta.layers{i}.filtersMomentum = gather(neta.layers{i}.filtersMomentum) ;
    neta.layers{i}.biasesMomentum = gather(neta.layers{i}.biasesMomentum) ;
  end
  
  netb.useGpu = false;
  netb = vl_simplenn_move(netb, 'cpu') ;
  for i=1:numel(netb.layers)
    if ~strcmp(netb.layers{i}.type,'conv'), continue; end
    netb.layers{i}.filtersMomentum = gather(netb.layers{i}.filtersMomentum) ;
    netb.layers{i}.biasesMomentum = gather(netb.layers{i}.biasesMomentum) ;
  end
  
  netc.useGpu = false;
  netc = vl_simplenn_move(netc, 'cpu') ;
  for i=1:numel(netc.layers)
    if ~strcmp(netc.layers{i}.type,'conv'), continue; end
    netc.layers{i}.filtersMomentum = gather(netc.layers{i}.filtersMomentum) ;
    netc.layers{i}.biasesMomentum = gather(netc.layers{i}.biasesMomentum) ;
  end
    
end

% -------------------------------------------------------------------------
%                                                        Train and validate
% -------------------------------------------------------------------------

if opts.useGpu
  one = gpuArray(single(1)) ;
else
  one = single(1) ;
end

info.train.objective = [] ;
info.train.error = [] ;
info.train.topFiveError = [] ;
info.train.speed = [] ;
info.val.objective = [] ;
info.val.error = [] ;
info.val.topFiveError = [] ;
info.val.speed = [] ;

lr = 0 ;
resa = [] ;
resb = [] ;
resc = [] ;
for epoch=1:opts.numEpochs
  prevLr = lr ;
  lr = opts.learningRate(min(epoch, numel(opts.learningRate))) ;

  % fast-forward to where we stopped
  modelPath = @(ep) fullfile(opts.expDir, sprintf('net-epoch-%d.mat', ep));
  modelFigPath = fullfile(opts.expDir, 'net-train.pdf') ;
  if opts.continue
    if exist(modelPath(epoch),'file'), continue ; end
    if epoch > 1
      fprintf('resuming by loading epoch %d\n', epoch-1) ;
      load(modelPath(epoch-1), 'neta', 'netb', 'netc', 'info') ;
    end
  end

  train = opts.train(randperm(numel(opts.train))) ;
  val = opts.val ;

  info.train.objective(end+1) = 0 ;
  info.train.error(end+1) = 0 ;
  info.train.topFiveError(end+1) = 0 ;
  info.train.speed(end+1) = 0 ;
  info.val.objective(end+1) = 0 ;
  info.val.error(end+1) = 0 ;
  info.val.topFiveError(end+1) = 0 ;
  info.val.speed(end+1) = 0 ;

  % reset momentum if needed
  if prevLr ~= lr
    fprintf('learning rate changed (%f --> %f): resetting momentum\n', prevLr, lr) ;
    for l=1:numel(neta.layers)
      if ~strcmp(neta.layers{l}.type, 'conv'), continue ; end
      neta.layers{l}.filtersMomentum = 0 * neta.layers{l}.filtersMomentum ;
      neta.layers{l}.biasesMomentum = 0 * neta.layers{l}.biasesMomentum ;
    end
    
    for l=1:numel(netb.layers)
      if ~strcmp(netb.layers{l}.type, 'conv'), continue ; end
      netb.layers{l}.filtersMomentum = 0 * netb.layers{l}.filtersMomentum ;
      netb.layers{l}.biasesMomentum = 0 * netb.layers{l}.biasesMomentum ;
    end
    
    for l=1:numel(netc.layers)
      if ~strcmp(netc.layers{l}.type, 'conv'), continue ; end
      netc.layers{l}.filtersMomentum = 0 * netc.layers{l}.filtersMomentum ;
      netc.layers{l}.biasesMomentum = 0 * netc.layers{l}.biasesMomentum ;
    end
  end

  for t=1:opts.batchSize:numel(train)
    % get next image batch and labels
    batch = train(t:min(t+opts.batchSize-1, numel(train))) ;
    batch_time = tic ;
    fprintf('training: epoch %02d: processing batch %3d of %3d ...', epoch, ...
            fix((t-1)/opts.batchSize)+1, ceil(numel(train)/opts.batchSize)) ;
     
    [im, labels] = getBatch(imdb, batch, opts.dataAugmentation{1}, opts.scale) ;
    if opts.prefetch
      nextBatch = train(t+opts.batchSize:min(t+2*opts.batchSize-1, numel(train)), opts.scale) ;
      getBatch(imdb, nextBatch) ;
    end
    
    if(exist('dA', 'var'))
        clear dA dB dEdpsi psi_n ima imb resa resb
        wait(gpuDevice);
        resc = [];
    end
    
    % do forward passes on neta and netb to get bilinear CNN features
    [psi, resa, resb] = bcnn_asym_forward(neta, netb, im, ...
        'regionBorder', opts.regionBorder, ...
        'normalization', 'none', 'networkconservmemory', false);
    
    if opts.useGpu
        A = gather(resa(end).x);
        B = gather(resb(end).x);
    else
        A = resa(end).x;
        B = resb(end).x;
    end
    
    psi = cat(2, psi{:});
    
  
    psi = reshape(psi, [1,1,size(psi,1),size(psi,2)]);
    
    if opts.useGpu
      psi = gpuArray(psi) ;
    end
    
    
    netc.layers{end}.class = labels ;
    
    % do forward and backward passes on netc after bilinear pool
    resc = vl_bilinearnn(netc, psi, 1, resc, ...
      'conserveMemory', false, ...
      'sync', opts.sync) ;
  
    % compute the derivative with respected to the outputs of network A and network B
    dEdpsi = reshape(squeeze(resc(1).dzdx), size(A,3), size(B,3), size(A,4));
    [dA, dB] = arrayfun(@(x) compute_deriv_resp_AB(dEdpsi(:,:,x), A(:,:,:,x), B(:,:,:,x), opts.useGpu), 1:size(dEdpsi, 3), 'UniformOutput', false);
    dA = cat(4, dA{:});
    dB = cat(4, dB{:});



    % backprop through network A
    resa = vl_bilinearnn(neta, [], dA, resa, ...
      'conserveMemory', opts.conserveMemory, ...
      'sync', opts.sync, 'doforward', false) ;
  
    % backprop through network B
    resb = vl_bilinearnn(netb, [], dB, resb, ...
      'conserveMemory', opts.conserveMemory, ...
      'sync', opts.sync, 'doforward', false) ;
  
    % gradient step on network A
    for l=1:numel(neta.layers)
      if ~strcmp(neta.layers{l}.type, 'conv'), continue ; end

      neta.layers{l}.filtersMomentum = ...
        opts.momentum * neta.layers{l}.filtersMomentum ...
          - (lr * neta.layers{l}.filtersLearningRate) * ...
          (opts.weightDecay * neta.layers{l}.filtersWeightDecay) * neta.layers{l}.filters ...
          - (lr * neta.layers{l}.filtersLearningRate) / numel(batch) * resa(l).dzdw{1} ;

      neta.layers{l}.biasesMomentum = ...
        opts.momentum * neta.layers{l}.biasesMomentum ...
          - (lr * neta.layers{l}.biasesLearningRate) * ....
          (opts.weightDecay * neta.layers{l}.biasesWeightDecay) * neta.layers{l}.biases ...
          - (lr * neta.layers{l}.biasesLearningRate) / numel(batch) * resa(l).dzdw{2} ;

      neta.layers{l}.filters = neta.layers{l}.filters + neta.layers{l}.filtersMomentum ;
      neta.layers{l}.biases = neta.layers{l}.biases + neta.layers{l}.biasesMomentum ;
    end
    
    
    % gradient step on network B
    for l=1:numel(netb.layers)
      if ~strcmp(netb.layers{l}.type, 'conv'), continue ; end

      netb.layers{l}.filtersMomentum = ...
        opts.momentum * netb.layers{l}.filtersMomentum ...
          - (lr * netb.layers{l}.filtersLearningRate) * ...
          (opts.weightDecay * netb.layers{l}.filtersWeightDecay) * netb.layers{l}.filters ...
          - (lr * netb.layers{l}.filtersLearningRate) / numel(batch) * resb(l).dzdw{1} ;

      netb.layers{l}.biasesMomentum = ...
        opts.momentum * netb.layers{l}.biasesMomentum ...
          - (lr * netb.layers{l}.biasesLearningRate) * ....
          (opts.weightDecay * netb.layers{l}.biasesWeightDecay) * netb.layers{l}.biases ...
          - (lr * netb.layers{l}.biasesLearningRate) / numel(batch) * resb(l).dzdw{2} ;

      netb.layers{l}.filters = netb.layers{l}.filters + netb.layers{l}.filtersMomentum ;
      netb.layers{l}.biases = netb.layers{l}.biases + netb.layers{l}.biasesMomentum ;
    end
   
    % gradient step on network C
    for l=1:numel(netc.layers)
      if ~strcmp(netc.layers{l}.type, 'conv'), continue ; end

      netc.layers{l}.filtersMomentum = ...
        opts.momentum * netc.layers{l}.filtersMomentum ...
          - (lr * netc.layers{l}.filtersLearningRate) * ...
          (opts.weightDecay * netc.layers{l}.filtersWeightDecay) * netc.layers{l}.filters ...
          - (lr * netc.layers{l}.filtersLearningRate) / numel(batch) * resc(l).dzdw{1} ;

      netc.layers{l}.biasesMomentum = ...
        opts.momentum * netc.layers{l}.biasesMomentum ...
          - (lr * netc.layers{l}.biasesLearningRate) * ....
          (opts.weightDecay * netc.layers{l}.biasesWeightDecay) * netc.layers{l}.biases ...
          - (lr * netc.layers{l}.biasesLearningRate) / numel(batch) * resc(l).dzdw{2} ;

      netc.layers{l}.filters = netc.layers{l}.filters + netc.layers{l}.filtersMomentum ;
      netc.layers{l}.biases = netc.layers{l}.biases + netc.layers{l}.biasesMomentum ;
    end

    % print information
    batch_time = toc(batch_time) ;
    speed = numel(batch)/batch_time ;
    info.train = updateError(opts, info.train, netc, resc, batch_time) ;

    fprintf(' %.2f s (%.1f images/s)', batch_time, speed) ;
    n = t + numel(batch) - 1 ;
    fprintf(' err %.1f err5 %.1f', ...
      info.train.error(end)/n*100, info.train.topFiveError(end)/n*100) ;
    fprintf('\n') ;

    % debug info
    if opts.plotDiagnostics
      figure(2) ; vl_simplenn_diagnose(net,res) ; drawnow ;
    end
  end % next batch

  % evaluation on validation set
  for t=1:opts.batchSize:numel(val)
      
      
    batch_time = tic ;
    batch = val(t:min(t+opts.batchSize-1, numel(val))) ;
    fprintf('validation: epoch %02d: processing batch %3d of %3d ...', epoch, ...
            fix((t-1)/opts.batchSize)+1, ceil(numel(val)/opts.batchSize)) ;      
      
    [im, labels] = getBatch(imdb, batch, opts.dataAugmentation{2}, opts.scale) ;
    if opts.prefetch
      nextBatch = train(t+opts.batchSize:min(t+2*opts.batchSize-1, numel(train)), opts.scale) ;
      getBatch(imdb, nextBatch, opts.dataAugmentation{2}, opts.scale) ;
    end
    
    if(exist('psi_n', 'var'))
        clear psi_n ima imb resa resb resc
        wait(gpuDevice);
        resc = [];
    end
    
    % do forward pass on neta and netb to get bilinear CNN features
    [psi, ~, ~] = bcnn_asym_forward(neta, netb, im, ...
        'regionBorder', opts.regionBorder, 'normalization', 'none');
    
    
    psi = cat(2, psi{:});
    
    psi = reshape(psi, [1,1,size(psi,1),size(psi,2)]);
    
    if opts.useGpu
      psi = gpuArray(psi) ;
    end
    
    
    netc.layers{end}.class = labels ;
    % do forward pass on netc after bilinear pool
    resc = vl_bilinearnn(netc, psi, [], resc, ...
      'conserveMemory', opts.conserveMemory, ...
      'sync', opts.sync) ;
   
  
  
    % print information
    batch_time = toc(batch_time) ;
    speed = numel(batch)/batch_time ;
    info.val = updateError(opts, info.val, netc, resc, batch_time) ;

    fprintf(' %.2f s (%.1f images/s)', batch_time, speed) ;
    n = t + numel(batch) - 1 ;
    fprintf(' err %.1f err5 %.1f', ...
      info.val.error(end)/n*100, info.val.topFiveError(end)/n*100) ;
    fprintf('\n') ;
  end

  % save
  info.train.objective(end) = info.train.objective(end) / numel(train) ;
  info.train.error(end) = info.train.error(end) / numel(train)  ;
  info.train.topFiveError(end) = info.train.topFiveError(end) / numel(train) ;
  info.train.speed(end) = numel(train) / info.train.speed(end) ;
  info.val.objective(end) = info.val.objective(end) / numel(val) ;
  info.val.error(end) = info.val.error(end) / numel(val) ;
  info.val.topFiveError(end) = info.val.topFiveError(end) / numel(val) ;
  info.val.speed(end) = numel(val) / info.val.speed(end) ;
  save(modelPath(epoch), 'neta', 'netb', 'netc', 'info', '-v7.3') ;

  figure(1) ; clf ;
  subplot(1,2,1) ;
  semilogy(1:epoch, info.train.objective, 'k') ; hold on ;
  semilogy(1:epoch, info.val.objective, 'b') ;
  xlabel('training epoch') ; ylabel('energy') ;
  grid on ;
  h=legend('train', 'val') ;
  set(h,'color','none');
  title('objective') ;
  subplot(1,2,2) ;
  switch opts.errorType
    case 'multiclass'
      plot(1:epoch, info.train.error, 'k') ; hold on ;
      plot(1:epoch, info.train.topFiveError, 'k--') ;
      plot(1:epoch, info.val.error, 'b') ;
      plot(1:epoch, info.val.topFiveError, 'b--') ;
      h=legend('train','train-5','val','val-5') ;
    case 'binary'
      plot(1:epoch, info.train.error, 'k') ; hold on ;
      plot(1:epoch, info.val.error, 'b') ;
      h=legend('train','val') ;
  end
  grid on ;
  xlabel('training epoch') ; ylabel('error') ;
  set(h,'color','none') ;
  title('error') ;
  drawnow ;
  print(1, modelFigPath, '-dpdf') ;
end

bcnn_net.neta = neta;
bcnn_net.netb = netb;
bcnn_net.netc = netc;

% -------------------------------------------------------------------------
function info = updateError(opts, info, net, res, speed)
% -------------------------------------------------------------------------
predictions = gather(res(end-1).x) ;
sz = size(predictions) ;
n = prod(sz(1:2)) ;

labels = net.layers{end}.class ;
info.objective(end) = info.objective(end) + sum(double(gather(res(end).x))) ;
info.speed(end) = info.speed(end) + speed ;
switch opts.errorType
  case 'multiclass'
    [~,predictions] = sort(predictions, 3, 'descend') ;
    error = ~bsxfun(@eq, predictions, reshape(labels, 1, 1, 1, [])) ;
    info.error(end) = info.error(end) +....
      sum(sum(sum(error(:,:,1,:))))/n ;
    info.topFiveError(end) = info.topFiveError(end) + ...
      sum(sum(sum(min(error(:,:,1:5,:),[],3))))/n ;
  case 'binary'
    error = bsxfun(@times, predictions, labels) < 0 ;
    info.error(end) = info.error(end) + sum(error(:))/n ;
end




function Ar = array_resize(A, w, h)
numChannels = size(A, 3);
indw = round(linspace(1,size(A,2),w));
indh = round(linspace(1,size(A,1),h));
Ar = zeros(w*h, numChannels, 'single');
for i = 1:numChannels,
    Ai = A(indh,indw,i);
    Ar(:,i) = Ai(:);
end


function [dA, dB] = compute_deriv_resp_AB(dEdpsi, A, B, useGpu)

w1 = size(A,2) ;
h1 = size(A,1) ;
w2 = size(B,2) ;
h2 = size(B,1) ;

%% make sure A and B have same aspect ratio
assert(w1/h1==w2/h2, 'Only support two feature maps have same aspect ration')


if w1*h1 <= w2*h2,
    %downsample B
    B = array_resize(B, w1, h1);
    A = reshape(A, [w1*h1 size(A,3)]);
else
    %downsample A
    A = array_resize(A, w2, h2);
    B = reshape(B, [w2*h2 size(B,3)]);
end

dA = B*dEdpsi';
dB = A*dEdpsi;


if w1*h1 <= w2*h2,
    %B is downsampled, upsample dB back to original size
    dB = reshape(dB, h1, w1, size(dB,2));
    tempdB = dB;
    if(useGpu)
        dB = gpuArray(zeros(h2, w2, size(B,2), 'single'));
    else
        dB = zeros(h2, w2, size(B,2), 'single');
    end
    
    indw = round(linspace(1,w2,w1));
    indh = round(linspace(1,h2,h1));
    dB(indh, indw, :) = tempdB;
    dA = reshape(dA, h1, w1, size(dA,2));
else
    %A is downsampled, upsample dA back to original size
    dA = reshape(dA, h2, h2, size(dA,2));  
    tempdA = dA;
    if(useGpu)
        dA = gpuArray(zeros(h1, w1, size(A,2), 'single'));
    else
        dA = zeros(h1, w1, size(A,2), 'single');
    end
    
    
    indw = round(linspace(1,w1,w2));
    indh = round(linspace(1,h1,h2));
    dA(indh, indw, :) = tempdA;  
    dB = reshape(dB, h2, w2, size(dB, 2));
end

