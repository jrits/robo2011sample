//
// Driver.h
//
#ifndef DRIVER_H_
#define DRIVER_H_

#include "constants.h"
#include "Skill.h"

/**
 * �h���C�o�̒��ۃN���X�A���̃N���X�̃I�u�W�F�N�g�͐�������Ȃ�
 */
class Driver
{
protected:
    int mState; //!< ���ݏ��
    Skill* mSkill; //!< ���݃h���C�o
public:
    /**
     * �R���X�g���N�^
     */
	Driver(){}
    /**
     * �f�X�g���N�^
     */
	virtual ~Driver(){}
    /**
     * �^�]����B
     * 
     * ���݂̏󋵂ɂ��킹�ēK�؂ȃX�L����I�����A�X�L���𔭊����邱�ƂŃ��{�b�g���^�]����B
     *
     * @retval true �ŏI��ԁB����ȍ~�̏�ԑJ�ڂȂ��B
     * @retval false �܂���ڂ��������Ă��܂���B
     *
     * Override Me!
     */
	virtual bool drive() { return true; }
};
#endif
