//
// StraightDetector.cpp
//

#include "StraightDetector.h"
#include "constants.h"
#include "factory.h"
#include "macro.h"

/**
 * ���������m����
 *
 * @return �������m�̌���
 */
bool StraightDetector::detect()
{
    // �ړ����ό����̕ω��ʂ�3�x�ȉ��Ȃ璼��
    float diffDirection = mDirectionAverageHistory.calcDifference();
    bool isStraight = (ABS(diffDirection) < 3.0);

#if 0
    // ���O���M
    LOGGER_SEND = 1;
    LOGGER_DATAS08[0] = (S8)(isStraight);
	LOGGER_DATAS32[0] = (S32)(diffDirection);
#endif
    
    return isStraight;
}
