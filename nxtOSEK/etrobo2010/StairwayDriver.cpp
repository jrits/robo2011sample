///////////////////////////////////////////////////////////
//  StairwayDriver.cpp
//  Implementation of the Class StairwayDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: nseo
///////////////////////////////////////////////////////////

#include "StairwayDriver.h"
#include "factory.h"
#include "constants.h"
#include "Speaker.h"

/**
 * �R���X�g���N�^
 */
StairwayDriver::StairwayDriver()
{
    mState = StairwayDriver::INIT;
    mTimeCounter = 0;
    mInitState = true;
    mDoDetectWall = false;

}

/**
 * ���s����B�K�i���U������B
 *
 * @retval true �ŏI��ԁB����ȍ~�̏�ԑJ�ڂȂ��B
 * @retval false �܂���ڂ��������Ă��܂���B
 */
bool StairwayDriver::drive()
{
#if 0 // ���O���M
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
	LOGGER_DATAS32[0] = (S32)(mGps.getXCoordinate());
	LOGGER_DATAS32[1] = (S32)(mGps.getYCoordinate());
	LOGGER_DATAS32[2] = (S32)(mGps.getDirection());
	LOGGER_DATAS32[3] = (S32)(mGps.getDistance());
#endif
#if 1 // DEBUG
    //DESK_DEBUG = true; // ���[�^���񂳂Ȃ��f�o�O
    if (mTimeCounter % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "StairwayDriver");
        lcd.putf("dn", mState);
        lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        lcd.putf("dn", (S32)(mGps.getDirection()));
        lcd.putf("dn", (S32)(mGps.getDistance()));
        lcd.putf("dn", (S32)(mGyroHistory.calcDifference()));
        lcd.putf("dn", (S32)(K_THETADOT*10));
        lcd.disp();
    }
#endif

    if (mState == StairwayDriver::INIT) {
        mState = StairwayDriver::BEFORELINETRACE;
        mInitState = true;
    	mWallDetector.setThreshold(40);
    }
    if (mState == StairwayDriver::BEFORELINETRACE) {
        if (mInitState) {
            mPrevMotor = mLeftMotor.getCount();
            mDoDetectWall = false;
            mInitState = false;
        }
        if (! mDoDetectWall) {
            mLineTrace.setForward(100);
            K_THETADOT = 7.0F; // Find! �K�i�O�Ȑ������ꂢ�Ƀ��C���g���[�X�ł���▭�Ȓl
            if (mGps.getYCoordinate() > -2900) {
            	K_THETADOT = 11.0F;
            }
            if (mGps.getYCoordinate() > -2800) {
                mPrevDirection = mGps.getDirection();
                mLineTrace.setForward(100);
                mDoDetectWall = true;
            	mTimeCounter = 0;
            }
            mLineTrace.execute();
        }
        if (mDoDetectWall) {
            mAngleTrace.setForward(100);
            mAngleTrace.setTargetAngle(mPrevDirection);
            K_THETADOT = 11.0F; // Find! �P�i�ڂP��ōڂ�▭�Ȓl
            //if (mTimeCounter > 250 && mWallDetector.detect()) {
            if (mWallDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                //mActivator.reset(USER_GYRO_OFFSET + 10); // �傫�����ċ}���i
                mState = StairwayDriver::ON1STSTAGE;
                mInitState = true;
            }
            mAngleTrace.execute();    
        }
    }
    else if (mState == StairwayDriver::ON1STSTAGE) {
        // �i���P�ɍڂ�������
        if (mInitState) {
            mPrevMotor = mLeftMotor.getCount();
            mTimeCounter = 0;
            mDoDetectWall = false;
            mInitState = false;
        }
        if (! mDoDetectWall) {
            mActivator.reset(USER_GYRO_OFFSET - 15); // ���������ċ}�u���[�L
            K_THETADOT = 6.0F; // Find! �Q�i�ڂP��ōڂ�▭�Ȓl
            if (mTimeCounter > 25 && mLeftMotorHistory.calcDifference() < 0) {
                mActivator.reset(USER_GYRO_OFFSET);
                mTimeCounter = 0;
                mDoDetectWall = true;
            }
            mAngleTrace.execute();
        }
        if (mDoDetectWall) {
            if (mLeftMotor.getCount() - mPrevMotor > 270 && 
                mWallDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                //mActivator.reset(USER_GYRO_OFFSET + 20); // �傫�����ċ}���i
                //mActivator.reset(USER_GYRO_OFFSET); // ���łɐ��������Ă���
                mState = StairwayDriver::ON2NDSTAGE;
                mInitState = true;
            }
            mAngleTrace.execute();
        }
        if (mLeftMotor.getCount() < mPrevMotor) { // �߂��������
            mState = StairwayDriver::BEFORELINETRACE;
            mInitState = true;
            mDoDetectWall = true;
        }
    }
    else if (mState == StairwayDriver::ON2NDSTAGE) {
        // �i���Q�ɍڂ�������
        if (mInitState) {
            mPrevMotor = mLeftMotor.getCount();
            mTimeCounter = 0;
            mDoDetectWall = false;
            mInitState = false;
        }
        if (! mDoDetectWall) {
            mActivator.reset(USER_GYRO_OFFSET - 15); // ���������ċ}�u���[�L
            if (mTimeCounter > 25 && mLeftMotorHistory.calcDifference() < 0) {
                //mActivator.reset(USER_GYRO_OFFSET); // ����őO�X�ɂȂ��Ă��܂�
                mActivator.reset(USER_GYRO_OFFSET - 5); // ����ł������i��
                mAngleTrace.setForward(0); // USER_GYRO_OFFSET - 5 �� 0 �ł������i��
                mLineTrace.setForward(0); // USER_GYRO_OFFSET - 5 �� 0 �ł������i��
                K_THETADOT = 5.0F;
                mTimeCounter = 0;
                mDoDetectWall = true;
            }
            mAngleTrace.execute();
        }
        // �h�X�����m
        if (mDoDetectWall) {
            mAngleTrace.execute();
            if (mLeftMotor.getCount() - mPrevMotor > 360 && 
                mWallDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                mState = StairwayDriver::DROPDOWN;
                mInitState = true;
            }
        }
        if (mLeftMotor.getCount() < mPrevMotor) { // �߂��������
            mState = StairwayDriver::BEFORELINETRACE;
            mInitState = true;
            mDoDetectWall = true;
        }
    }
    else if (mState == StairwayDriver::DROPDOWN) {
        // �h�X������
        if (mInitState) {
            //mActivator.reset(USER_GYRO_OFFSET - 50); // ���������ċ}�u���[�L
            mInitState = false;
            mTimeCounter = 0;
            mDoDetectWall = false;
            mInitState = false;
        }
        if (! mDoDetectWall) {
            mAngleTrace.execute();
            if (mTimeCounter > 250) {
                //&& mLeftMotorHistory.calcDifference() < 0) {
                //mActivator.reset(USER_GYRO_OFFSET);
                mTimeCounter = 0;
                mDoDetectWall = true;
            }
        }
        // ���C�����m
        if (mDoDetectWall) {
            //VectorT<float> command(30, 0); // 0 �ō��ɋȂ���
            //mActivator.run(command);
            mAngleTrace.setTargetAngle(mPrevDirection + 10);
            K_THETADOT = 9.5F;
            mAngleTrace.setForward(30);
            mAngleTrace.execute();
            if (mLineDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                mState = StairwayDriver::AFTERLINETRACE;
                mInitState = true;
            }
        }
    }
    else if (mState == StairwayDriver::AFTERLINETRACE) {
        if (mInitState) {
            K_THETADOT = 9.5F;
            mLineTrace.setForward(30);
            mTimeCounter = 0;
            mInitState = false;
        }
        mLineTrace.execute();
        if (mTimeCounter > 250) {
            mLineTrace.setForward(60);
        }
        if (mTimeCounter > 500) {
            mLineTrace.setForward(100);
        }
    }
    mTimeCounter++;
	return mState == StairwayDriver::AFTERLINETRACE; // �I�����܂���
}
