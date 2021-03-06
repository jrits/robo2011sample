//
// Pid.h
//
#include"Pid.h"

/**
 * RXgN^
 *
 * @param[in] kp äáW
 * @param[in] ki ÏªW
 * @param[in] kd ÷ªW
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
 * fXgN^
 */
Pid::~Pid()
{
}

/**
 * PID§äÉæéìÊÌvZ
 *
 * @param[in] P äá¬ª
 * @return ìÊ
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
