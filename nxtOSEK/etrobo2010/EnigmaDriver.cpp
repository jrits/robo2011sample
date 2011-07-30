//
// EnigmaDriver.cpp
//

#include "EnigmaDriver.h"
#include "factory.h"
#include "constants.h"
#include "Speaker.h"

#define DETECT_THRESH (25)
#define MAX_DISTANCE (255)
#define DETECT_RANGE (20) // ���m�Ɏ኱�]�T����������
#define ENIGMA_SAMPLECOUNT 100 // *4ms
extern bool gDoSonar; //!< �\�i�[�Z���T�����t���O
extern bool gSonarIsDetected; //!< �՗����m�̌���

/**
 * �R���X�g���N�^
 */
EnigmaDriver::EnigmaDriver()
{
    mState = EnigmaDriver::INIT;
    mObstacleFlag[0] = false;
    mObstacleFlag[1] = false;
}

/**
 * �G�j�O�}�E�f�R�[�f�B���O��Ԃ��U������
 *
 * �X�^�[�g�������C���g���[�X����A���C�����A�ド�C���g���[�X�܂ł�S������B
 *
 * @retval true �ŏI��ԁB����ȍ~�̏�ԑJ�ڂȂ��B
 * @retval false �܂���ڂ��������Ă��܂���B
 */
bool EnigmaDriver::drive()
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
        lcd.putf("sn", "EnigmaDriver");
        lcd.putf("dn", mState);
        lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        lcd.putf("dn", (S32)(mGps.getDirection()));
        lcd.putf("dn", (S32)(mGps.getDistance()));
        lcd.disp();
    }
#endif
    if (mState == EnigmaDriver::INIT) { // ���������
    	gDoSonar = true;
        mTimeCounter = 0;
        mInitState = true;
        mState = EnigmaDriver::BEFORELINETRACE;
        // ����e�X�g�p�B������Ƃ������C���g���[�X���Ă���ENTERPET
        // mGps.adjustXCoordinate(430);
        // mGps.adjustYCoordinate(-1200);
        // mGps.adjustDirection(270);
        // ����e�X�g�p�BENTERPET ����
        // mState = EnigmaDriver::ENTERPET;
        // mGps.adjustXCoordinate(430);
        // mGps.adjustYCoordinate(-1500);
        // mGps.adjustDirection(360);
    }
    // �܂��X�^�[�g�������C���g���[�X(�C���R�[�X�K���[�W�E�C����O�����ӂ肩����Ɨǂ�)
    if (mState == EnigmaDriver::BEFORELINETRACE) {
        if (mInitState) {
            mTimeCounter = 0;
            mLineTrace.setForward(100);
            mSlowdownSkill.setSkill(&mLineTrace);
            //mSlowdownSkill.setAllowableError(10); // 1cm
            mInitState = false;
        }
        mSlowdownSkill.setTargetDistance(mGps.calcDistanceTo(MakePoint(460,-1515+200)));  // @todo: �▭��y���W���w��
        mSlowdownSkill.execute();
        // ���C����������Ȃ����߉i���ɒH����Ȃ��ꍇ������
        // if (mSlowdownSkill.isArrived()) { 
        if (mGps.getYCoordinate() < -1515+200) { // �𒼂ɍ��W�� // @todo: �▭��y���W���w��
            mState = EnigmaDriver::PREPAREENTERPET;
            mInitState = true;
        }
    }
    // �N�������Ƃ���0�x����������
    else if (mState == EnigmaDriver::PREPAREENTERPET) {
        if (mInitState) {
            mTimeCounter = 0;
            mAngleTrace.setForward(0);
            mAngleTrace.setTargetAngle(0);
            mAngleTrace.setAllowableError(2.0); // 2�x
            mInitState = false;
        }
        mAngleTrace.execute();
        if (mAngleTrace.isArrived()) {
            //�ڕW���W�ɓ��������玟�̏�Ԃ֑J��
            mState = EnigmaDriver::ENTERPET;
            mInitState = true;
        }
    }
    // �y�b�g�{�g���̊ԂɐN��
    else if (mState == EnigmaDriver::ENTERPET) {
        if (mInitState) {
            mTimeCounter = 0;
            mCoordinateTrace.setForward(30);
            mSlowdownSkill.setSkill(&mCoordinateTrace);
            // @todo: �▭��y���W���w��
            mCoordinateTrace.setTargetCoordinate(MakePoint(600, -1515+50)); 
            mSlowdownSkill.setTargetDistance(mGps.calcDistanceTo(MakePoint(600, -1515+50)));
            mSlowdownSkill.setAllowableError(10); // 1cm
            mInitState = false;
        }
        mSlowdownSkill.execute();
        if (mSlowdownSkill.isArrived()) {
            mState = EnigmaDriver::LEFTPET;
            mInitState = true;
        }
    }
    //1�{�ڔ���
    else if (mState == EnigmaDriver::LEFTPET) {
        if (mInitState) {
            gDoSonar = true; // �\�i�[�N��
            mOrigK_PHIDOT = K_PHIDOT; // ��Ŗ߂����߂ɕۑ�
            K_PHIDOT = 62.5F;

            mTimeCounter = 0;
            mAngleTrace.setForward(0);
            mAngleTrace.setTargetAngle(90);
            mAngleTrace.setAllowableError(2.0); // 2�x
            mInitState = false;
            mIsArrived = false;
            gDoSonar = false; // �\�i�[�N���O�͈�U���Z�b�g
        }
        mAngleTrace.execute();
        // �����]������
        if (! mIsArrived && mAngleTrace.isArrived()) {
            mIsArrived = true;
            mTimeCounter = 0;
        }
        // �����]���������Ă���՗����m�B
        if (mIsArrived) {
            gDoSonar = true; // �\�i�[�N��
        }
        // �����]�����Ă��玞�Ԍo�߂ŏI��
        if (mIsArrived && mTimeCounter >= ENIGMA_SAMPLECOUNT) {
            mState = EnigmaDriver::RIGHTPET;
            mInitState = true;
            mTimeCounter = 0;
            mObstacleFlag[0] =  gSonarIsDetected;
            gDoSonar = false; // �\�i�[��~      
            if (mObstacleFlag[0]) { Speaker s; s.playTone(1000, 1, 100); }
        }
    }
    //2�{�ڔ���
    else if(mState == EnigmaDriver::RIGHTPET) {
        if (mInitState) {
            mTimeCounter = 0;
            mAngleTrace.setForward(0);
            mAngleTrace.setTargetAngle(270);
            mAngleTrace.setAllowableError(2.0); // 2�x
            mInitState = false;
            mIsArrived = false;
        	gDoSonar = true; // �\�i�[�N��
        }
        mAngleTrace.execute();
        // �����]������
        if (! mIsArrived && mAngleTrace.isArrived()) {
            mIsArrived = true;
            mTimeCounter = 0;
        }
        // �����]���������Ă���՗����m�B
        if (mIsArrived) {
            gDoSonar = true; // �\�i�[�N��
        }
        // �����]�����Ă��玞�Ԍo�߂ŏI��
        if (mIsArrived && mTimeCounter >= ENIGMA_SAMPLECOUNT) {
            mState = EnigmaDriver::EXITPET;
            mInitState = true;
            mTimeCounter = 0;
            mObstacleFlag[1] =  gSonarIsDetected;
            gDoSonar = false; // �\�i�[��~      
            if (mObstacleFlag[1]) { Speaker s; s.playTone(1000, 1, 100); }
        }
    }
    // �E�o�̂���20�x������
    else if (mState == EnigmaDriver::EXITPET) { 
        if (mInitState) {
            mTimeCounter = 0;
            mAngleTrace.setForward(0);
            mAngleTrace.setTargetAngle(20);
            mAngleTrace.setAllowableError(2.0); // 2�x
            mInitState = false;
        }
        mAngleTrace.execute();
        if (mAngleTrace.isArrived()) {
        	mGps.adjustDirection(Gps::marge360(mGps.getDirection()));        	
            mState = EnigmaDriver::LINERETURN;
            K_PHIDOT = mOrigK_PHIDOT;
            mInitState = true;
        }
    }
    //���C���Ɍ������Ď΂߂ɓːi�B���C�����A�ł��邩�I�H
    else if (mState == EnigmaDriver::LINERETURN) { 
        if (mInitState) {
            mTimeCounter = 0;
            mCoordinateTrace.setTargetCoordinate(MakePoint(1500, -1100));
            mCoordinateTrace.setForward(30);
            mCoordinateTrace.setAllowableError(10); // 1cm
            mInitState = false;
        }
        mCoordinateTrace.execute();
        if (mLineDetector.detect() != LineDetector::ON_WHITE) { // ���C�����m
            mState = EnigmaDriver::AFTERLINETRACE;
            mInitState = true;
        }
        // else if (mCoordinateTrace.isArrived()) {
        //     // ���C�������o���Ă��Ȃ��̂ɒH����Ă��܂����ꍇ�B
        //     // �����������܂悤�Ƃ��H
        //     mState = EnigmaDriver::LINERETURN2;
        // }
    }
    else if (mState == EnigmaDriver::AFTERLINETRACE) {
        if (mInitState) {
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
    return mState == EnigmaDriver::AFTERLINETRACE; // �I�����܂���
}

/**
 * �՗������m����
 *
 * @retval true ���m����
 * @retval false ���m���Ȃ�����
 */
bool EnigmaDriver::detect()
{
    //return (10 < gSonarDistance) && (gSonarDistance < 45);
    return false;
}

/**
 * �G�j�O�}�E�f�R���f�B���O������
 *
 * MysteryDriver �����ɗ���B
 *
 * @return ���[�gID
 */
int EnigmaDriver::decode()
{
    if(mObstacleFlag[0] == false && mObstacleFlag[1] == false) {
        //�E�A�E
        return 0;
    }
    else if(mObstacleFlag[0] == false && mObstacleFlag[1] == true) {
        //�E�A��
        return 1;
    }
    else if(mObstacleFlag[0] == true && mObstacleFlag[1] == false) {
        //���A�E
        return 2;
    }
    else if(mObstacleFlag[0] == true && mObstacleFlag[1] == true) {
        //���A��
        return 3;
    }
    return -1;
}
