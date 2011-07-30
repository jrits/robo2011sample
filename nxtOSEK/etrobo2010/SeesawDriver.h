///////////////////////////////////////////////////////////
//  SeesawDriver.h
//
//  Created on:      10-9-2010 19:43:43
//  Original author: masui
///////////////////////////////////////////////////////////

#ifndef SEESAWDRIVER_H
#define SEESAWDRIVER_H

#include "Driver.h"

/**
  �V�[�\�[�h���C�o
*/
class SeesawDriver : public Driver
{
public:
    /**
     * �T�u���
     */
	enum eSubSection {
        INIT = -1,
        BEFORELINETRACE,
        ONTHESEESAW,
        LINERETURN,
		AFTERLINETRACE
    };
private:
    int mState;    //!< ���
    int mTimeCounter; //!< �^�C�}�J�E���^
	bool mInitState; //!< �������
	float mStartAngleCounter; //!< �����p�x�J�E���^
public:
	SeesawDriver();
	~SeesawDriver();
    bool drive();

};
#endif // !SEESAWDRIVER_H
