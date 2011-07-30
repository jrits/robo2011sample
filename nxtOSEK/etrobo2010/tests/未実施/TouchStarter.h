//
// TouchStarter.h
//
// �^�b�`�X�^�[�^�X�^�u

#ifndef TOUCHSTARTER_H_
#define TOUCHSTARTER_H_

#include "TouchSensor.h"

class TouchStarter
{
private:
	TouchSensor& mrTouch;

public:
	TouchStarter(TouchSensor& touch): mrTouch(touch) {}

	~TouchStarter(){}
	
	// �^�b�`�Z���T�����ŊJ�n
	void waitForReadyGo(){ while(!mrTouch.isPressed()); }
};

#endif /*TOUCHSTARTER_H_*/
