//
// Pid.h
//
#ifndef PID_H_
#define PID_H_

#include "constants.h"

#define MAX_BUF_SIZE 10

/**
 * PID ����N���X
 */
class Pid
{
private:
	float mKp; //!< ���W��
	float mKi; //!< �ϕ��W��
	float mKd; //!< �����W��
	int index; //!< �����O�o�b�t�@�̌��݃C���f�b�N�X
	float buf[MAX_BUF_SIZE]; //!< �����O�o�b�t�@�̃o�b�t�@
public:
	Pid(float kp, float ki, float kd);
	virtual ~Pid();
public:
	float control(float P);
};

#endif
