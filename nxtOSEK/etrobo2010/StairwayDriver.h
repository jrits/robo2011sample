///////////////////////////////////////////////////////////
//  StairwayDriver.h
//
//  Created on:      10-9-2010 19:43:43
//  Original author: hayama
///////////////////////////////////////////////////////////

#ifndef STAIRWAYDRIVER_H
#define STAIRWAYDRIVER_H

#include "Driver.h"

/**
 * �K�i�h���C�o
 */
class StairwayDriver : public Driver
{
public:
    /**
     * �T�u���
     */
    enum eSubSection {
        INIT = -1,
        BEFORELINETRACE,
        ON1STSTAGE,
        ON2NDSTAGE,
        DROPDOWN,
        AFTERLINETRACE
    };
private:
    int mState; //!< �K�i���
    bool mInitState; //!< ��ԏ������t���O
    int mTimeCounter; //!< �^�C���J�E���^
    int mPrevMotor; //!< �O�̃��[�^�̉�]��
    bool mDoDetectWall; //!< ��ǂ𔭌��t���O
    float mPrevDirection; //!< �O�̕���
public:
    StairwayDriver();
	~StairwayDriver(){}
    bool drive();
};
#endif // STAIRWAYDRIVER_H

