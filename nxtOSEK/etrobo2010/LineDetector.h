//
// LineDetector.h
//

#ifndef LineDetector_H_
#define LineDetector_H_

#include "Detector.h"

/**
 * ���C�����m
 *
 * ���C�����A�Ɏg��
 */
class LineDetector : public Detector
{
public:
    /**
     * ���C�����m�̃p�^�[��������
     */
    enum ePattern {
        ON_WHITE,       //!< ��=>��(���C���O)
        WHITE_TO_BLACK, //!< ��=>��
        BLACK_TO_WHITE, //!< ��=>��
        ON_BLACK,       //!< ��=>��
    };
public:
	LineDetector(){}
	~LineDetector(){}
    LineDetector::ePattern detect();
};

#endif
