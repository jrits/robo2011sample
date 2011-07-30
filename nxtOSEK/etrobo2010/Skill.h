//
// Skill.h
//
#ifndef SKILL_H_
#define SKILL_H_

#include "constants.h"
#include "Activator.h"
extern Activator mActivator;

/**
 * �X�L�����ۃN���X
 *
 * ���̃N���X�̃I�u�W�F�N�g�͐�������Ȃ�
 */
class Skill
{
protected:
	float mForward; //!< �t�H���[�h�l
public:
    /**
     * �R���X�g���N�^
     */
	Skill(){}
    /**
     * �f�X�g���N�^
     */
	virtual ~Skill(){}
    /**
     * �t�H���[�h�l���Z�b�g����
     *
     * @param[in] forward �t�H���[�h�l
     */
	void setForward(float forward)
    {
        mForward = forward;
    }
    /**
     * �X�L���𔭊����ă��{�b�g�𓮂����B
     *
     * calcCommand ��p���đ��s�x�N�g�������肵�A����@��(Activator)�𑀍삷��B
     */
    void execute()
    {
        VectorT<float> command = calcCommand();
        mActivator.run(command);//����@��ɃZ�b�g
    }
    /**
     * ���s�x�N�g�����v�Z
     *
     * Override Me!!
     */
	virtual VectorT<float> calcCommand() { return VectorT<float>(0,0); }
    // �������z�֐��ɂ���ƃv���O�����e�ʂ���剻����B�Ƃ肠�����������z�֐��������B
};

#endif
