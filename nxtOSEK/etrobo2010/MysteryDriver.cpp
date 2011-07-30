//
// MysteryDriver.cpp
//

#include "MysteryDriver.h"
#include "factory.h"
#include "constants.h"

//�~�X�e���[�T�[�N���̃��[�g(4�p�^�[��)
#include "MysteryLineLL.h"
#include "MysteryLineLR.h"
#include "MysteryLineRL.h"
#include "MysteryLineRR.h"

/**
 * �R���X�g���N�^
 */
MysteryDriver::MysteryDriver()
{
    mState = MysteryDriver::INIT;
    mInitState = true;
}

/**
 * �~�X�e���[�T�[�N����Ԃ��U������
 *
 * �T�[�N���O�������C���g���[�X����A���C�����A�ド�C���g���[�X�܂ł�S������B
 *
 * @retval true �ŏI��ԁB����ȍ~�̏�ԑJ�ڂȂ��B
 * @retval false �܂���ڂ��������Ă��܂���B
 */
bool MysteryDriver::drive()
{
#if 0 // ���O���M�����i0�F�����A1�F���{�j
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
	LOGGER_DATAS32[0] = (S32)(mGps.getXCoordinate());
	LOGGER_DATAS32[1] = (S32)(mGps.getYCoordinate());
	LOGGER_DATAS32[2] = (S32)(mGps.getDirection());
	LOGGER_DATAS32[3] = (S32)(mGps.getDistance());
#endif
    
#if 1 // �f�o�b�O����
    // DESK_DEBUG = true; // ���[�^���񂳂Ȃ���
    static int count = 0; // static�͊�{�֎~�����A�f�o�b�O������g�p
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "MysteryDriver");
        lcd.putf("dn", mState);
        lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        lcd.putf("dn", (S32)(mGps.getDirection()));
        lcd.putf("dn", (S32)(mGps.getDistance()));
        lcd.disp();
    }
#endif
    
    if (mState == MysteryDriver::INIT) { // ���������
        mRoute       = mEnigmaDriver.decode();  // �G�j�O�}�h���C�o����A���[�g��񂪑����Ă���B
        mInitState   = true;
        
        mState = MysteryDriver::BEFORELINETRACE;
    }
    // ��O�������C���g���[�X(�C���R�[�X�A�A�E�g�R�[�X�V�[�\�[��}�[�J�[�I���n�_)
    if (mState == MysteryDriver::BEFORELINETRACE) {
        if (mInitState) {
            mInitState = false;
        }
        mLineTrace.execute();
        if (mGps.getXCoordinate() > 3200) {
            mState = MysteryDriver::MYSTERYROUTE;
            mInitState = true;
        }
    }
    if (mState == MysteryDriver::MYSTERYROUTE) {    // �c�����s���s
        if (mInitState) {
            mInitState = false;

        switch (mRoute) {//���[�g����
            case 0: //�E-�E�R�[�X
                mVirtualLineTrace.setControlPoints(MYSTERY_LINE_RR, NUM_MYSTERY_LINE_RR);
                break;
            case 1: //�E-���R�[�X
                mVirtualLineTrace.setControlPoints(MYSTERY_LINE_RL, NUM_MYSTERY_LINE_RL);
                break;
            case 2: //��-�E�R�[�X
                mVirtualLineTrace.setControlPoints(MYSTERY_LINE_LR, NUM_MYSTERY_LINE_LR);
                break;
            case 3: //��-���R�[�X
                mVirtualLineTrace.setControlPoints(MYSTERY_LINE_LL, NUM_MYSTERY_LINE_LL);
                break;
            }
            mVirtualLineTrace.reset();
        }
        mVirtualLineTrace.execute();
        // �i���Ӂj���C���Ɍ������Ď΂߂ɓːi����Ƃ���܂Ŋ܂߂Ă������ƁB
        if (mVirtualLineTrace.isLast()) {   // ���C�����A�ł��邩�ǂ����̔��f�B
            if (mLineDetector.detect() != LineDetector::ON_WHITE) {
                mState = MysteryDriver::AFTERLINETRACE;
                mInitState = true;
            }
        }
    }
    else if (mState == MysteryDriver::AFTERLINETRACE) { //�c�����s���s�I����
        if (mInitState) {
            mInitState = false;
            mLineTrace.reset(); // INIT_FORWARD����
        }
        mLineTrace.execute();   // ���C���g���[�X���s���s
    }
    
    return mState == MysteryDriver::AFTERLINETRACE; // �I�����܂���
}
