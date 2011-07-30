//
// NXTway_GS.cpp
//

#include "NXTway_GS.h"

NXTway_GS::NXTway_GS(const Nxt& nxt, const GyroSensor& gyro, Motor& motorL, Motor& motorR)
:
    mrNxt(nxt),
    mrGyro(gyro),
    mrMotorL(motorL),
    mrMotorR(motorR)
{
    this->reset();
}

//=============================================================================
// ���䏉����
void NXTway_GS::reset(S16 offset)
{
    // ���[�^���Z�b�g
    mrMotorL.reset();
    mrMotorR.reset();

    // �|���U�q����̏�����
    balance_init();

    this->setGyroOffset(offset);
}

//=============================================================================
// �W���C���Z���T�I�t�Z�b�g�l�̐ݒ�
void NXTway_GS::setGyroOffset(S16 offset)
{
    mGyroOffset = offset;
}

//=============================================================================
// �h���C�o�[���߂ɂ��2�֓|���U�q���s�̂̐���
void NXTway_GS::drive(Driver* driver)
{
    S8 pwmL, pwmR;
	
    VectorT<S8> command = driver->getCommand();

    balance_control(
        (F32)command.mX, // �O��i����
        (F32)command.mY, // ���񖽗�
        (F32)mrGyro.get(),
        (F32)mGyroOffset,
        (F32)mrMotorL.getCount(),
        (F32)mrMotorR.getCount(),
        (F32)mrNxt.getBattMv(),
        &pwmL,
        &pwmR);
		
    mrMotorL.setPWM(pwmL);
    mrMotorR.setPWM(pwmR);
}
