//
// VirtualLineTrace.h
//

#ifndef VirtualLineTrace_H_
#define VirtualLineTrace_H_

#include "Skill.h"
#include "struct.h"

/**
 * ���z���C���g���[�X�X�L��
 */
class VirtualLineTrace : public Skill
{
private:
    ControlPoint *mControlPoints; //!< ����_�Z�b�g
    int mNumOfControlPoints; //!< ����_�̐�
    int mCurrentIndex; //!< ���ݐ���_�C���f�b�N�X
	float mXoffset; //!< ���̂��߂̒����p�ϐ�
	float mYoffset; //!< ���̂��߂̒����p�ϐ�
public:
	VirtualLineTrace();
	~VirtualLineTrace(){}
	void setControlPoints(ControlPoint *controlPoints, int numOfControlPoints);
    void reset();
public:
	VectorT<float> calcCommand();
    bool isLast();
};
#endif
