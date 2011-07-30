//
// CoordinateTrace.h
//

#ifndef COORDINATETRACE_H_
#define COORDINATETRACE_H_

#include "Skill.h"
#include "struct.h"

/**
 * ���W�w�葖�s�X�L��
 */
class CoordinateTrace : public Skill
{
private:
	Point mTargetCoordinate; //!< �ڕW���W
    float mAllowableError;   //!< ���e�G���[�l(mm)
public:
	CoordinateTrace();
	~CoordinateTrace(){}
	void setTargetCoordinate(Point targetCoordinate);
	void setAllowableError(float allowableError);
public:
	VectorT<float> calcCommand();
public:
    bool isArrived();
};
#endif
