///////////////////////////////////////////////////////////
//  TestDriver.cpp
//  Implementation of the Class TestDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#include "TestDriver.h"
#include "factory.h"
#include "TestLine.h"

/**
 * �R���X�g���N�^
 */
TestDriver::TestDriver()
{
    mState = -1; // ���������
}

bool TestDriver::drive()
{
// SORA    
#if 0 // ���O���M
    LOGGER_SEND = 2;
	LOGGER_DATAS08[0] = (S8)(mLineDetector.detect());
	LOGGER_DATAS16[0] = (S16)(mGps.getXCoordinate());
	LOGGER_DATAS16[1] = (S16)(mGps.getYCoordinate());
	LOGGER_DATAS16[2] = (S16)(mGps.getDirection());
	LOGGER_DATAS16[3] = (S16)(mGps.getDistance());
	LOGGER_DATAS32[0] = (S32)(mLightHistory.calcDifference());
#endif
#if 1 // DEBUG
    //DESK_DEBUG = true; // ���[�^���񂳂Ȃ��f�o�O
    static int count = 0; // static�͌����֎~����������
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "TestDriver");
        lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        lcd.putf("dn", (S32)(mGps.getDirection()));
        lcd.putf("dn", (S32)(mGps.getDistance()));
        //lcd.putf("dn", (S32)(mLeftMotor.getCount()));
        //lcd.putf("dn", (S32)(mRightMotor.getCount()));
        lcd.putf("dn", (S32)(mLineDetector.detect()));
        lcd.putf("dn", (S32)(mLightHistory.calcDifference()));
        lcd.disp();
    }
#endif
    //// ���C���g���[�X�e�X�g
    // mLineTrace.setForward(50);
    // mLineTrace.execute();
    // mStraightDetector.detect(); // �������m�e�X�g

    //// �����g���[�X�e�X�g
    // if (mState == -1) {
    //     // �J�n����
    //     mGps.adjustDirection(180);
    //     // �ڕW����
    //     mAngleTrace.setTargetAngle(180);
    //     mAngleTrace.setForward(50);
    //     mState = 0;
    // }
    // mAngleTrace.execute();

    //// ���W�g���[�X�e�X�g
    // if (mState == -1) {
    //     // �J�n���W
    //     mGps.adjustXCoordinate(GPS_COURSE_START_X);
    //     mGps.adjustYCoordinate(GPS_COURSE_START_Y);
    //     mGps.adjustDirection(GPS_COURSE_START_DIRECTION);
    //     // �ڕW���W
    //     mCoordinateTrace.setTargetCoordinate(MakePoint(GPS_GARAGE_X, GPS_GARAGE_Y));
    //     mCoordinateTrace.setForward(50);
    //     mState = 0;
    // }
    // mCoordinateTrace.execute();

    //// ���z���C���g���[�X�e�X�g
    if (mState == -1) { // ���������
         mVirtualLineTrace.setControlPoints(TEST_LINE, NUM_TEST_LINE);
         mVirtualLineTrace.reset();
         mState = 0;
     }
     mVirtualLineTrace.execute();

    //// ���W�w�蒅�n�e�X�g
/*
    if (mState == -1) { // ���������
        // �J�n���W
        mGps.adjustXCoordinate(GPS_COURSE_START_X);
        mGps.adjustYCoordinate(GPS_COURSE_START_Y);
        mGps.adjustDirection(GPS_COURSE_START_DIRECTION);
        // �ڕW���W
        mCoordinateStop.setTargetCoordinate(MakePoint(GPS_GARAGE_X, GPS_GARAGE_Y));
//        mCoordinateStop.setTargetCoordinate(MakePoint(GPS_COURSE_START_X, GPS_COURSE_START_Y));
        mCoordinateStop.setForward(100);
//        mCoordinateStop.setSlowdownDistance(500);
        //SORA test
        //�}��~���s�������B�ڕW���W��250[mm]�ȓ��ɗ�������Ƌ}��~���s���B
        mCoordinateStop.setSlowdownDistance(210);
        //SORA test
        mState = 0;
    }
    mCoordinateStop.execute();
    */
    //// SlowdownSkill(mAngleTrace)�e�X�g�BmSlowdownPid�����p
    // if (mState == -1) { // ���������
    //     // �J�n���W
    //     mGps.adjustXCoordinate(0.0);
    //     mGps.adjustYCoordinate(0.0);
    //     mGps.adjustDirection(0.0);
    //     // �ڕW����
    //     mAngleTrace.setTargetAngle(0.0);
    //     mAngleTrace.setForward(50.0);
    //     // ��~�ڕW����
    //     mSlowdownSkill.setSkill(&mAngleTrace); // �����g���[�X���f�R���[�g
    //     mSlowdownSkill.setTargetDistance(600.0); // 60cm
    //     //mSlowdownSkill.setMinimumForward(0); // ��~
    //     //mSlowdownSkill.setMinimumForward(-1); // �o�b�N����
    //     mSlowdownSkill.setMinimumForward(-30); // �s�����肫���苖��
    //     mState = 0;
    // }
    // mSlowdownSkill.execute(); // SlowdownSkill�����s

    //// StopSkill(mAngleTrace)�e�X�g�B
    // if (mState == -1) { // ���������
    //     // �J�n���W
    //     K_THETADOT = 7.5F;
    //     mGps.adjustXCoordinate(0.0);
    //     mGps.adjustYCoordinate(0.0);
    //     mGps.adjustDirection(0.0);
    //     // �ڕW����
    //     mAngleTrace.setTargetAngle(0.0);
    //     mAngleTrace.setForward(100.0);
    //     // ��~�ڕW����
    //     mStopSkill.setSkill(&mAngleTrace); // �����g���[�X���f�R���[�g
    //     mStopSkill.setTargetDistance(600.0); // 60cm
    //  	mStopSkill.setAllowableError(100.0); // 50cm�n�_�Ńt�H���[�h�l0
    //     mState = 0;
    // }
    // mStopSkill.execute(); // StopSkill�����s

    return true;
}
