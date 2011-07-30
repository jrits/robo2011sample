//
// SlowdownSkill.h
//

#ifndef SlowdownSkill_H_
#define SlowdownSkill_H_

#include "Skill.h"
#include "struct.h"

/**
 * �����X�L��
 *
 * �X�L���Ɍ����@�\��ǉ����� Decorator �N���X�B
 */
class SlowdownSkill : public Skill
{
    Skill *mSkill;         //!< �����@�\��ǉ�����X�L���B
    float mMinimumForward; //!< �����Œᑬ�x(�f�t�H���g10)
    float mTargetDistance; //!< ��~�ڕW����(mm)
    float mAllowableError; //!< ���e�G���[�l(mm)
public:
	SlowdownSkill();
	~SlowdownSkill(){}
public:
    void setSkill(Skill *skill);
    void setTargetDistance(float targetDistance);
    void setMinimumForward(float minimumForward);
	void setAllowableError(float allowableError);
    bool isArrived();
    bool isPassed();
public:
	VectorT<float> calcCommand();
};
#endif
