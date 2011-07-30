///////////////////////////////////////////////////////////
//  SeesawDriver.cpp
//  Implementation of the Class SeesawDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: masui
///////////////////////////////////////////////////////////

#include "SeesawDriver.h"
#include "factory.h"
#include "constants.h"

#define RETURNTIMING 375 //�l��Ԃ��^�C�~���O
#define LIGHTBUF 10 //���Z���T�q�X�g���̃o�b�t�@�T�C�Y
#define LATEST 0

/**
 * �R���X�g���N�^
 */
SeesawDriver::SeesawDriver()    //�����l�̐ݒ�
{
	mTimeCounter = 0;
	mInitState = false;             
	mState = StairwayDriver::INIT;  //�X�e�[�g
}

/**
 * �f�X�g���N�^
 */
SeesawDriver::~SeesawDriver(){}

/**
 * ���s����B�V�[�\�[���U������B
 *
 * @retval true �ŏI��ԁB����ȍ~�̏�ԑJ�ڂȂ��B
 * @retval false �܂���ڂ��������Ă��܂���B
 */
 

bool SeesawDriver::drive()
{	
//	float gyro = mGyroHistory.calcDifference();
#if 0
    LOGGER_SEND = 2;
	LOGGER_DATAS08[0] = (S8)(mState);
//	LOGGER_DATAS16[0] = (S16)();
//	LOGGER_DATAS16[1] = (S16)(mGps.getYCoordinate());
//	LOGGER_DATAS16[2] = (S16)(mGps.getDirection());
//	LOGGER_DATAS16[3] = (S16)(mGps.getDistance());
	LOGGER_DATAS32[0] = (S32)(mGyroHistory.calcDifference());
#endif
	float xCo = mGps.getXCoordinate();
//	float yCo = mGps.getYCoordinate();

//	�i���U��
	if (mState == SeesawDriver::INIT) {
        mState = SeesawDriver::BEFORELINETRACE;
        mInitState = true;
		mWallDetector.setThreshold(80);
    }
	if(mState == SeesawDriver::BEFORELINETRACE){
    	if(mInitState){
			K_THETADOT = 10.5F;
			mAngleTrace.setTargetAngle(354.0);
			mAngleTrace.setForward(100);
			mLineTrace.setForward(100);
    		mInitState = false;
    	}
		if(1200 < xCo && xCo < 1500)
		{
			mAngleTrace.execute();
		}else if(xCo <= 1200){
			mLineTrace.execute();
		}
		else if(xCo >= 1500){
	    	if(mWallDetector.detect()){
				mStartAngleCounter = mLeftMotorHistory.get();
    			mState = SeesawDriver::ONTHESEESAW;
    			mInitState = true;
	    	}
			mLineTrace.execute();
		}
	}else if(mState == SeesawDriver::ONTHESEESAW){
		if(mInitState){
			K_THETADOT = 5.0F;
			mAngleTrace.setTargetAngle(350.0);
			mAngleTrace.setForward(25);
			mInitState = false;
		}
		if (fabs(mLeftMotorHistory.calcDifference()) < 5 && (mLeftMotorHistory.get() - mStartAngleCounter) > 360) {
            mState = SeesawDriver::LINERETURN;
            mInitState = true;
		}
		mAngleTrace.execute();
	}else if (mState == SeesawDriver::LINERETURN) {
        if (mInitState) {
            mInitState = false;
            K_THETADOT = 7.5F;
        }
        if (mLineDetector.detect()) {
            mState = SeesawDriver::AFTERLINETRACE;
            mInitState = true;
        }
        VectorT<F32> command(15, -5);
        mActivator.run(command);
    }else if (mState == SeesawDriver::AFTERLINETRACE) {
        if (mInitState) {
            K_THETADOT = 6.5F;
            mLineTrace.setForward(20);
//        	mLineTrace.reset();
            mInitState = false;
        }
    	if(mTimeCounter > 500){
            K_THETADOT = 7.5F;
            mLineTrace.setForward(100);
    	}
        mLineTrace.execute();
    	mTimeCounter++;
    }
	return mState == SeesawDriver::AFTERLINETRACE; // �I�����܂���
}


