/**
 ******************************************************************************
 **	�t�@�C���� : sample.c
 **
 **	�T�v : 2�֓|���U�q���C���g���[�X���{�b�g��TOPPERS/ATK1(OSEK)�pC�T���v���v���O����
 **
 ** ���L : sample_c2 (sample_c1�ɒ����g�Z���T�ɂ���Q�����m�@�\��ǉ�)
 ******************************************************************************
 **/
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h" /* �|���U�q����p�w�b�_�t�@�C�� */


/* ���L�̃}�N���͌�/���ɍ��킹�ĕύX����K�v������܂� */
/* sample_c1�}�N�� */
#define GYRO_OFFSET  605 /* �W���C���Z���T�I�t�Z�b�g�l(�p���x0[deg/sec]��) */
#define LIGHT_WHITE	 500 /* ���F�̌��Z���T�l */
#define LIGHT_BLACK	 700 /* ���F�̌��Z���T�l */
/* sample_c2�}�N�� */
#define SONAR_ALERT_DISTANCE 30 /* �����g�Z���T�ɂ���Q�����m����[cm] */

/* �֐��v���g�^�C�v�錾 */
static int sonar_alert(void);

//*****************************************************************************
// �֐��� : ecrobot_device_initialize
// ���� : �Ȃ�
// �߂�l : �Ȃ�
// �T�v : ECROBOT�f�o�C�X�����������t�b�N�֐�
//*****************************************************************************
void ecrobot_device_initialize()
{
	ecrobot_set_light_sensor_active(NXT_PORT_S3); /* ���Z���T�ԐFLED��ON */
	ecrobot_init_sonar_sensor(NXT_PORT_S2); /* �����g�Z���T(I2C�ʐM)�������� */
}

//*****************************************************************************
// �֐��� : ecrobot_device_terminate
// ���� : �Ȃ�
// �߂�l : �Ȃ�
// �T�v : ECROBOT�f�o�C�X�I�������t�b�N�֐�
//*****************************************************************************
void ecrobot_device_terminate()
{
	ecrobot_set_light_sensor_inactive(NXT_PORT_S3); /* ���Z���T�ԐFLED��OFF */
	ecrobot_term_sonar_sensor(NXT_PORT_S2); /* �����g�Z���T(I2C�ʐM)���I�� */
}

//*****************************************************************************
// �֐��� : user_1ms_isr_type2
// ���� : �Ȃ�
// �߂�l : �Ȃ�
// �T�v : 1msec�������荞�݃t�b�N�֐�(OSEK ISR type2�J�e�S��)
//*****************************************************************************
void user_1ms_isr_type2(void){}

//*****************************************************************************
// �^�X�N���@: TaskMain
// �T�v	�@: ���C���^�X�N
//*****************************************************************************
TASK(TaskMain)
{
	signed char forward;      /* �O��i���� */
	signed char turn;         /* ���񖽗� */
	signed char pwm_L, pwm_R; /* ���E���[�^PWM�o�� */

	while(1)
	{
		if (ecrobot_get_touch_sensor(NXT_PORT_S4) == 1)
		{
			break; /* �^�b�`�Z���T�������ꂽ */
		}
	}

	balance_init();						/* �|���U�q���䏉���� */
	nxt_motor_set_count(NXT_PORT_C, 0); /* �����[�^�G���R�[�_���Z�b�g */
	nxt_motor_set_count(NXT_PORT_B, 0); /* �E���[�^�G���R�[�_���Z�b�g */
	while(1)
	{
		if (sonar_alert() == 1) /* ��Q�����m */
		{
			forward = turn = 0; /* ��Q�������m�������~ */
		}
		else
		{
			forward = 50; /* �O�i���� */
			if (ecrobot_get_light_sensor(NXT_PORT_S3) <= (LIGHT_WHITE + LIGHT_BLACK)/2)
			{
				turn = 50;  /* �E���񖽗� */
			}
			else
			{
				turn = -50; /* �����񖽗� */
			}
		}

		/* �|���U�q����(forward = 0, turn = 0�ŐÎ~�o�����X) */
		balance_control(
			(float)forward,								 /* �O��i����(+:�O�i, -:��i) */
			(float)turn,								 /* ���񖽗�(+:�E����, -:������) */
			(float)ecrobot_get_gyro_sensor(NXT_PORT_S1), /* �W���C���Z���T�l */
			(float)GYRO_OFFSET,							 /* �W���C���Z���T�I�t�Z�b�g�l */
			(float)nxt_motor_get_count(NXT_PORT_C),		 /* �����[�^��]�p�x[deg] */
			(float)nxt_motor_get_count(NXT_PORT_B),		 /* �E���[�^��]�p�x[deg] */
			(float)ecrobot_get_battery_voltage(),		 /* �o�b�e���d��[mV] */
			&pwm_L,										 /* �����[�^PWM�o�͒l */
			&pwm_R);									 /* �E���[�^PWM�o�͒l */
		nxt_motor_set_speed(NXT_PORT_C, pwm_L, 1); /* �����[�^PWM�o�̓Z�b�g(-100�`100) */
		nxt_motor_set_speed(NXT_PORT_B, pwm_R, 1); /* �E���[�^PWM�o�̓Z�b�g(-100�`100) */

		systick_wait_ms(4); /* 4msec�E�F�C�g */
	}
}

//*****************************************************************************
// �֐��� : sonar_alert
// ���� : ����
// �Ԃ�l : 1(��Q������)/0(��Q������)
// �T�v : �����g�Z���T�ɂ���Q�����m
//*****************************************************************************
static int sonar_alert(void)
{
	static unsigned int counter = 0;
	static int alert = 0;

	signed int distance;

	if (++counter == 40/4) /* ��40msec�������ɏ�Q�����m  */
	{
		/*
		 * �����g�Z���T�ɂ�鋗����������́A�����g�̌��������Ɉˑ����܂��B
		 * NXT�̏ꍇ�́A40msec�������x���o����̍ŒZ��������ł��B
		 */
		distance = ecrobot_get_sonar_sensor(NXT_PORT_S2);
		if ((distance <= SONAR_ALERT_DISTANCE) && (distance >= 0))
		{
			alert = 1; /* ��Q�������m */
		}
		else
		{
			alert = 0; /* ��Q������ */
		}
		counter = 0;
	}

	return alert;
}
