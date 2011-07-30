//
// OutGarageDriver.cpp
//

#include "OutGarageDriver.h"
#include "factory.h"
#include "constants.h"

/**
 * �R���X�g���N�^
 */
OutGarageDriver::OutGarageDriver()
{
    mState = OutGarageDriver::INIT;
    //K_THETADOT����邭����i�⓹�œ]�|�������߁j
    //K_THETADOT = 7.0F;
    mGrayThroughFlag = false;
}

/**
 * �A�E�g�R�[�X�K���[�W�E�C����Ԃ��U������
 *
 * �⓹�������C���g���[�X����A�K���[�W�E�C���܂ł�S������B
 *
 * @retval true �ŏI��ԁB����ȍ~�̏�ԑJ�ڂȂ��B
 * @retval false �܂���ڂ��������Ă��܂���B
 */
bool OutGarageDriver::drive()
{
#if 0 // ���O���M(0�F�����A1�F���{)
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
	LOGGER_DATAS32[0] = (S32)(mGps.getXCoordinate());
	LOGGER_DATAS32[1] = (S32)(mGps.getYCoordinate());
	LOGGER_DATAS32[2] = (S32)(mGps.getDirection());
//	LOGGER_DATAS32[3] = (S32)(mGps.getDistance());

#endif
#if 1 // �f�o�b�O(0�F�����A1�F���{)
    //DESK_DEBUG = true; // ���[�^���񂳂Ȃ��f�o�b�O
    static int count = 0; // static�͊�{�֎~�����A�f�o�b�O�Ȃ̂�
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "OutGarageDriver");
        lcd.putf("dn", mState);
        lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        lcd.putf("dn", (S32)(mGps.getDirection()));
        lcd.putf("dn", (S32)(mGps.getDistance()));
        lcd.disp();
    }
#endif
    if (mState == OutGarageDriver::INIT) { // ���������
        mTimeCounter = 0;
        K_THETADOT = 7.0F;
    	mState = OutGarageDriver::BEFORELINETRACE;
        mLineTrace.setForward(100);
        mAngleTrace.setForward(100);
        mGrayThroughFlag = false;
     }
    // ���C���g���[�X(�K�i�㒼���A�΂̒[����)
    if (mState == OutGarageDriver::BEFORELINETRACE) {
        if(! mGrayThroughFlag && mGps.getXCoordinate() < 4323) { // �p�x�g���[�X����
            mGrayThroughFlag = true;
            mAngleTrace.setTargetAngle(mGps.getDirection());
        }
        if (mGrayThroughFlag && mGps.getXCoordinate() < 4014) { // �p�x�g���[�X����
            mGrayThroughFlag = false;
        }
        if (mGrayThroughFlag) { // �p�x�g���[�X���{
            mAngleTrace.execute();
        }
        else {  // ���C���g���[�X���{
            mLineTrace.execute();
        }
        // �������m���ǂ��Ŕ������邩�킩��Ȃ��̂��܂����H�H�H
        // �����Ɏ��ԂŔ��f�B(11/30)
		if (mTimeCounter > 250 && mStraightDetector.detect() == true) {
			mState = OutGarageDriver::STRAIGHTLINETRACE;
	    	mSlowdownSkill.setSkill(&mLineTrace);
		    // �y���C���g���[�X�i�⓹��O�`�S�[���n�_�j�̑��s�����F������ς���I�I�z
    		mSlowdownSkill.setTargetDistance(4300);
    		mSlowdownSkill.setMinimumForward(100);
		}
        // if (mGps.getXCoordinate() < ?????) {
        //     mState = OutGarageDriver::GARAGEIN;
        //     mTimeCounter = 0;
        // }
    }
	
// ���W�w�葖�s�p�\�[�X��
/*
	else if(mState == OutGarageDriver::STRAIGHTLINETRACE) {
		mSlowdownSkill.execute();
		if (mSlowdownSkill.isArrived()) {

		    mState = OutGarageDriver::ENTERGARAGE;
			//�ڕW���W�̐ݒ�
		    //K_THETA�������Ƃ���
	        //K_THETADOT = 5.0F;
			mCoordinateTrace.setTargetCoordinate(MakePoint((GPS_GARAGE_X + 50), (GPS_GARAGE_Y + 50) ));
			mCoordinateTrace.setForward(100);
			mCoordinateTrace.setAllowableError(50); //5cm    
		}
	}
*/
// ���W�w�葖�s�p�\�[�X�� 

	
// �p�x�g���[�X�Ń\�[�X��	
	// ���C���g���[�X��̌�����]
    else if (mState == OutGarageDriver::STRAIGHTLINETRACE) {
        if(! mGrayThroughFlag && 4014 < mGps.getXCoordinate() && mGps.getXCoordinate() < 4323) { // �p�x�g���[�X����
            mGrayThroughFlag = true;
            mAngleTrace.setTargetAngle(mGps.getDirection());
        }
        if (mGrayThroughFlag && mGps.getXCoordinate() < 4014) { // �p�x�g���[�X����
            mGrayThroughFlag = false;
        }
        if (mGrayThroughFlag) { // �p�x�g���[�X���{
            mAngleTrace.execute();
        }
        else { // �X���[�_�E���X�L�����{
            mSlowdownSkill.execute();
        }
		
		if (mSlowdownSkill.isArrived()) {
			mState = OutGarageDriver::PREPAREENTERGARAGE;
			//mAngleTrace.setTargetAngle(90);
			//mAngleTrace.setForward(0);
		    //SORA�y�K���[�W�Ɍ������p�x�F������ς��饥��K�v�͂�������Ȃ�������z
		    mAngleTrace.setTargetAngle(175 - 7);
			mAngleTrace.setForward(100);
   			mSlowdownSkill.setSkill(&mAngleTrace);
		    //SORA�y�K���[�W�Ɍ������Ƃ��̋����F������ς���I�I�z
			mSlowdownSkill.setTargetDistance(1200 + 200);
			mSlowdownSkill.setMinimumForward(0);

    	}
    }
	
	//�������ړ]��A�ԌɂƓ���Y���W�֑O�i
	else if (mState == OutGarageDriver::PREPAREENTERGARAGE) {   //�K���[�W�C������
		//mAngleTrace.execute();
		//if (mAngleTrace.isArrived()) {
		//	mState = OutGarageDriver::STRAIGHTANGLETRACE;
		//	mAngleTrace.setForward(50);
		//	mSlowdownSkill.setSkill(&mAngleTrace);
		//	mSlowdownSkill.setTargetDistance(5);
		//	mSlowdownSkill.setMinimumForward(0);
		//}
		    
	    mSlowdownSkill.execute();
		    if(mSlowdownSkill.isArrived()) {
		        mState = OutGarageDriver::FIN_GARAGEIN;
		        mAngleTrace.setTargetAngle(180);
	    		mAngleTrace.setForward(0);        
		    }
	    		
	}
/* pass
    // �������ԌɂɌ�����
	else if (mState == OutGarageDriver::STRAIGHTANGLETRACE) {
		mSlowdownSkill.execute();
		if (mSlowdownSkill.isArrived()) {
			mState = OutGarageDriver::GOGARAGE;
			mAngleTrace.setTargetAngle(180);
			mAngleTrace.setForward(0);
		}
	}
	// �Ԍɓ��ꓮ��
	else if (mState == OutGarageDriver::GOGARAGE) {
		mAngleTrace.execute();
		if (mAngleTrace.isArrived()) {
			mState = OutGarageDriver::ENTERGARAGE;
			mAngleTrace.setForward(50);
			mSlowdownSkill.setSkill(&mAngleTrace);
			mSlowdownSkill.setTargetDistance(500);
			mSlowdownSkill.setMinimumForward(0);
		}
	}
    
pass */
    // �Ԍɓ��ꊮ��
    else if (mState == OutGarageDriver::FIN_GARAGEIN) {
        mAngleTrace.execute();
    
    }
    mTimeCounter++;
    return false; // �I�����Ȃ�(�Ō�Ȃ̂łȂ�ł��ǂ�)
}
    
    
// �p�x�g���[�X�Ŏ����I����    

    
// ���W�w�葖�s�� 
/*    

    //�Ԍɓ��ꊮ��
	else if (mState == OutGarageDriver::ENTERGARAGE) {
		mCoordinateTrace.execute();
	    if(mCoordinateTrace.isArrived()) {
	        mState = OutGarageDriver::FIN_GARAGEIN;
	        
			mAngleTrace.setForward(0);
		    mAngleTrace.setTargetAngle(180);
		}
	}
	
    else if (mState == OutGarageDriver::FIN_GARAGEIN) {
        mAngleTrace.execute();
    
    }
	//else if (mState == OutGarageDriver::GARAGEIN) { 
    //}
	
    mTimeCounter++;
    return false; // �I�����Ȃ�(�Ō�Ȃ̂łȂ�ł��ǂ�)
}

// ���W�w�葖�s ��
*/

