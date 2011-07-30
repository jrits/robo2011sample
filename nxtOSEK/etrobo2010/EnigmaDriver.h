//
// EnigmaDriver.h
//

#ifndef EnigmaDriver_H_
#define EnigmaDriver_H_

#include "Driver.h"

/**
 * �G�j�O�}�E�f�R�[�f�B���O�h���C�o
 */
class EnigmaDriver : public Driver
{
private:
    /**
     * �T�u���
     */
    enum eSubSection {
        INIT = -1,
        BEFORELINETRACE,
        PREPAREENTERPET,
        ENTERPET,
        LEFTPET,
        RIGHTPET,
        EXITPET,
        LINERETURN,
        AFTERLINETRACE
    };
    eSubSection mState;    //!< ���
    bool mInitState;       //!< ��ԏ������t���O
    int  mTimeCounter;     //!< �^�C���J�E���^
    bool mIsArrived;       //!< isArrived�ۑ��o�b�t�@
    bool mObstacleFlag[2]; //!< �՗����m�t���O�B�Q�{�����݂Ȃ�
    float mOrigK_PHIDOT;   //!< �ύX�O��K_PHIDOT��ۑ����Ă����o�b�t�@
public:
	EnigmaDriver();
	~EnigmaDriver(){}
    bool drive();
public:
    bool detect();
    int decode();
};

#endif
