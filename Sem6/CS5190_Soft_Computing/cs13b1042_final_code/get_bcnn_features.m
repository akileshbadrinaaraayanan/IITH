function [code, varargout]= get_bcnn_features(neta, netb, im, varargin)
% GET_BCNN_FEATURES  Get bilinear cnn features for an image
%   This function extracts the binlinear combination of CNN features
%   extracted from two different networks.

% Copyright (C) 2015 Tsung-Yu Lin, Aruni RoyChowdhury, Subhransu Maji.
% All rights reserved.
%
% This file is part of the BCNN and is made available under
% the terms of the BSD license (see the COPYING file).

nVargOut = max(nargout,1)-1;

if nVargOut==1 
    assert(true, 'Number of output should not be two.')
end

opts.crop = true ;
%opts.scales = 2.^(1.5:-.5:-3); % try a bunch of scales
opts.scales = 2;
opts.encoder = [] ;
opts.regionBorder = 0.05;
opts.normalization = 'sqrt_L2';
opts = vl_argparse(opts, varargin) ;

% get parameters of the network
info = vl_simplenn_display(neta) ;
borderA = round(info.receptiveField(end)/2+1) ;
averageColourA = mean(mean(neta.normalization.averageImage,1),2) ;
imageSizeA = neta.normalization.imageSize;

info = vl_simplenn_display(netb) ;
borderB = round(info.receptiveField(end)/2+1) ;
averageColourB = mean(mean(netb.normalization.averageImage,1),2) ;
imageSizeB = netb.normalization.imageSize;

keepAspect = neta.normalization.keepAspect;

assert(all(imageSizeA == imageSizeB));
assert(neta.normalization.keepAspect==netb.normalization.keepAspect);

if ~iscell(im)
  im = {im} ;
end

code = cell(1, numel(im));

if nVargOut==2
    im_resA = cell(numel(im), 1);
    im_resB = cell(numel(im), 1);
end
% for each image
for k=1:numel(im)
    im_cropped = imresize(single(im{k}), imageSizeA([2 1]), 'bilinear');
    crop_h = size(im_cropped,1) ;
    crop_w = size(im_cropped,2) ;
    resA = [] ;
    resB = [] ;
    psi = cell(1, numel(opts.scales));
    % for each scale
    for s=1:numel(opts.scales)
        if min(crop_h,crop_w) * opts.scales(s) < min(borderA, borderB), continue ; end
        if sqrt(crop_h*crop_w) * opts.scales(s) > 1024, continue ; end
        
        
        if keepAspect
            w = size(im{k},2) ;
            h = size(im{k},1) ;
            factor = [imageSizeA(1)/h,imageSizeA(2)/w];
            
            
            factor = max(factor)*opts.scales(s) ;
            
            im_resized = imresize(single(im{k}), ...
                'scale', factor, ...
                'method', 'bilinear') ;
            
            w = size(im_resized,2) ;
            h = size(im_resized,1) ;
            
            im_resized = imcrop(im_resized, [fix((w-imageSizeA(1)*opts.scales(s))/2)+1, fix((h-imageSizeA(2)*opts.scales(s))/2)+1,...
              round(imageSizeA(1)*opts.scales(s))-1, round(imageSizeA(2)*opts.scales(s))-1]);
        else
            im_resized = imresize(single(im{k}), round(imageSizeA([2 1])*opts.scales(s)), 'bilinear');
        end
        im_resizedA = bsxfun(@minus, im_resized, averageColourA) ;
        im_resizedB = bsxfun(@minus, im_resized, averageColourB) ;
        if nVargOut==2
            im_resA{k} = im_resizedA;
            im_resB{k} = im_resizedB;
        end
        if neta.useGpu
            im_resizedA = gpuArray(im_resizedA) ;
            im_resizedB = gpuArray(im_resizedB) ;
        end
        resA = vl_simplenn(neta, im_resizedA, [], resA, ...
                            'conserveMemory', true, 'sync', true);
        resB = vl_simplenn(netb, im_resizedB, [], resB, ...
                            'conserveMemory', true, 'sync', true);
        A = gather(resA(end).x);
        B = gather(resB(end).x);

        psi{s} = bilinear_pool(A,B);
        feat_dim = max(cellfun(@length,psi));
        code{k} = zeros(feat_dim, 1);
    end
    % pool across scales
    for s=1:numel(opts.scales),
        if ~isempty(psi{s}),
            code{k} = code{k} + psi{s};
        end
    end
    assert(~isempty(code{k}));
end
% square-root and l2 normalize (like: Improved Fisher?)
switch opts.normalization
    case 'sqrt_L2'
        for k=1:numel(im),
            code{k} = sign(code{k}).*sqrt(abs(code{k}));
            code{k} = code{k}./(norm(code{k}+eps));
        end
    case 'L2'
        for k=1:numel(im),
            code{k} = code{k}./(norm(code{k}+eps));
        end
    case 'sqrt'
        for k=1:numel(im),
            code{k} = sign(code{k}).*sqrt(abs(code{k}));            
        end
    case 'none'
end

if nVargOut==2
    varargout{1} = cat(4, im_resA{:});
    varargout{2} = cat(4, im_resB{:});
end


function psi = bilinear_pool(A, B)
w1 = size(A,2) ;
h1 = size(A,1) ;
w2 = size(B,2) ;
h2 = size(B,1) ;

if w1*h1 <= w2*h2,
    %downsample B
    B = array_resize(B, w1, h1);
    A = reshape(A, [w1*h1 size(A,3)]);
else
    %downsample A
    A = array_resize(A, w2, h2);
    B = reshape(B, [w2*h2 size(B,3)]);
end

% bilinear pool
psi = A'*B;
psi = psi(:);

function Ar = array_resize(A, w, h)
numChannels = size(A, 3);
indw = round(linspace(1,size(A,2),w));
indh = round(linspace(1,size(A,1),h));
Ar = zeros(w*h, numChannels, 'single');
for i = 1:numChannels,
    Ai = A(indh,indw,i);
    Ar(:,i) = Ai(:);
end
