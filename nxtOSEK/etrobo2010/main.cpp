//
// �t�@�C���� : main.cpp
//
//	�T�v       : 2�֓|���U�q���C���g���[�X���{�b�g��TOPPERS/ATK(OSEK)�p���C���v���O����
//

#include "constants.h"
#include "factory.h"
using namespace ecrobot;

extern "C"
{
#if 0
} // �G�f�B�^�̎����C���f���g�@�\�����܂����n�b�N
#endif

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/**
 * Bluetooth �ڑ�
 *
 * ���̊֐��Ŏw�肵��Bluetooth�f�o�C�X�����ڑ��惍�{�b�g�̃f�o�C�X���ɐݒ肳��܂��B
 *
 * @param[in] lcd Lcd�N���X�I�u�W�F�N�g
 * @param[in] bt_name Bluetooth�f�o�C�X��
 */
static void connect_bt(Lcd &lcd, char BT_NAME[16]);

//=============================================================================
// TOPPERS/ATK declarations
DeclareCounter(SysTimerCnt);
DeclareTask(TaskDrive);
DeclareEvent(EventDrive);
DeclareAlarm(AlarmDrive);
DeclareTask(TaskGps);
DeclareEvent(EventGps);
DeclareAlarm(AlarmGps);
DeclareTask(TaskHistory);
DeclareEvent(EventHistory);
DeclareAlarm(AlarmHistory);
DeclareTask(TaskSonar);
DeclareEvent(EventSonar);
DeclareAlarm(AlarmSonar);
DeclareTask(TaskLogger);
DeclareEvent(EventLogger);
DeclareAlarm(AlarmLogger);


//=============================================================================
// �֐���	: user_1ms_isr_type2
// ����		: �Ȃ�
// �߂�l	: �Ȃ�
// �T�v		: 1msec�������荞�݃t�b�N�֐�(OSEK ISR type2�J�e�S��)
//
void user_1ms_isr_type2(void)
{
    (void)SignalCounter(SysTimerCnt); // Increment System Timer Count
    SleeperMonitor(); // Clock�N���X�܂���I2C�f�o�C�X�N���X�g�p���ɕK�v
    // ENTER �{�^���� bluetooth �ڑ����L�����Z��
    if (mNxt.getButtons() == Nxt::ENTR_ON && !mBluetooth.isConnected()) {
        mBluetooth.cancelWaitForConnection();
    }
}

// ECRobot C API �f�o�C�X�̏�����
void ecrobot_device_initialize(void)
{
	// ecrobot_init_sonar_sensor(NXT_PORT_S2);
}

// ECRobot C API �f�o�C�X�̏I��
void ecrobot_device_terminate(void)
{
	// ecrobot_term_sonar_sensor(NXT_PORT_S2);
}

// �^�X�N�ԋ��L������
bool gDoSonar = false; //!< �\�i�[�Z���T�����t���O
int gSonarDistance = 255; //!< �\�i�[�Z���T�̌���
bool gSonarIsDetected = false; //!< �՗����m�̌���
bool gTouchStarter = false; //!< �^�b�`�Z���T�����t���O

/*
 * Sonar�^�X�N
 */
TASK(TaskSonar)
{
    // 48msec ���ɃC�x���g�ʒm����ݒ�
    SetRelAlarm(AlarmSonar, 1, 48); 
    WaitEvent(EventSonar);

    int gSonarDetectCount = 0;
    int gSonarTotalCount = 0;
    float gSonarDetectRatio = 0.0;
    while (1) {
        if (! gDoSonar) {
            gSonarDetectCount = 0;
            gSonarTotalCount = 0;
        }
    	if (gDoSonar) {
            gSonarDistance = mSonarSensor.getDistance();
            gSonarDetectCount += (gSonarDistance < 60);
            gSonarTotalCount++;
            gSonarDetectRatio = (gSonarDetectCount / (float)gSonarTotalCount);
            gSonarIsDetected = (gSonarDetectRatio > 0.1);
        }

#if 0 // DEBUG
        gDoSonar = true;
        static int count = 0;
        if (count++ > 5) {
            Lcd lcd;
            lcd.clear();
            lcd.putf("sn", "TaskSonar");
            lcd.putf("dn", gDoSonar);
            lcd.putf("dn", gSonarDistance);
            lcd.putf("dn", gSonarDetectCount);
            lcd.putf("dn", gSonarTotalCount);
            lcd.putf("dn", gSonarIsDetected);
            lcd.disp();
        }
#endif
        // �C�x���g�ʒm��҂�
        ClearEvent(EventSonar);
        WaitEvent(EventSonar);
    }
}

/**
 * ���s�^�X�N
 */
TASK(TaskDrive)
{
    // 4msec ���ɃC�x���g�ʒm����ݒ�
    SetRelAlarm(AlarmDrive, 1, 4); 
    WaitEvent(EventDrive);
//     K_THETADOT = 10.5F;

    //connect_bt(mLcd, BT_NAME); // bluetooth�ڑ�
    mActivator.reset(USER_GYRO_OFFSET);

    while(!(gTouchStarter = mTouchSensor.isPressed()));
    bool doDrive = true;
    while (1) {
        if (mFailDetector.detect()) doDrive = false;
        if (doDrive) mCourse->drive();
    	else mActivator.stop();

        // �C�x���g�ʒm��҂�
        ClearEvent(EventDrive);
        WaitEvent(EventDrive);
        
    }
    TerminateTask();
}

/*
 * GPS�X�V�^�X�N
 */
TASK(TaskGps)
{
    // 4msec ���ɃC�x���g�ʒm����ݒ�
    SetRelAlarm(AlarmGps, 1, 4); 
    WaitEvent(EventGps);

    while (1) {
        mGps.update();
        // �C�x���g�ʒm��҂�
        ClearEvent(EventGps);
        WaitEvent(EventGps);
    }
}

/*
 * History�X�V�^�X�N
 */
TASK(TaskHistory)
{
    // 4msec ���ɃC�x���g�ʒm����ݒ�
    SetRelAlarm(AlarmHistory, 1, 4); 
    WaitEvent(EventHistory);

    while (1) {
        mLightHistory.update(mLightSensor.get());
        mRightMotorHistory.update(mRightMotor.getCount());
        mLeftMotorHistory.update(mLeftMotor.getCount());
        mRightMotorSpeedHistory.update(mRightMotorHistory.calcDifference());
        mLeftMotorSpeedHistory.update(mLeftMotorHistory.calcDifference());
        mDirectionHistory.update(mGps.getDirection());
        mDirectionAverageHistory.update(mDirectionHistory.calcAverage());
        mGyroHistory.update(mGyroSensor.get());
        // �C�x���g�ʒm��҂�
        ClearEvent(EventHistory);
        WaitEvent(EventHistory);
    }
}

/*
 * Logger(bluetooth)�^�X�N
 */
TASK(TaskLogger)
{
    // 100msec ���ɃC�x���g�ʒm����ݒ�
    SetRelAlarm(AlarmLogger, 1, 100); 
    WaitEvent(EventLogger);

    while(!gTouchStarter);
    while (1) {
        if (LOGGER_SEND == 1) {
            mDaq.send(LOGGER_DATAS08, LOGGER_DATAS32[0]);
        } 
        else if (LOGGER_SEND == 2) {
            mDaq.send(LOGGER_DATAS08, LOGGER_DATAU16, LOGGER_DATAS16, LOGGER_DATAS32);
        }

        // �C�x���g�ʒm��҂�
        ClearEvent(EventLogger);
        WaitEvent(EventLogger);
    }
}

/**
 * Bluetooth �ڑ�
 *
 * ���̊֐��Ŏw�肵��Bluetooth�f�o�C�X�����ڑ��惍�{�b�g�̃f�o�C�X���ɐݒ肳��܂��B
 *
 * @param[in] lcd Lcd�N���X�I�u�W�F�N�g
 * @param[in] bt_name Bluetooth�f�o�C�X��
 */
static void connect_bt(Lcd &lcd, char bt_name[16])
{
    //CHAR  name[16]; 
    U8 address[7];

    lcd.clear();

    if (mBluetooth.getDeviceAddress(address)) // get the device address
    {
        lcd.putf("sn", "BD_ADDR:");
        for (SINT i=0; i<7; i++) lcd.putf("x", address[i],2);

        mBluetooth.setFriendlyName(bt_name); // set the friendly device name
        if (mBluetooth.getFriendlyName(bt_name)) // display the friendly device name
        {
            lcd.putf("nssn", "BT_NAME: ", bt_name);
        }

        lcd.putf("nsn", "Connecting BT...");
        lcd.putf("sn",  "ENTR to cancel.");
        lcd.disp();

        if (mBluetooth.waitForConnection("1234", 0)) // wait forever
        {
            lcd.putf("s", "Connected.");
        }
    }
    else
    {
        lcd.putf("s", "BT Failed.");
    }

    lcd.putf("ns", "Press Touch.");
    lcd.disp();
}

};
