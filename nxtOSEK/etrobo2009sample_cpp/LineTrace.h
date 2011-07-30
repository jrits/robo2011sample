//
// LineTrace.h
//

#ifndef LINETRACE_H_
#define LINETRACE_H_

#include "Driver.h"
#include "LightSensor.h"
using namespace ecrobot;

class LineTrace: public Driver
{
private:
	LightSensor& mLight;
	
public:
	LineTrace(LightSensor& light): mLight(light) {}

	~LineTrace(){}

	// ���C���g���[�X����
	virtual VectorT<S8> getCommand();
};

#endif
