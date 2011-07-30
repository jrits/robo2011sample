//
// LineTrace.cpp
//
#include "LineTrace.h"
#include "factory.h"

/**
 * �R���X�g���N�^
 *
 * @param[in] black ��
 * @param[in] white ��
 * @param[in] threshold ����臒l
 */
LineTrace::LineTrace(float black, float white, float threshold)
{
	mBlack = black;
	mWhite = white;
	mLineThreshold = threshold;
    mInitForward = INIT_FORWARD;
    mInitDuration = INIT_SAMPLECOUNT;
    mUseOnoff = false;
    setForward(FORWARD);
    reset();
}

/**
 * ��Ԃ̃��Z�b�g
 *
 * ��������n�߂�
 */
void LineTrace::reset()
{
    mTimeCounter = 0;
}

/**
 * �����̐ݒ�
 *
 * @param[in] initForward ����
 */
void LineTrace::setInitForward(int initForward)
{
    mInitForward = initForward;
}

/**
 * �����p���J�E���^���Ԃ̐ݒ�
 *
 * @param[in] initDuration �����p���J�E���^����
 */
void LineTrace::setInitDuration(int initDuration)
{
    mInitDuration = initDuration;
}

/**
 * ON/OFF����(true) or PID����(false)�t���O�̐ݒ�
 *
 * @param[in] useOnoff ON/OFF����(true) or PID����(false)�t���O
 */
void LineTrace::setUseOnoff(bool useOnoff)
{
    mUseOnoff = useOnoff;
}

/**
 * ���݂̌��l����A���C���g���[�X������̂ɓK�؂ȑ��s�x�N�g�����v�Z����B
 *
 * @return ���s�x�N�g��
 */
VectorT<float> LineTrace::calcCommand()
{
	gLineTrace = true;  //���s�X�L���t���O�B���C���g���[�X���̂�true�ƂȂ�B(Gps�̕␳�̂��߂̃t���O)
    // �N�����}�_�b�V�����邽�߁A�ŏ��̂݃X�s�[�h�����߂�
    float X;
    if (mTimeCounter < mInitDuration) {
        mTimeCounter++;
        X = mInitForward;
    }
    else { // �ʏ�X�s�[�h
        X = mForward;
    }

#if 0 // DEBUG
    {
        static int count = 0;
        if (count++ % 25 == 0)
        {
            Lcd lcd;
            lcd.clear();
            lcd.putf("sn", "LineTrace");
            lcd.putf("dn", mState);
            lcd.putf("dn", mTimeCounter);
            lcd.putf("dn", (int)X);
            lcd.disp();
        }
    }
#endif
    float Y;
    if (mUseOnoff) { 
        Y = calcCommandTurnByOnOff();
    }
    else {
        Y = calcCommandTurn();
    }
    return VectorT<F32>(X,Y);
}

/**
 * (ONOFF����)���݂̌��l����A���C���g���[�X������̂ɓK�؂ȃ^�[���l���v�Z����B
 *
 * @return �^�[���l
 */
float LineTrace::calcCommandTurnByOnOff()
{
    float P = (mLightSensor.get() - mLineThreshold); // �΍�

	//ONOFF����
    float Y;
    if (P < 0) { // ��
        Y = -LIGHT_ONOFF_K;
    }
    else { // ��
        Y = LIGHT_ONOFF_K;
    }
	
	//���C���̉E�����g���[�X���邩�������g���[�X���邩�Ő�����������܂�
	if(TRACE_EDGE == LEFT ) Y *= -1;
	if(TRACE_EDGE == RIGHT) Y *=  1;
    
    return Y;
}

/**
 * (PID����)���݂̌��l����A���C���g���[�X������̂ɓK�؂ȃ^�[���l���v�Z����B
 *
 * @return �^�[���l
 */
float LineTrace::calcCommandTurn()
{
	//���K���������Z���T�l��P�Ɋi�[����
	float P = this->lightValueNormalization();
	//Pid����
    float Y = mLightPid.control(P);
	
	//���C���̉E�����g���[�X���邩�������g���[�X���邩�Ő�����������܂�
	if(TRACE_EDGE == LEFT ) Y *= -1;
	if(TRACE_EDGE == RIGHT) Y *=  1;
    
    return Y;
}

/**
 * ���K���������Z���T�̒l���擾����
 *
 * @return ���K���������Z���T�̒l
 */
float LineTrace::lightValueNormalization()
{
    float L = 0;
	L = mLightSensor.get();
	
    float P = (L - mLineThreshold); // �΍�
    if(L < mLineThreshold){ // ��
        P = P / (mLineThreshold - mWhite); // [-1.0, 1.0] �̒l�ɐ��K��
    }
    else{ // ��
        P = P / (mBlack - mLineThreshold); // [-1.0, 1.0] �̒l�ɐ��K��
    }
	
	if(P > 1) P = 1;
	if(P < -1) P = -1;
	
    return P;
}
