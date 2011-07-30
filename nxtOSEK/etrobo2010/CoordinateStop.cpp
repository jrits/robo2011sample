#include "CoordinateStop.h"
#include "factory.h"

/**
 * �R���X�g���N�^
 */
CoordinateStop::CoordinateStop() : 
    Skill()
{
    setTargetCoordinate(MakePoint(0, 0));
    setSlowdownDistance(500); // mm
    setForward(100);
}

/**
 * �ڕW�ʒu��ݒ肷��B
 *
 * @param[in] targetCoordinate �ڕW���W
 */
void CoordinateStop::setTargetCoordinate(Point targetCoordinate)
{
    mTargetCoordinate = targetCoordinate;
    // ���ݍ��W
    float currentX = mGps.getXCoordinate();
    float currentY = mGps.getYCoordinate();
    // �ڕW���W�ƌ��ݍ��W�Ƃ̋���
    float diffX = mTargetCoordinate.X - currentX;
    float diffY = mTargetCoordinate.Y - currentY;
    float diffDistance = sqrt(pow(diffX, 2) + pow(diffY, 2));
    // �ڕW�������Z�o
    mTargetDistance = mGps.getDistance() + diffDistance;
};

/**
 * �ڕW���W�̉�mm��O���璅�n������n�߂邩�ݒ肷��
 *
 * @param[in] slowdownDistance �ڕW���W�̉�mm��O���璅�n������n�߂邩
 */
void CoordinateStop::setSlowdownDistance(float slowdownDistance)
{
	mSlowdownDistance = slowdownDistance;
}

/**
 * �ڕW���������O�Őݒ肷��B
 *
 * setTargetCoordinate() ���ڕW����������Ɍv�Z���Ă���邪�A�������������ꍇ�͂�������g���B
 *
 * @param[in] targetDistance �ڕW����
 */
void CoordinateStop::setTargetDistance(float targetDistance)
{
    mTargetDistance = targetDistance;
};

/**
 * �ڕW�����ƌ��݋�������A���W�w�蒅�n���s���̂ɓK�؂ȑ��s�x�N�g�����v�Z����
 *
 * @return ���s�x�N�g��
 */
 
/* 
VectorT<float> CoordinateStop::calcCommand()
{
    // ���W�w�葖�s�̗��p�B�t�H���[�h�l���㏑������B
    VectorT<float> command = mCoordinateTrace.calcCommand();

    // ���݋���
    float distance = mGps.getDistance();

    // �������߂Â����珙�X�ɑ��x��������B�ʂ�߂�����o�b�N����B
    // @todo: �ʂ�߂��Ȃ��̂���ԗǂ��B
    if (mTargetDistance - distance < mSlowdownDistance) { // ���n����J�n
        float P = mTargetDistance - distance;
        float X = mStopPid.control(P);
        if(X > mForward) X = mForward;
        if(X < -mForward) X = -mForward;
        command.mX = X;

#if 0
        {
            Lcd lcd;
            lcd.clear();
            lcd.putf("sn", "CoordinateStop");
            lcd.putf("dn", (int)mTargetDistance);
            lcd.putf("dn", (int)distance);
            lcd.putf("dn", (int)command.mX);         
            lcd.disp();
        }
#endif
    }
    else { // �܂������ꍇ
        command.mX = mForward;
    }

	return command;
}

*/

// SORA
//calcCommand2 �ڕW���W�t�߂ŋ}��~�p�^�[��

VectorT<float> CoordinateStop::calcCommand()
{
    
    // ���W�w�葖�s�̗��p�B�t�H���[�h�l���㏑������B
    VectorT<float> command = mCoordinateTrace.calcCommand();

    // ���݋���
    float distance = mGps.getDistance();

    // �������߂Â�����}��~�BSlowdownDistance���ЂƂ܂��q�؂��Ă���B�K���ȕϐ����������ēK���Ȓl�ɕύX���ׂ��H
    if ((mTargetDistance - distance) < mSlowdownDistance) { // ���n����J�n
        
        command.mX = 0;
        //SORA test
        command.mY = 0;

#if 0
        {
            Lcd lcd;
            lcd.clear();
            lcd.putf("sn", "CoordinateStop");
            lcd.putf("dn", (int)mTargetDistance);
            lcd.putf("dn", (int)distance);
            lcd.putf("dn", (int)command.mX);         
            lcd.disp();
        }
#endif
    }
    else { // �܂������ꍇ
        //command.mX = mForward;
    }
    
//SORA [log]    
#if 2
    // ���O���M
    LOGGER_SEND = 2;
//	LOGGER_DATAS08[0] = (S8)(linePattern);
	LOGGER_DATAS16[0] = (S16)(mTargetDistance - distance);
	LOGGER_DATAS16[1] = (S16)(command.mX);
//	LOGGER_DATAS16[2] = (S16)(mTargetDistance - distance);
//	LOGGER_DATAS16[3] = (S16)(lightDiff);
#endif
    
    
        
	return command;
}

//  2010/11/25  ���s�F�}��~�͓]��
//SORA */
