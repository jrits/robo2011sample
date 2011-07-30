//
// InGarageDriver.h
//

#ifndef InGarageDriver_H_
#define InGarageDriver_H_

#include "Driver.h"

/**
 * �C���R�[�X�K���[�W�E�C���h���C�o
 */
class InGarageDriver : public Driver
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
    	ENTERGARAGE,
    };
    eSubSection mState; //!< ���
    int mTimeCounter;   //!< �^�C���J�E���^
public:
	InGarageDriver();
	~InGarageDriver(){}
    bool drive();
};

#endif
