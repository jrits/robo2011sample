//
// Pid.h
//
#include"Pid.h"

/**
 * �R���X�g���N�^
 *
 * @param[in] kp ���W��
 * @param[in] ki �ϕ��W��
 * @param[in] kd �����W��
 */
Pid::Pid(float kp, float ki, float kd) : 
    mKp(kp), mKi(ki), mKd(kd)
{
	index = 0;
	for(int i = 0; i < MAX_BUF_SIZE; i++)
	{
		buf[i] = 0;
	}
}

/**
 * �f�X�g���N�^
 */
Pid::~Pid()
{
}

/**
 * PID����ɂ�鑀��ʂ̌v�Z
 *
 * @param[in] P ��ᐬ��
 * @return �����
 */
float Pid::control(float P)
{
    // Nothing but buffering (Ring bufffer)
    float pP = buf[index]; //oldest value
    buf[index] = P;  // newest value
	index = (index + 1) % MAX_BUF_SIZE;
    
    // Integral
	// I += P;
    float I = 0;
	for(int i = 0; i < MAX_BUF_SIZE; i++)
	{
        I += buf[i];
    }

    //Derivative
	float D = P - pP;

	float Y = (mKp * P) + (mKi * I) + (mKd * D);
	
    return Y;
}
