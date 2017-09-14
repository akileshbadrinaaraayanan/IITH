function [code, varargout]= bcnn_asym_forward(neta, netb, im, varargin)
% BCNN_ASYM_FORWARD  run the forward passing of the two networks and output the 
% bilinear cnn features for batch of images. The images are pre-cropped,
% resized and mean subtracted. The function doesn't preprocess the images
% instead just get the bcnn outputs.

% INPUT
%       neta: network A beased on MatConvNet structure
%       netb: network B beased on MatConvNet structure
%       im{1} : cell array of images input for network A
%       im{2} : cell array of images input for network B

% OUTPUT
% code: cell array of output bcnn codes
% varargout: 
%       varargout{1}: output of network A 
%       varargout{2}: output of network B

% Copyright (C) 2015 Tsung-Yu Lin, Aruni RoyChowdhury, Subhransu Maji.
% All rights reserved.
%
% This file is part of the BCNN and is made available under
% the terms of the BSD license (see the COPYING file).


nVargOut = max(nargout,1)-1;

if nVargOut==1 || nVargOut==2 || nVargOut==3
    assert(true, 'Number of output problem')
end

% basic setting
opts.crop = true ;
opts.encoder = [] ;
opts.regionBorder = 0.05;
opts.normalization = 'sqrt';
opts.networkconservmemory = true;
opts = vl_argparse(opts, varargin) ;

% re-structure the images to 4-D arrays
im_resA = im{1};
im_resB = im{2};

N = numel(im_resA);

im_resA = cat(4, im_resA{:});
im_resB = cat(4, im_resB{:});

resA = [] ;
resB = [] ;
code = cell(1, N);

% move images to GPU
if neta.useGpu
    im_resA = gpuArray(im_resA) ;
    im_resB = gpuArray(im_resB) ;
end

% forward passsing
resA = vl_bilinearnn(neta, im_resA, [], resA, ...
    'conserveMemory', opts.networkconservmemory, 'sync', true);
resB = vl_bilinearnn(netb, im_resB, [], resB, ...
    'conserveMemory', opts.networkconservmemory, 'sync', true);

% get the output of the last layers
A = gather(resA(end).x);
B = gather(resB(end).x);

% compute outer product and pool across pixels for each image
for k=1:N
    code{k} = bilinear_pool(squeeze(A(:,:,:,k)), squeeze(B(:,:,:,k)));
    assert(~isempty(code{k}));
end
        

% square-root and l2 normalize (like: Improved Fisher?)
switch opts.normalization
    case 'sqrt'
        for k=1:N,
            code{k} = sign(code{k}).*sqrt(abs(code{k}));
            code{k} = code{k}./(norm(code{k}+eps));
        end
    case 'L2'
        for k=1:N,
            code{k} = code{k}./(norm(code{k}+eps));
        end
    case 'none'
end

if nVargOut==2
    varargout{1} = resA;
    varargout{2} = resB;
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
