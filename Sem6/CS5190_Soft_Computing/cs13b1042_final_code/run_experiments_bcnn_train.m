function run_experiments_bcnn_train()

% Copyright (C) 2015 Tsung-Yu Lin, Aruni RoyChowdhury, Subhransu Maji.
% All rights reserved.
%
% This file is part of the BCNN and is made available under
% the terms of the BSD license (see the COPYING file).

%fine tuning bcnn models
if(~exist('data', 'dir'))
    mkdir('data');
end

  bcnnmm.name = 'bcnnmm' ;
  bcnnmm.opts = {...
    'type', 'bcnn', ...
    'modela', '../data/models/imagenet-vgg-m.mat', ...     % intialize network A with pre-trained model
    'layera', 14,...                                    % specify the output of certain layer of network A to be bilinearly combined
    'modelb', '../data/models/imagenet-vgg-m.mat', ...     % intialize network B with pre-trained model 
    'layerb', 14,...                                    % specify the output of certain layer of network B to be bilinearly combined
    'shareWeight', true,...                             % true: symmetric implementation where two networks are identical
    } ;

  bcnnvdm.name = 'bcnnvdm' ;
  bcnnvdm.opts = {...
    'type', 'bcnn', ...
    'modela', '../data/models/imagenet-vgg-verydeep-16.mat', ...
    'layera', 30,...
    'modelb', '../data/models/imagenet-vgg-m.mat', ...
    'layerb', 14,...
    'shareWeight', false,...                            % false: asymmetric implementation where two networks are distinct
    } ;

  bcnnvdvd.name = 'bcnnvdvd' ;
  bcnnvdvd.opts = {...
    'type', 'bcnn', ...
    'modela', '../data/models/imagenet-vgg-verydeep-16.mat', ...
    'layera', 30,...
    'modelb', '../data/models/imagenet-vgg-verydeep-16.mat', ...
    'layerb', 30,...
    'shareWeight', true,...
    };

    
  setupNameList = {'bcnnvdm'};
  encoderList = {{bcnnvdm}}; 
  datasetList = {{'cub', 1}};  

  for ii = 1 : numel(datasetList)
    dataset = datasetList{ii} ;
    if iscell(dataset)
      numSplits = dataset{2} ;
      dataset = dataset{1} ;
    else
      numSplits = 1 ;
    end
    for jj = 1 : numSplits
      for ee = 1: numel(encoderList)
        
          [opts, imdb] = model_setup('dataset', dataset, ...
			  'encoders', encoderList{ee}, ...
			  'batchSize', 1, ...
              'bcnnScale', 2, ...       % specify the scale of input images
              'bcnnLRinit', true, ...   % do logistic regression to initilize softmax layer
              'dataAugmentation', {'f2','none','none'},...      % do data augmentation [train, val, test]. Only support flipping for train set on current release.
			  'useGpu', 1, ...          %specify the GPU to use. 0 for using CPU
              'numEpochs', 45, ...
              'momentum', 0.3);
          imdb_bcnn_train(imdb, opts);
      end
    end
  end
end

%{
The following are the setting we run in which fine-tuning works stable without GPU memory issues on Nvidia K40.
m-m model: batchSize 64, momentum 0.9
d-m model: batchSize 1, momentum 0.3
%}

