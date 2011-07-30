## run_research(4)
## �A���S���Y��4. ���F�������Ԍ���Ȃ���ΊD�F��ԁB�Ȃ߂炩�ȃ��C���g���[�X�ł͎g���Ȃ��A���S���Y���B

## Author: nseo <nseo@NSEO>
## Created: 2009-07-23

Ts = 4; % 4ms
GRAY = 610; % �D�F�l
NSAMPLE = 600; % �D�F��Ԃł���Ɣ��f�������̂ɗp����T���v����

daq = read_light_daq('light_daq/daq.csv');

GRAYPOINT = daq < GRAY;
figure; plot(GRAYPOINT); title('GRAYPOINT'); xlabel('Sampling Time'); ylabel('Result');
ylim([0 1.1]);
print -deps 'daq_graypoint.eps'; print -djpg 'daq_graypoint.jpg';

CUMSUM_GRAYPOINT = [1];
for i = 2:1:length(GRAYPOINT)
    if (daq(i) < GRAY)
        CUMSUM_GRAYPOINT(i) = CUMSUM_GRAYPOINT(i-1) + 1;
	else
	    CUMSUM_GRAYPOINT(i) = 0;
    end
end
figure; plot(CUMSUM_GRAYPOINT); title('CUMSUM GRAYPOINT'); xlabel('Sampling Time'); ylabel('Result');
print -deps 'daq_cumsum_graypoint.eps'; print -djpg 'daq_cumsum_graypoint.jpg';

MARKER = CUMSUM_GRAYPOINT > NSAMPLE;
figure; plot(MARKER); title('MARKER'); xlabel('Sampling Time'); ylabel('Result');
ylim([0 1.1]);
print -deps 'daq_marker_algorithm4.eps'; print -djpg 'daq_marker_algorithm4.jpg';
