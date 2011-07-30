//
// InGarageDriver.cpp
//

#include "InGarageDriver.h"
#include "factory.h"
#include "constants.h"

/**
 * �R���X�g���N�^
 */
InGarageDriver::InGarageDriver()
{
    mState = InGarageDriver::INIT;
}

/**
 * �C���R�[�X�K���[�W�E�C����Ԃ��U������
 *
 * �⓹�������C���g���[�X����A�K���[�W�E�C���܂ł�S������B
 *
 * @retval true �ŏI��ԁB����ȍ~�̏�ԑJ�ڂȂ��B
 * @retval false �܂���ڂ��������Ă��܂���B
 */
bool InGarageDriver::drive()
{
#if 0 // ���O���M(0�F�����A1�F���{)
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
	LOGGER_DATAS32[0] = (S32)(mGps.getXCoordinate());
	LOGGER_DATAS32[1] = (S32)(mGps.getYCoordinate());
	LOGGER_DATAS32[2] = (S32)(mGps.getDirection());
	LOGGER_DATAS32[3] = (S32)(mGps.getDistance());
#endif
#if 1 // �f�o�b�O(0�F�����A1�F���{)
    //DESK_DEBUG = true; // ���[�^���񂳂Ȃ��f�o�b�O
    static int count = 0; // static�͊�{�֎~�����A�f�o�b�O�Ȃ̂Ŏg�p
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "InGarageDriver");
        lcd.putf("dn", mState);
        lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        lcd.putf("dn", (S32)(mGps.getDirection()));
        lcd.putf("dn", (S32)(mGps.getDistance()));
        lcd.disp();
    }
#endif
    if (mState == InGarageDriver::INIT) { // ���������
        mTimeCounter = 0;
        mState = InGarageDriver::BEFORELINETRACE;
     }

	// �y�C���R�[�X�E�K���[�W�C���̂��߂Ɂz
	// �C���R�[�X���O�̏o�����肩�瑖�s���A���������m�����i�K�ŋ����𑪂�B
	// �C���R�[�X�E�K���[�W�̐��ʂ܂ňړ������āA�p�x�g���[�X�ō�90�x��]�EFORWARD��0�B
	// ���̌�A�p�x�g���[�X�ŃK���[�W��IN
	
	
    // �܂���Ԓ������C���g���[�X(�C���R�[�X���O�J�[�u�̈�ԏo�������Ă��鏊����)
    if (mState == InGarageDriver::BEFORELINETRACE) {
        mLineTrace.execute();	
    	if (mStraightDetector.detect() == true) {
			mState = InGarageDriver::STRAIGHTLINETRACE;
	    	mSlowdownSkill.setSkill(&mLineTrace);
    	    // �������m���Ă��瑖�鋗���B�������蓖���莟��H
    		mSlowdownSkill.setTargetDistance(3450 - 135);
    		mSlowdownSkill.setMinimumForward(0);
		}
    }
    else if (mState == InGarageDriver::STRAIGHTLINETRACE) {
		mSlowdownSkill.execute();
		if (mSlowdownSkill.isArrived()) {
    		mState = InGarageDriver::PREPAREENTERGARAGE;    		
			mAngleTrace.setTargetAngle(270 -2);
			mAngleTrace.setForward(0);
    	}
    }
	else if (mState == InGarageDriver::PREPAREENTERGARAGE) {
		mAngleTrace.execute();
		if (mAngleTrace.isArrived()) {
			mState = InGarageDriver::ENTERGARAGE;
			mAngleTrace.setTargetAngle(270 - 2);
		    // FORWARD�l�B��������ɒ�������
			mAngleTrace.setForward(20);
			mSlowdownSkill.setSkill(&mAngleTrace);
			mSlowdownSkill.setTargetDistance(100 - 50);
			mSlowdownSkill.setMinimumForward(0);
		}
	}
	else if (mState == InGarageDriver::ENTERGARAGE) {		
		mSlowdownSkill.execute();
	}
    mTimeCounter++;
    return false; // �I�����Ȃ�(�Ō�Ȃ̂łȂ�ł��ǂ�)
}
