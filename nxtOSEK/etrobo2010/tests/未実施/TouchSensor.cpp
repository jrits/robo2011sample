//
// TouchSensor.cpp
//
// �^�b�`�Z���T�X�^�u
//

#include "TouchSensor.h"

//=============================================================================
// Constructor
TouchSensor::TouchSensor(){
	pressed = "";
}

//=============================================================================
// get Touch Sensor status
bool TouchSensor::isPressed()
{
	bool val = false;
	pressed = " ";
	
	/* 1�����͂����܂Ń��[�v */
	cout << "press 1 to start " << endl;
	cin >> pressed;
	
	if(pressed == "1")
		val = true;
	return val;
}
