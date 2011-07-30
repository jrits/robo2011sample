//
// CoordinateStop.h
//

#ifndef COORDINATESTOP_H_
#define COORDINATESTOP_H_

#include "Skill.h"
#include "struct.h"

/**
 * ���W�w�蒅�n
 */
class CoordinateStop : public Skill
{
private:
    Point mTargetCoordinate; //!< �ڕW���W
    float mSlowdownDistance; //!< �ڕW���W�̉�mm��O���璅�n������n�߂邩
private:
    float mTargetDistance;   //!< ��~�ڕW����
public:
	CoordinateStop();
	~CoordinateStop(){}
    void setTargetCoordinate(Point targetCoordinate);
	void setSlowdownDistance(float slowdownDistance);
public:
    void setTargetDistance(float targetDistance);
protected:
	VectorT<float> calcCommand();
};
#endif
