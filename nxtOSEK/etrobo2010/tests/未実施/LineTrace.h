///////////////////////////////////////////////////////////
//  LineTrace.h
//  Implementation of the Class LineTrace
//  Created on:      10-9-2010 19:43:08
//  Original author: skawasak
///////////////////////////////////////////////////////////

#if !defined(EA_F7286BF8_B846_4e5c_82B2_A69DC6C89781__INCLUDED_)
#define EA_F7286BF8_B846_4e5c_82B2_A69DC6C89781__INCLUDED_

#include "LightSensor.h"
#include <iostream>
#include <cassert>
using namespace std;
//#include "PID.h"
//#include "Skill.h"


class LineTrace //: public Skill
{
private:
	float mBlack;
	float mWhite;
	float mThreshold;
	LightSensor &mLightSensor;

//	VectorT<float> calcVector();
	float calcVector();
	float normalizeLightValue();
	
public:
	LineTrace(float black, float white, float lineThresh, LightSensor &light);
	virtual ~LineTrace();
	//skill�N���X���Ȃ����߃w�b�_���ɂ�drive�֐���錾,���ۂ̓I�[�o�[���[�h����͂�
	void drive() {
        float Y; // �����
        Y = calcVector();
        
        //VectorT<F32> command;
        //command.mX = (F32)mForward; // �O��i���߃Z�b�g
        //command.mY = (F32)Y;        // ���񖽗߃Z�b�g
        
        //mActivator.run(command);//����@��ɃZ�b�g
    }
//	PID &mPID;

};
#endif // !defined(EA_F7286BF8_B846_4e5c_82B2_A69DC6C89781__INCLUDED_)
