% gen_hist - �q�X�g�O�������擾����
%
% Synopsis
%   [hist, xout] = gen_hist(x, intervals)
%
% Description
%   �q�X�g�O�������擾����
%
% Inputs ([]s are optional)
%   (vector) x         ���͐M��
%   (vector) intervals �q�X�g�O�����̒l�Ԋu
%
% Outputs ([]s are optional)
%   (vector) hist      �q�X�g�O����
%   (vector) xout      �v���b�g����ۂɎg�p����x���̒l�Bintervals
%
% Examples
%    x = rand(1,100);
%    intervals = 0:0.1:1;
%    [hist, xout] = gen_hist(x, intervals);
%    plot(xout, hist);
%
% See also

% Authors
%   Naotoshi Seo <sonots(at)sonots.com>
%
% License
%   MIT License.
%
% Changes
%   2009-07-22  First Edition
function [hist, xout] = gen_hist(x, intervals)
hist = [];
for i = 1:length(intervals)-1
	hist(i) = sum((x >= intervals(i)) & (x < intervals(i+1)));
end
xout = intervals(1:end-1);
endfunction
