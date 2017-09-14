function imo = imdb_get_batch_bcnn(images, varargin)
% imdb_get_batch_bcnn  Load, preprocess, and pack images for BCNN evaluation
% For asymmetric model, the function preprocesses the images twice for two networks
% separately.

% OUTPUT
% imo: a cell array where each element is a cell array of images.
%       For symmetric bcnn model, numel(imo) will be 1 and imo{1} will be a
%       cell array of images
%       For asymmetric bcnn, numel(imo) will be 2. imo{1} is a cell array containing the preprocessed images for network A 
%       Similarly, imo{2} is a cell array containing the preprocessed images for network B

%
% Copyright (C) 2015 Tsung-Yu Lin, Aruni RoyChowdhury, Subhransu Maji.
% All rights reserved.
%
% This file is part of the BCNN and is made available under
% the terms of the BSD license (see the COPYING file).
%
% This file modified from CNN_IMAGENET_GET_BATCH of MatConvNet


for i=1:numel(varargin{1})
    opts(i).imageSize = [227, 227] ;
    opts(i).border = [0, 0] ;
    opts(i).averageImage = [] ;
    opts(i).augmentation = 'none' ;
    opts(i).interpolation = 'bilinear' ;
    opts(i).numAugments = 1 ;
    opts(i).numThreads = 0 ;
    opts(i).prefetch = false ;
    opts(i).keepAspect = false;
    opts(i).scale = 1;
    opts(i) = vl_argparse(opts(i), {varargin{1}(i),varargin{2:end}});
    %opts(i) = vl_argparse(opts(i), varargin(2:end));
    
    if(i==1)
		switch opts(i).augmentation
		  case 'none'
		    tfs = [.5 ; .5 ; 0 ];
		  case 'f2'
		    tfs = [...
			0.5 0.5 ;
			0.5 0.5 ;
			  0   1];
		end
		
		[~,augmentations] = sort(rand(size(tfs,2), numel(images)), 1) ;
    end
    
    imo{i} = get_batch_fun(images, opts(i), augmentations, tfs);
    
end




function imo = get_batch_fun(images, opts, augmentations, tfs)

opts.imageSize(1:2) = round(opts.imageSize(1:2).*opts.scale);
if(opts.scale ~= 1)
    opts.averageImage = mean(mean(opts.averageImage, 1),2);
end

% fetch is true if images is a list of filenames (instead of
% a cell array of images)
% fetch = numel(images) > 1 && ischar(images{1}) ;
fetch = ischar(images{1}) ;

% prefetch is used to load images in a separate thread
prefetch = fetch & opts.prefetch ;



im = cell(1, numel(images)) ;
if opts.numThreads > 0
  if prefetch
    vl_imreadjpeg(images, 'numThreads', opts.numThreads, 'prefetch') ;
    imo = [] ;
    return ;
  end
  if fetch
    im = vl_imreadjpeg(images,'numThreads', opts.numThreads) ;
  end
end
if ~fetch
  im = images ;
end

imo = cell(1, numel(images)*opts.numAugments) ;

si=1;
for i=1:numel(images)

  % acquire image
  if isempty(im{i})
    imt = imread(images{i}) ;
    imt = single(imt) ; % faster than im2single (and multiplies by 255)
  else
    imt = im{i} ;
  end
  if size(imt,3) == 1
      imt = cat(3, imt, imt, imt) ;
  end
  
  % resize
      if opts.keepAspect
      w = size(imt,2) ;
      h = size(imt,1) ;
      factor = [(opts.imageSize(1)+opts.border(1))/h ...
          (opts.imageSize(2)+opts.border(2))/w];
      
          factor = max(factor) ;
          if any(abs(factor - 1) > 0.0001)
              
              imt = imresize(imt, ...
                  'scale', factor, ...
                  'method', opts.interpolation) ;
          end
          
          w = size(imt,2) ;
          h = size(imt,1) ;
          
          imt = imcrop(imt, [fix((w-opts.imageSize(1))/2)+1, fix((h-opts.imageSize(2))/2)+1, opts.imageSize(1)-1, opts.imageSize(2)-1]);
      else
          imt = imresize(imt, ...
              opts.imageSize(1:2), ...
              'method', opts.interpolation) ;          
      end

  
  % crop & flip
  w = size(imt,2) ;
  h = size(imt,1) ;
  for ai = 1:opts.numAugments
    t = augmentations(ai,i) ;
    tf = tfs(:,t) ;
    sx = 1:w;
    if tf(3), sx = fliplr(sx) ; end
    imo{si} = imt(:,sx,:);
    si = si + 1 ;
  end
end


if ~isempty(opts.averageImage)
    for i=1:numel(imo)
        imo{i} = bsxfun(@minus, imo{i}, opts.averageImage) ;
    end
end
