//
// �t�@�C���� : Main.cpp
// �T�v       : 2�֓|���U�q���C���g���[�X���{�b�g��TOPPERS/ATK1(OSEK)�pC++�T���v���v���O����
//

// ECRobot C++ API
#include "TouchSensor.h"
#include "SonarSensor.h"
#include "LightSensor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "Nxt.h"
#include "Clock.h"
#include "Lcd.h"
#include "Speaker.h"
using namespace ecrobot;

#include "NXTway_GS.h"
#include "Driver.h"
#include "StartStopDriver.h"
#include "SonarDriver.h"
#include "LineTraceDriver.h"
#include "DriverManager.h"

//=============================================================================
// �f�o�C�X�I�u�W�F�N�g�̒�`
TouchSensor touch(PORT_1);
SonarSensor sonar(PORT_2);
LightSensor light(PORT_3);
GyroSensor   gyro(PORT_4);
Motor      motorR(PORT_B);
Motor      motorL(PORT_C);
Clock clock;
Lcd lcd;
Nxt nxt;
Speaker speaker;

// �����^�X�N����R�[�������h���C�o
StartStopDriver ssDriver;
SonarDriver sonarDriver;

extern "C"
{
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

//=============================================================================
// TOPPERS/ATK1�̐錾
DeclareCounter(SysTimerCnt);
DeclareAlarm(Alarm4msec);
DeclareEvent(EventDrive);

// �^�X�N�̋N������[msec]
#define DRIVE_PERIOD       (4)
#define BACKGROUND_PERIOD (48)

//=============================================================================
// �֐���	: user_1ms_isr_type2
// ����	: �Ȃ�
// �߂�l	: �Ȃ�
// �T�v	: 1msec�������荞�݃t�b�N�֐�(OSEK ISR type2�J�e�S��)
//=============================================================================
void user_1ms_isr_type2(void)
{
	(void)SignalCounter(SysTimerCnt); // Alarm�n���h��
	SleeperMonitor(); // Nxt�����I2C�f�o�C�X�̎g�p���ɕK�v
}

//=============================================================================
// �^�X�N���@: TaskMain
// �T�v	�@: ���C���^�X�N
//=============================================================================
TASK(TaskMain)
{
	lcd.clear();
	lcd.putf("s", "TOUCH:START/STOP");
	lcd.disp();

	SetRelAlarm(Alarm4msec, 1, DRIVE_PERIOD); // �h���C�u�pAlarm�^�C�}�[�̃Z�b�g

	while(1)
	{
		ssDriver.checkInput(touch.isPressed()); // �^�b�`�Z���T�̏�Ԃ����o
		sonarDriver.checkObstacles(sonar); // �����g�Z���T�ɂ���Q�����o

		clock.wait(BACKGROUND_PERIOD); // �E�F�C�g���[�v
	}
}

//=============================================================================
// �^�X�N���@: TaskDrive
// �T�v	�@: �h���C�u�^�X�N
//=============================================================================
TASK(TaskDrive)
{
	LineTraceDriver ltDriver(light);
	DriverManager drivers;
	(void)drivers.createDriverTable(3);
	(void)drivers.add(&ssDriver); // �ō��D��x
	(void)drivers.add(&sonarDriver);
	(void)drivers.add(&ltDriver); // �Œ�D��x

	NXTway_GS robot(nxt, gyro, motorL, motorR);

	while(1)
	{
        ClearEvent(EventDrive); // �h���C�u�C�x���g�̃N���A
		WaitEvent(EventDrive);  // �h���C�u�C�x���g�҂�

		drivers.notify(robot.readInternalData()); // ���s�̂̓����f�[�^���h���C�o�[�֒ʒm
		drivers.update(); // �h���C�o���N�G�X�g���擾����O�ɍX�V
		switch(drivers.getRequest())
		{
			case Driver::STOP:
				robot.stop();
				break;
			case Driver::START:
				CancelAlarm(Alarm4msec);            // �h���C�u�pAlarm�^�C�}�[�̃L�����Z��

				robot.reset(robot.calGyroOffset()); // �W���C���Z���T�̃L�����u���[�V�����ɖ�400msec������
				drivers.reset();
				speaker.playTone(440U, 500U, 30U);  // ���Z�b�g�����ʒm�u�U�[

				SetRelAlarm(Alarm4msec, 1, DRIVE_PERIOD); // �h���C�u�pAlarm�^�C�}�[�̃Z�b�g
				break;
			case Driver::DRIVE:
				robot.drive(drivers.getCommand());
				break;
			case Driver::NO_REQ:
			default:
				break; // Do nothing
		}
	}
}
};

