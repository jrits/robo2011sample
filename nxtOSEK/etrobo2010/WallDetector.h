///////////////////////////////////////////////////////////
//  WallDetector.h
//  Implementation of the Class WallDetector
//  Created on:      14-9-2010 9:22:12
//  Original author: skawasak
///////////////////////////////////////////////////////////

#if !defined WALLDETECTOR_H
#define WALLDETECTOR_H

#include "Detector.h"

/**
 * �Փˌ��m
 */
class WallDetector : public Detector
{
private:
	float mThreshold;   //!< 臒l
public:
	WallDetector(){ mThreshold = 60; }
	~WallDetector(){}
    /**
     * �Z�b�^
     */
	void setThreshold(float threshold) { mThreshold = threshold; }
	bool detect();
};
#endif // !defined WALLDETECTOR_H
