//
// InCourse.cpp
//
#include "InCourse.h"
#include "factory.h"
extern int gSonarDistance;

/**
 * �R���X�g���N�^
 *
 * @param[in] aSection �ǂ̃Z�N�V��������n�߂邩�B�f�t�H���g�̓X�^�[�g�n�_����B
 */
InCourse::InCourse(InCourse::eSection aSection)
    : Course()
{
    mState = aSection;
    switch (mState) {
    case InCourse::ENIGMA:
        // �G�j�O�}����e�X�g
        // �C���R�[�X�K���[�W�E�C����O��
        mGps.adjustXCoordinate(GPS_ENIGMA_START_X);
        mGps.adjustYCoordinate(GPS_ENIGMA_START_Y);
        mGps.adjustDirection(GPS_ENIGMA_START_DIRECTION);
        break;
    case InCourse::MYSTERY:
        // �~�X�e���[�T�[�N������e�X�g
        // �C���R�[�X�A�A�E�g�R�[�X�V�[�\�[��}�[�J�[�I���n�_
        mGps.adjustXCoordinate(GPS_MYSTERY_START_X);
        mGps.adjustYCoordinate(GPS_MYSTERY_START_Y);
        mGps.adjustDirection(GPS_MYSTERY_START_DIRECTION);
        break;
    case InCourse::GARAGEIN:
        // �K���[�W�C������e�X�g
        // �C���R�[�X���O�J�[�u�̈�ԏo�������Ă���Ƃ���
        mGps.adjustXCoordinate(GPS_GARAGEIN_START_X);
        mGps.adjustYCoordinate(GPS_GARAGEIN_START_Y);
        mGps.adjustDirection(GPS_GARAGEIN_START_DIRECTION);
        break;
    case InCourse::START:
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
void InCourse::drive()
{
#if 1 // ���O���M(0�F�����A1�F���{)
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
	LOGGER_DATAS08[1] = (S8)(mEnigmaDriver.detect()); // ��u�����Ȃ̂Ń��O�Ɏc��Ȃ��\������
	LOGGER_DATAU16    = (U16)(mStraightDetector.detect());
	LOGGER_DATAS16[0] = (S16)(mGps.getXCoordinate());
	LOGGER_DATAS16[1] = (S16)(mGps.getYCoordinate());
	LOGGER_DATAS16[2] = (S16)(mGps.getDirection());
	LOGGER_DATAS16[3] = (S16)(mGps.getDistance());
	LOGGER_DATAS32[0] = (S32)(mLeftMotor.getCount());
	LOGGER_DATAS32[1] = (S32)(mRightMotor.getCount());
	LOGGER_DATAS32[2] = (S32)(mLightSensor.get());
	LOGGER_DATAS32[3] = (S32)(gSonarDistance);
#endif
    
#if 0 // �f�o�b�O(0�F�����A1�F���{)
    {
        static int count = 0;
        if (count++ % 25 == 0) {
            Lcd lcd;
            lcd.clear();
            lcd.putf("sn", "InCourse");
            lcd.putf("dn", mState);
            lcd.disp();
        }
    }
#endif
    
    if (mState == InCourse::START) { // �X�^�[�g��ʏ���
        if (mNormalDriver.drive()) {
            float X = mGps.getXCoordinate();
            float Y = mGps.getYCoordinate();
            //if (inRegion(GPS_ENIGMA_START, MakePoint(X, Y))) {
            //    mState = InCourse::ENIGMA;
            //}
            if (inRegion(GPS_GARAGEIN_START, MakePoint(X, Y))) {
                mState = InCourse::GARAGEIN;
            }
        }
    }
    else if (mState == InCourse::ENIGMA) { // �G�j�O�}���
        if (mEnigmaDriver.drive()) {
            float X = mGps.getXCoordinate();
            float Y = mGps.getYCoordinate();
            if (inRegion(GPS_MYSTERY_START, MakePoint(X, Y))) {
                mState = InCourse::MYSTERY;
            }
        }
    }
    else if (mState == InCourse::MYSTERY) { // �~�X�e���[�T�[�N�����
        if (mMysteryDriver.drive()) {
            float X = mGps.getXCoordinate();
            float Y = mGps.getYCoordinate();
            if (inRegion(GPS_GARAGEIN_START, MakePoint(X, Y))) {
                mState = InCourse::GARAGEIN;
            }
        }
    }
    else if (mState == InCourse::GARAGEIN) { // �K���[�W�E�C�����
        mInGarageDriver.drive();
    }
    // �e�X�g�h���C�o�N��
    else { 
        mTestDriver.drive();
    }     
}
