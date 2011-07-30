///////////////////////////////////////////////////////////
//  FailDetector.h
//  Implementation of the Class FailDetector
//  Created on:      11-25-2010 19:42:51
//  Original author: ksuzuki
///////////////////////////////////////////////////////////

#if !defined FailDetector_H
#define FailDetector_H

#include "Detector.h"
#include "History.h"

/**
 * �]�|���m
 */
class FailDetector : public Detector
{
private:
    int mTimeCounter; //!< �^�C���J�E���^
    int mPrevLeftCount; //!< �O��̍����[�^�G���R�[�_�l
    int mPrevRightCount; //!< �O��̉E���[�^�G���R�[�_�l
    // int mSampleCount; // FAILSAFE_SAMPLECOUNT
    // int mThreshold; //!< FAILSAFE_THRESHOLD
public:
	FailDetector();
	~FailDetector(){}
	bool detect();
};
#endif // !defined FailDetector_H
