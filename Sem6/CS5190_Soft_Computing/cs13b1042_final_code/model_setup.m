function [opts, imdb] = model_setup(varargin)


% Copyright (C) 2015 Tsung-Yu Lin, Aruni RoyChowdhury, Subhransu Maji.
% All rights reserved.
%
% This file is part of the BCNN and is made available under
% the terms of the BSD license (see the COPYING file).

setup ;

opts.seed = 1 ;
opts.batchSize = 128 ;
opts.numEpochs = 100;
opts.momentum = 0.9;
opts.keepAspect = true;
opts.useVal = false;
opts.useGpu = 0 ;
opts.regionBorder = 0.05 ;
opts.numDCNNWords = 64 ;
opts.numDSIFTWords = 256 ;
opts.numSamplesPerWord = 1000 ;
opts.printDatasetInfo = true ;
opts.excludeDifficult = true ;
opts.datasetSize = inf;
opts.encoders = {struct('type', 'rcnn', 'opts', {})} ;
opts.dataset = 'cub' ;
opts.carsDir = '../data/car_ims';
opts.cubDir = '../data/CUB_200_2011';
opts.aircraftDir = '../data/oid-aircraft-beta-1';
opts.suffix = 'baseline' ;
opts.prefix = 'v1' ;
opts.model  = '../data/models/imagenet-vgg-m.mat';
opts.modela = '../data/models/imagenet-vgg-m.mat';
opts.modelb = '../data/models/imagenet-vgg-s.mat';
opts.layer  = 14;
opts.layera = 14;
opts.layerb = 14;
%opts.bcnn = false;
opts.bcnnScale = 1;
opts.bcnnLRinit = false;
opts.bcnnLayer = 14;
opts.dataAugmentation = {'none', 'none', 'none'};
[opts, varargin] = vl_argparse(opts,varargin) ;

opts.expDir = sprintf('data/%s/%s-seed-%02d', opts.prefix, opts.dataset, opts.seed) ;
opts.imdbDir = fullfile(opts.expDir, 'imdb') ;
opts.resultPath = fullfile(opts.expDir, sprintf('result-%s.mat', opts.suffix)) ;

opts = vl_argparse(opts,varargin) ;

if nargout <= 1, return ; end

% Setup GPU if needed
if opts.useGpu
  gpuDevice(opts.useGpu) ;
end

% -------------------------------------------------------------------------
%                                                            Setup encoders
% -------------------------------------------------------------------------

models = {} ;
modelPath = {};
for i = 1:numel(opts.encoders)
  if isstruct(opts.encoders{i})
    name = opts.encoders{i}.name ;
    opts.encoders{i}.path = fullfile(opts.expDir, [name '-encoder.mat']) ;
    opts.encoders{i}.codePath = fullfile(opts.expDir, [name '-codes.mat']) ;
    [md, mdpath] = get_cnn_model_from_encoder_opts(opts.encoders{i});
    models = horzcat(models, md) ;
    modelPath = horzcat(modelPath, mdpath);
%     models = horzcat(models, get_cnn_model_from_encoder_opts(opts.encoders{i})) ;
  else
    for j = 1:numel(opts.encoders{i})
      name = opts.encoders{i}{j}.name ;
      opts.encoders{i}{j}.path = fullfile(opts.expDir, [name '-encoder.mat']) ;
      opts.encoders{i}{j}.codePath = fullfile(opts.expDir, [name '-codes.mat']) ;
      [md, mdpath] = get_cnn_model_from_encoder_opts(opts.encoders{i}{j});      
      models = horzcat(models, md) ;
      modelPath = horzcat(modelPath, mdpath);
%       models = horzcat(models, get_cnn_model_from_encoder_opts(opts.encoders{i}{j})) ;
    end
  end
end

% -------------------------------------------------------------------------
%                                                       Download CNN models
% -------------------------------------------------------------------------

for i = 1:numel(models)
    if ~exist(modelPath{i})
        error(['cannot find model ', models{i}]) ;
    end
end

% -------------------------------------------------------------------------
%                                                              Load dataset
% -------------------------------------------------------------------------

vl_xmkdir(opts.expDir) ;
vl_xmkdir(opts.imdbDir) ;

imdbPath = fullfile(opts.imdbDir, sprintf('imdb-seed-%d.mat', opts.seed)) ;
if exist(imdbPath)
  imdb = load(imdbPath) ;
  return ;
end

switch opts.dataset
    case 'cubcrop'
        imdb = cub_get_database(opts.cubDir, true, false);
    case 'cub'
        imdb = cub_get_database(opts.cubDir, false, opts.useVal);
    case 'aircraft-variant'
        imdb = aircraft_get_database(opts.aircraftDir, 'variant');
    case 'cars'
        imdb = cars_get_database(opts.carsDir, false, opts.useVal);
    otherwise
        error('Unknown dataset %s', opts.dataset) ;
end

save(imdbPath, '-struct', 'imdb') ;

if opts.printDatasetInfo
  print_dataset_info(imdb) ;
end

% -------------------------------------------------------------------------
function [model, modelPath] = get_cnn_model_from_encoder_opts(encoder)
% -------------------------------------------------------------------------
p = find(strcmp('model', encoder.opts)) ;
if ~isempty(p)
  [~,m,e] = fileparts(encoder.opts{p+1}) ;
  model = {[m e]} ;
  modelPath = encoder.opts{p+1};
else
  model = {} ;
  modelPath = {};
end

% bilinear cnn models
p = find(strcmp('modela', encoder.opts)) ;
if ~isempty(p)
  [~,m,e] = fileparts(encoder.opts{p+1}) ;
  model = horzcat(model,{[m e]}) ;
  modelPath = horzcat(modelPath, encoder.opts{p+1});
end
p = find(strcmp('modelb', encoder.opts)) ;
if ~isempty(p)
  [~,m,e] = fileparts(encoder.opts{p+1}) ;
  model = horzcat(model,{[m e]}) ;
  modelPath = horzcat(modelPath, encoder.opts{p+1});
end


