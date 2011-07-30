//
// OutGarageDriver.h
//

#ifndef OutGarageDriver_H_
#define OutGarageDriver_H_

#include "Driver.h"

/**
 * �A�E�g�R�[�X�K���[�W�E�C���h���C�o
 */
class OutGarageDriver : public Driver
{
private:
    /**
     * �T�u���
     */
    enum eSubSection {
        INIT = -1,
        BEFORELINETRACE,
		STRAIGHTLINETRACE,
    	PREPAREENTERGARAGE,
    	STRAIGHTANGLETRACE,
    	GOGARAGE,
    	ENTERGARAGE,
        FIN_GARAGEIN,
    };
    eSubSection mState; //!< ���
    int mTimeCounter;   //!< �^�C���J�E���^
    bool mGrayThroughFlag; //!< �����B�D�F��ԃW�����v
public:
	OutGarageDriver();
	~OutGarageDriver(){}
    bool drive();
};

#endif
