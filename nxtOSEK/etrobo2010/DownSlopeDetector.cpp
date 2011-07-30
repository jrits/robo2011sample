///////////////////////////////////////////////////////////
//  DownSlopeDetector.cpp
//  Implementation of the Class DownSlopeDetector
//  Created on:      10-9-2010 19:42:52
//  Original author: skawasak
///////////////////////////////////////////////////////////

#include "DownSlopeDetector.h"
#include "factory.h"

/**
 * ����⌟�m����
 *
 * @retval true ���m����
 * @retval false ���m���Ȃ�����
 */
bool DownSlopeDetector::detect()
{
	float accR = mRightMotorSpeedHistory.calcDifference();
	float accL = mLeftMotorSpeedHistory.calcDifference();
	return (accR > DOWNSLOPE_THRESHOLD) && (accL > DOWNSLOPE_THRESHOLD);
}
