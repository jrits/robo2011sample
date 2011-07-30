//
// OutCourse.h
//
#ifndef OUTCOURSE_H_
#define OUTCOURSE_H_

#include "Course.h"

/**
 * �A�E�g�R�[�X
 */
class OutCourse : public Course
{
public:
    /**
     * �A�E�g�R�[�X�̋��
     */
    enum eSection {
        TESTDRIVE, //!< �e�X�g�h���C�o�N��
        START,     //!< �X�^�[�g���
        SEESAW,    //!< �V�[�\�[���
        STAIRWAY,  //!< �K�i���
        GARAGEIN   //!< �K���[�W�E�C�����
    };
public:
 	OutCourse(OutCourse::eSection aState = OutCourse::START);
	~OutCourse(){}
    void drive();
};
	
#endif /*OUTCOURSE_H_*/
