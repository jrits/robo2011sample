//
// Course.h
//
#ifndef COURSE_H_
#define COURSE_H_

#include "constants.h"
#include "Driver.h"

/**
 * ���ۃR�[�X(�i�r�Q�[�^)�N���X
 */
class Course
{
protected:
    int mState; //!< ���ݏ��
    Driver *mDriver; //!< ���݃h���C�o
public:
    /**
     * �R���X�g���N�^
     */
	Course(){}
    /**
     * �f�X�g���N�^
     */
	virtual ~Course(){}
    /**
     * ���݂̏󋵂ɓK�؂ȃh���C�o��I�����A�h���C�o�ɉ^�]������
     *
     * Override Me!
     */
	virtual void drive(){}
};
	
#endif /*COURSE_H_*/
