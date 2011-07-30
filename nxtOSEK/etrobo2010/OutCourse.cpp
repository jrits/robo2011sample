//
// OutCourse.cpp
//
#include "OutCourse.h"
#include "factory.h"

/**
 * �R���X�g���N�^
 *
 * @param[in] aSection �ǂ̃Z�N�V��������n�߂邩�B�f�t�H���g�̓X�^�[�g�n�_����B
 */
OutCourse::OutCourse(OutCourse::eSection aSection)
    : Course()
{
    mState = aSection;
    switch (mState) {
    case OutCourse::SEESAW:
        // �V�[�\�[����e�X�g
        // �C���R�[�X�K���[�W�E�C����O��
        mGps.adjustXCoordinate(GPS_SEESAW_START_X);
        mGps.adjustYCoordinate(GPS_SEESAW_START_Y);
        mGps.adjustDirection(GPS_SEESAW_START_DIRECTION);
        break;
    case OutCourse::STAIRWAY:
        // �~�X�e���[�T�[�N������e�X�g
        // �C���R�[�X�A�A�E�g�R�[�X�V�[�\�[��}�[�J�[�I���n�_
        mGps.adjustXCoordinate(GPS_STAIRWAY_START_X);
        mGps.adjustYCoordinate(GPS_STAIRWAY_START_Y);
        mGps.adjustDirection(GPS_STAIRWAY_START_DIRECTION);
        break;
    case OutCourse::GARAGEIN:
        // �K���[�W�C������e�X�g
        // �C���R�[�X�⓹�O�}�[�J�[�I���n�_
        mGps.adjustXCoordinate(GPS_GARAGEIN_START_X);
        mGps.adjustYCoordinate(GPS_GARAGEIN_START_Y);
        mGps.adjustDirection(GPS_GARAGEIN_START_DIRECTION);
        break;
    case OutCourse::START:
    default:
        // �X�^�[�g�ʒu����e�X�g
        mGps.adjustXCoordinate(GPS_COURSE_START_X);
        mGps.adjustYCoordinate(GPS_COURSE_START_Y);
        mGps.adjustDirection(GPS_COURSE_START_DIRECTION);
        break;
    }
}

/**
 * �K�؂ȃh���C�o��I�����A�^�]������
 */
void OutCourse::drive()
{
#if 1 // ���O���M(0�F�����A1�F���{)
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
	LOGGER_DATAS08[1] = (S8)(mLineDetector.detect()); // ��u�����Ȃ̂Ń��O�Ɏc��Ȃ��\������
	LOGGER_DATAU16    = (U16)(mWallDetector.detect());
	LOGGER_DATAS16[0] = (S16)(mGps.getXCoordinate());
	LOGGER_DATAS16[1] = (S16)(mGps.getYCoordinate());
	LOGGER_DATAS16[2] = (S16)(mGps.getDirection());
	LOGGER_DATAS16[3] = (S16)(mGps.getDistance());
	LOGGER_DATAS32[0] = (S32)(mLeftMotor.getCount());
	LOGGER_DATAS32[1] = (S32)(mRightMotor.getCount());
	LOGGER_DATAS32[2] = (S32)(mLightSensor.get());
	LOGGER_DATAS32[3] = (S32)(mGyroSensor.get());
#endif
#if 0 // �f�o�b�O(0�F�����A1�F���{)
    {
    	//DESK_DEBUG = true;
        static int count = 0;
        if (count++ % 25 == 0) {
            Lcd lcd;
            lcd.clear();
            lcd.putf("sn", "OutCourse");
            lcd.putf("dn", mState);
            lcd.putf("dn", (S32)mGps.getXCoordinate());
            lcd.putf("dn", (S32)mGps.getYCoordinate());
            lcd.putf("dn", (S32)mGps.getDirection());
            lcd.putf("dn", (S32)mGps.getDistance());
            lcd.disp();
        }
    }
#endif
    if (mState == OutCourse::START) { // �X�^�[�g��ʏ���
        if (mNormalDriver.drive()) {
            float X = mGps.getXCoordinate();
            float Y = mGps.getYCoordinate();
            if (inRegion(GPS_SEESAW_START, MakePoint(X, Y))) { // ��Ԃ��V�[�\�[��ԂɍX�V
                mState = OutCourse::SEESAW;
            }
        }
    }
    else if (mState == OutCourse::SEESAW) { // �V�[�\�[���
        if (mSeesawDriver.drive()) {
            float X = mGps.getXCoordinate();
            float Y = mGps.getYCoordinate();
            if (inRegion(GPS_STAIRWAY_START, MakePoint(X, Y))) { // ��Ԃ��K�i��ԂɍX�V
                mState = OutCourse::STAIRWAY;
            }
        }
    }
    else if (mState == OutCourse::STAIRWAY) { // �K�i���
        if (mStairwayDriver.drive()) {
            float X = mGps.getXCoordinate();
            float Y = mGps.getYCoordinate();
            if (inRegion(GPS_GARAGEIN_START, MakePoint(X, Y))) { // ��Ԃ��K���[�W��ԂɍX�V
                mState = OutCourse::GARAGEIN;
            }
        }
    }
    else if (mState == OutCourse::GARAGEIN) { // �K���[�W�E�C�����
        mOutGarageDriver.drive();
    }
    // �e�X�g�h���C�o�N��
    else {
        mTestDriver.drive();
    }
}
