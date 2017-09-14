function [y, varargout] = vl_nnbilinearclpool(x1, x2, varargin)
% VL_NNBILINEARPOOL pools bilinear feature across all locations

% Copyright (C) 2015 Tsung-Yu Lin, Aruni RoyChowdhury, Subhransu Maji.
% All rights reserved.
%
% This file is part of the BCNN and is made available under
% the terms of the BSD license (see the COPYING file).

% flag for doing backward pass
backMode = numel(varargin) > 0 && ~isstr(varargin{1}) ;
if backMode
  dzdy = varargin{1};
end

% if GPU is used
gpuMode = isa(x1, 'gpuArray');

% [height width channels batchsize]
[h1, w1, ch1, bs] = size(x1);
[h2, w2, ch2, ~] = size(x2);


assert(h1==h2&&w1==w2, 'two layer outputs must be the same resolution');
h = h1; w = w1;


if backMode
    % do backward pass
    if gpuMode
        y1 = gpuArray(zeros(size(x1), 'single'));
        y2 = gpuArray(zeros(size(x2), 'single'));
    else
        y1 = zeros(size(x1), 'single');
        y2 = zeros(size(x2), 'single');
    end
    for b=1:bs
        dzdy_b = reshape(dzdy(1,1,:,b), [ch1, ch2]);
        A = reshape(x1(:,:,:,b), [h1*w1, ch1]);
        B = reshape(x2(:,:,:,b), [h2*w2, ch2]);
        y2(:,:,:,b) = reshape(A*dzdy_b, [h1, w1, ch2]);
        y1(:,:,:,b) = reshape(B*dzdy_b', [h2, w2, ch1]);
        
    end
    y = y1;
    varargout{1} = y2;
else
    % do forward pass
    if gpuMode
        y = gpuArray(zeros([1, 1, ch1*ch2, bs], 'single'));
    else
        y = zeros([1, 1, ch1*ch2, bs], 'single');
    end
    for b = 1:bs,
                
        xa = reshape(x1(:,:,:,b), [h*w, ch1]);
        xb = reshape(x2(:,:,:,b), [h*w, ch2]);
        
        y(1,1,:, b) = reshape(xa'*xb, [1 ch1*ch2]);
    end
end

