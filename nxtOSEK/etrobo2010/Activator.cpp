//
// Activator.cpp
//
#include "Activator.h"

/**
 * �R���X�g���N�^
 *
 * @param[in] leftMotor �����[�^
 * @param[in] rightMotor �E���[�^
 * @param[in] gyroSensor �W���C���Z���T
 * @param[in] nxt NXT�I�u�W�F�N�g
 */
Activator::Activator(Motor &leftMotor, 
                     Motor &rightMotor, 
                     GyroSensor &gyroSensor, 
                     Nxt &nxt) :
    mLeftMotor(leftMotor), 
    mRightMotor(rightMotor), 
    mGyroSensor(gyroSensor), 
    mNxt(nxt)
{
	mGyroOffset = USER_GYRO_OFFSET; //�I�t�Z�b�g�l��������
}

/**
 * �p�����[�^�̏�����
 *
 * @param[in] gyroOffset �W���C���I�t�Z�b�g
 */
void Activator::reset(int gyroOffset)
{
    mGyroOffset = gyroOffset;
}
/**
 * �n���h���A�A�N�Z���̑���B
 *
 * �o�����X����͐���@��(Activator)�������I�ɍs���Ă����B
 *
 * @param[in] command ���s�x�N�g��
 */
void Activator::run(VectorT<F32> command)
{
	S8 pwmL, pwmR;
	
	balance_control(
        (F32)command.mX, // �O��i����
        (F32)command.mY, // ���񖽗�
        (F32)mGyroSensor.get(),
        (F32)mGyroOffset,
        (F32)mLeftMotor.getCount(),
        (F32)mRightMotor.getCount(),
        (F32)mNxt.getBattMv(),
        &pwmL,
        &pwmR);

	PWMR = pwmR;
	PWML = pwmL;
	
    if (! DESK_DEBUG) {
        mLeftMotor.setPWM(pwmL);
        mRightMotor.setPWM(pwmR);
    }
}

/**
 * �u���[�L�̑���B
 *
 * �o�����X�𐧌�͍s��Ȃ��B���[�^���~�߂邾���B
 *
 * @return -
 */
void Activator::stop()
{
	mLeftMotor.setPWM(0);
	mRightMotor.setPWM(0);
	mLeftMotor.setBrake(true);
	mRightMotor.setBrake(true);
}

/**
 * �u���[�L���s(������)�B
 *
 * 
 *
 * @return -
 */
void Activator::slow()
{
	mLeftMotor.setPWM((S8)(PWML));
	mRightMotor.setPWM((S8)(PWMR));
}

