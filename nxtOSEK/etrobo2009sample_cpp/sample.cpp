//
// �t�@�C���� : sample.cpp
//
//	�T�v       : 2�֓|���U�q���C���g���[�X���{�b�g��TOPPERS/ATK(OSEK)�p�T���v���v���O����
//

// ECRobot++ API
#include "TouchSensor.h"
#include "LightSensor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "Clock.h"
#include "Nxt.h"
using namespace ecrobot;

#include "TouchStarter.h"
#include "NXTway_GS.h"
#include "LineTrace.h"

extern "C"
{
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

//=============================================================================
// �f�o�C�X�I�u�W�F�N�g�̐���
//
TouchSensor touch(PORT_1);
LightSensor light(PORT_3);
GyroSensor   gyro(PORT_4);
Motor motorL(PORT_C);
Motor motorR(PORT_B);

#define USER_GYRO_OFFSET (610) // ���̒l�͓K�X�������邱��

//=============================================================================
// �֐���	: user_1ms_isr_type2
// ����		: �Ȃ�
// �߂�l	: �Ȃ�
// �T�v		: 1msec�������荞�݃t�b�N�֐�(OSEK ISR type2�J�e�S��)
//
void user_1ms_isr_type2(void)
{
	SleeperMonitor(); // Clock�N���X�܂���I2C�f�o�C�X�N���X�g�p���ɕK�v
}

//=============================================================================
// �^�X�N��	: TaskMain
// �T�v		: ���C���^�X�N
//
TASK(TaskMain)
{
	Clock clock;
	Nxt nxt;

	LineTrace lineTraceDriver(light); // ���C���g���[�X�h���C�o�[
	NXTway_GS robot(nxt, gyro, motorL, motorR); // ���s��

	TouchStarter(touch).waitForReadyGo(); // �^�b�`�Z���T�𑖍s�J�n����Ɏg�p

	robot.reset(USER_GYRO_OFFSET); // ���s�̂̐��䏉����

	while(1)
	{
		robot.drive(&lineTraceDriver); // ���C���g���[�X�^�]
		clock.wait(4);                 // 4msec�E�F�C�g
	}
}

};
