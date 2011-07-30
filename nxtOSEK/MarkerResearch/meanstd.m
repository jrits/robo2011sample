% meanstd - ���ϒl�ƕ΍��l�̌v�Z
%
% Synopsis
%   [mu, std] = meanstd(data)
%
% Description
%   ���ϒl�ƕ΍��l�̌v�Z
%
% Inputs ([]s are optional)
%   (vector) data     1 x N. �f�[�^�Q�B
%
% Outputs ([]s are optional)
%   (scalar) mu       ����
%   (scalar) std      �΍�
%
% Examples
%
% See also

% Authors
%   Naotoshi Seo <sonots(at)sonots.com>
%
% License
%   MIT License.
%
% Changes
%   2009-07-15  First Edition
function [mu, std] = meanstd(data)
mu = mean(data);
std = sqrt(cov(data));
endfunction
