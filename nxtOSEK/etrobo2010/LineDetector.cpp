//
// LineDetector.cpp
//

#include "LineDetector.h"
#include "constants.h"
#include "factory.h"

#define LINEDETECT_THRESHOLD 20 // �����m���Ȃ����x�ɏ�������Ηǂ�

/**
 * ���C�������m����
 *
 * @return ���C�����m�̌���
 */
LineDetector::ePattern LineDetector::detect()
{
    LineDetector::ePattern linePattern;

    // ���ω���
    float lightDiff = mLightHistory.calcDifference();
    //lightDiff *= (100/FORWARD); // ���x���l���ɓ����

    if (lightDiff >= LINEDETECT_THRESHOLD) { // ��=>���p�^�[��
        linePattern =  WHITE_TO_BLACK;
    }
    else if (lightDiff <= (-1)*LINEDETECT_THRESHOLD) { // ���˔��p�^�[��
        linePattern = BLACK_TO_WHITE;
    }
    // ���ꂾ�����ƊD�F���m������B���Ƃ�����B
    else if (mLightHistory.get(LATEST) > LINE_THRESHOLD) { // ���ˍ�
        linePattern = ON_BLACK;
    }
    else { // ���˔�
        linePattern = ON_WHITE;
    }

#if 0
    // ���O���M
    LOGGER_SEND = 2;
	LOGGER_DATAS08[0] = (S8)(linePattern);
	LOGGER_DATAS16[0] = (S16)(mLightHistory.get(LATEST));
	LOGGER_DATAS16[1] = (S16)(mLightHistory.get(OLDEST));
	LOGGER_DATAS16[2] = (S16)(mLightHistory.calcDifference());
	LOGGER_DATAS16[3] = (S16)(lightDiff);
#endif

    return linePattern;
}
