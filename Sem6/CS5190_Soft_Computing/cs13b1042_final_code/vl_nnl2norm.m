function y = vl_nnl2norm(x, varargin)
% VL_L2NORM computes l2 normalization at each location

% Copyright (C) 2015 Tsung-Yu Lin, Aruni RoyChowdhury, Subhransu Maji.
% All rights reserved.
%
% This file is part of the BCNN and is made available under
% the terms of the BSD license (see the COPYING file).

% flag for doing backward pass
backMode = numel(varargin) > 0 && ~isstr(varargin{1}) ;
if backMode
  dzdy = varargin{1} ;
end

% number of channels
ch = size(x, 3);

% norm of features
x_norm = sqrt(sum(x.*x, 3)+eps);
if backMode
    % do backward pass
    E = bsxfun(@times, dzdy, x_norm.^(-1));
    F = sum(x.*dzdy,3);
    F = F.*x_norm.^(-3);
    F = bsxfun(@times, x, F);
    y = E-F;
    
else
    % do forward pass
    y = x./repmat(x_norm, [1, 1, ch, 1]);
end