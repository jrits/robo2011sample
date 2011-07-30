//
// AngleTrace.h
//

#ifndef ANGLETRACE_H_
#define ANGLETRACE_H_

#include "Skill.h"

/**
 * �p�x�g���[�X�X�L��
 */
class AngleTrace : public Skill
{
private:
	float mTargetAngle; //!< �ڕW�p�x
    float mAllowableError; //!< ���e�덷�p�x�B�����]���Ɏg���B [0, 180]
public:
	AngleTrace();
	~AngleTrace(){}
	void setTargetAngle(float targetAangle);
    void setAllowableError(float allowableError);
public:
	VectorT<float> calcCommand();
    bool isArrived();
};
#endif
