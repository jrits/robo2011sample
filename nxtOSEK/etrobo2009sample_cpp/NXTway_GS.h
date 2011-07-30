//
// NXTway_GS.h
//

#ifndef NXTWAY_GS_H_
#define NXTWAY_GS_H_

#include "Vector.h"
#include "Nxt.h"
#include "GyroSensor.h"
#include "Motor.h"
using namespace ecrobot;

#include "Driver.h"


extern "C"
{
#include "balancer.h"
};

class NXTway_GS
{
private:
	const Nxt& mrNxt;
	const GyroSensor& mrGyro;
	Motor& mrMotorL;
	Motor& mrMotorR;

	U16 mGyroOffset;

public:
	NXTway_GS(const Nxt& nxt, const GyroSensor& gyro, Motor& motorL, Motor& motorR);
	
	~NXTway_GS(){}

	// �|���U�q����̏�����
	void reset(S16 offset=GyroSensor::DEFAULT_OFFSET);

	// �W���C���Z���T�I�t�Z�b�g�l�̃Z�b�g
	void setGyroOffset(S16 offset);

	// �h���C�o�[���߂ɂ��2�֓|���U�q���s�̂̐���
	void drive(Driver* driver);
};

#endif

