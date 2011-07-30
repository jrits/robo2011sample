//
// Activator.h
//
#ifndef Activator_H_
#define Activator_H_

#include "constants.h"
#include "Motor.h"
#include "GyroSensor.h"
#include "Vector.h"
#include "Nxt.h"

using namespace ecrobot;

extern "C"
{
	#include "balancer.h"
};

/**
 * ����@��(�n���h���A�A�N�Z���A�u���[�L)
 *
 * Skill �ɂ���đ��삳���B
 */
class Activator
{
private:
	Motor &mLeftMotor; //!< �����[�^
	Motor &mRightMotor; //!< �E���[�^
	GyroSensor &mGyroSensor; //!< �W���C���Z���T
	Nxt &mNxt; //!< NXT�I�u�W�F�N�g
    int mGyroOffset; //!< �W���C���Z���T�I�t�Z�b�g�l
public:
	Activator(Motor &leftMotor, 
              Motor &rightMotor, 
              GyroSensor &gyroSensor, 
              Nxt &nxt);
	~Activator(){}
    void reset(int gyroOffset);
	void run(VectorT<F32> command);
	void stop();
	void slow();
private:
    // Vector<S8> balanceControl(VectorT<F32> command); // balancer_control �𒼂ɌĂԂ��Ƃ�
};
#endif
