//
// AngleTrace.cpp
//
#include "AngleTrace.h"
#include "factory.h"

/**
 * �R���X�g���N�^
 */
AngleTrace::AngleTrace()
{
    mTargetAngle = 0;
}
	
/**
 * �ڕW�p�x��ݒ�
 *
 * @param[in] targetAngle �ڕW�p�x
 */
void AngleTrace::setTargetAngle(float targetAngle)
{
	mTargetAngle = targetAngle;
}

/**
 * �����]���̋��e�덷��ݒ�
 *
 * @param[in] allowableError ���e�덷
 */
void AngleTrace::setAllowableError(float allowableError)
{
	mAllowableError = allowableError;
}

/**
 * �ڕW�p�x�ƌ��݂̊p�x����A�p�x�g���[�X���s���̂ɓK�؂ȑ��s�x�N�g�����v�Z����
 *
 * @return ���s�x�N�g��
 */
VectorT<float> AngleTrace::calcCommand()
{
	gLineTrace = false;
	float direction = 0;
	direction = mGps.getDirection(); //���݂̊p�x

	float P = -(mTargetAngle - direction);
	
	//�ڕW�p�x�ƌ��݊p�x�̍���P�� -180 < P <= 180 �͈͓̔��ɍĐݒ�
    P = Gps::marge180(P);
	
	//Y��-�l�ŉE�A+�l�ō���ڎw�������[�^�̐ڑ��|�[�g�m�F
	float Y = mAnglePid.control(P); //Pid����ヂ�[�^�o�͕␳����
	//Y += OFFSET_Y; //�����{��OFFSET_Y��8�������͂�
	
	//Pid����l���傫������ꍇ�A�����ŏC������
	//�����󂾂�P(���݊p�x�ƖڕW�p�x�̍���)��2.5�x�ȏォ-2.5�x�ȉ��ł���if���ɓ��� 2010/09/27
	if(Y > 100) Y = 100;
	if(Y < -100) Y = -100;
	
    VectorT<float> command;
    command.mX = mForward;
    command.mY = Y;
	return command;
}

/**
 * �����]���������������ǂ����𔻒肷��
 *
 * @retval true ����
 * @retval false ������
 */
bool AngleTrace::isArrived()
{
    // �ڕW�p�x�ƌ��݊p�x�̍�
	float diffDirection = mTargetAngle - mGps.getDirection();
    // [-180, 180] �͈͓̔��ɍĐݒ�
    diffDirection = Gps::marge180(diffDirection); 

    // ��������B[-Error, Error] �͈͓̔��ɂ����ꍇ�͓����Ƃ݂Ȃ��B
    if (fabs(diffDirection) < mAllowableError) { 
        return true;
    }
    else {
        return false;
    }
}
