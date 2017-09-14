function y = vl_nnsqrt(x, varargin)
% VL_NNSQRT signed square-root feature

% Copyright (C) 2015 Tsung-Yu Lin, Aruni RoyChowdhury, Subhransu Maji.
% All rights reserved.
%
% This file is part of the BCNN and is made available under
% the terms of the BSD license (see the COPYING file).

% small value to prevent divided by zero on gradient computation
thres = 10^-8;

% flag for doing backward pass 
backMode = numel(varargin) > 0 && ~isstr(varargin{1}) ;
if backMode
  dzdy = varargin{1} ;
end

if backMode
    % backward pass
    y = 0.5./sqrt(abs(x)+thres);
    y = y.*dzdy;
else
    %forward pass
    y = sign(x).*sqrt(abs(x));
end

