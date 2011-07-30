//
// InCourse.h
//
#ifndef INCOURSE_H_
#define INCOURSE_H_

#include "Course.h"

/**
 * �C���R�[�X
 */
class InCourse : public Course
{
public:
    /**
     * �C���R�[�X�̋��
     */
    enum eSection {
        TESTDRIVE, //!< �e�X�g�h���C�o�N��
        START,     //!< �X�^�[�g���
        ENIGMA,    //!< �G�j�O�}���
        MYSTERY,   //!< �~�X�e���[�T�[�N�����
        GARAGEIN   //!< �K���[�W�E�C�����
    };
public:
 	InCourse(InCourse::eSection aSection = InCourse::START);
	~InCourse(){}
    void drive();
};
	
#endif /*INCOURSE_H_*/
