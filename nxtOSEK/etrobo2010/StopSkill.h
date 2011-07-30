//
// StopSkill.h
//

#ifndef StopSkill_H_
#define StopSkill_H_

#include "Skill.h"
#include "struct.h"

/**
 * �}��~�X�L���B�ڕW�n�_�ő��x���}��0�ɗ��Ƃ��B
 *
 * �X�L���ɋ}��~�@�\��ǉ����� Decorator �N���X�B
 */
class StopSkill : public Skill
{
    Skill *mSkill;         //!< �}��~�@�\��ǉ�����X�L���B
    float mTargetDistance; //!< ��~�ڕW����(mm)
    float mAllowableError; //!< ���e�G���[�l(mm)�B��mm��O�ő��x��0�ɗ��Ƃ����B
public:
	StopSkill();
	~StopSkill(){}
public:
    void setSkill(Skill *skill);
    void setTargetDistance(float targetDistance);
	void setAllowableError(float allowableError);
    bool isArrived();
public:
	VectorT<float> calcCommand();
};
#endif
