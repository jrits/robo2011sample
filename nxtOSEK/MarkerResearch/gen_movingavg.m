% gen_movingavg - ���͐M������ړ����ϐM���𐶐�����
%
% Synopsis
%   [MEAN, STD, X] = gen_movingavg(x, noverlap, nsample)
%
% Description
%   ���͐M������ړ����ϐM���𐶐�����
%
% Inputs ([]s are optional)
%   (vector) x        1 x N. ���͐M��
%   (scalar) noverlap ���̃t���[��(���ς����͈�)�ƃI�[�o�[���b�v����T���v���̐�
%   (scalar) nsample  �e�t���[��(���ς����͈�)�̃T���v���̐�
%
% Outputs ([]s are optional)
%   (vector) MEAN     1 x K. �ړ����ϐM��
%   (vector) STD      1 x K. �ړ��΍��M��
%   (vector) X        1 x K. �v���b�g����ۂɂ��悤����x���̒l�B�e�t���[���̒����̓Y��
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
%   2009-07-23  First Edition
function [MEAN, STD, X] = gen_movingavg(x, noverlap, nsample)
 N = length(x);
 MEAN = [];
 STD = [];
 pos = 1;
 while (pos+nsample <= N)
     frame = x(pos:pos+nsample-1);
     pos = pos + (nsample - noverlap);
	 MEAN = [MEAN mean(frame)];
	 STD = [STD sqrt(cov(frame))]; % biased variance
 end
 X = round(nsample/2):(nsample-noverlap):N-1-round(nsample/2);
endfunction


