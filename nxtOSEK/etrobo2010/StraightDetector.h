//
// StraightDetector.h
//

#ifndef StraightDetector_H_
#define StraightDetector_H_

#include "Detector.h"

/**
 * �������m
 */
class StraightDetector : public Detector
{
public:
	StraightDetector(){}
	~StraightDetector(){}
    bool detect();
};

#endif
