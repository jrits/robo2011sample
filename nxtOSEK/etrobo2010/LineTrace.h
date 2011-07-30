//
// LineTrace.h
//

#ifndef LINETRACE_H_
#define LINETRACE_H_

#include "Skill.h"

/**
 * ���C���g���[�X�X�L��
 */
class LineTrace : public Skill
{
private:
	float mBlack;         //!< ��
	float mWhite;         //!< ��
	float mLineThreshold; //!< ����臒l
    int mInitForward;     //!< �����t�H���[�h�l
    int mInitDuration;    //!< �����t�H���[�h�l�p���J�E���^����
    int mTimeCounter;     //!< ���ԃJ�E���^
    bool mUseOnoff;       //!< ON/OFF����(true) or PID����(false)�t���O
    //Pid mLightPid(LIGHT_PID_KP, LIGHT_PID_KI, LIGHT_PID_KD);
    //float LIGHT_ONOFF_K;
public:
	LineTrace(float black, float white, float threshold);
	~LineTrace(){}
    void reset();
    void setInitForward(int initForward);
    void setInitDuration(int initDuration);
    void setUseOnoff(bool useOnoff);
public:
    VectorT<float> calcCommand();
private:
    float calcCommandTurnByOnOff();
    float calcCommandTurn();
	float lightValueNormalization();
};
#endif
