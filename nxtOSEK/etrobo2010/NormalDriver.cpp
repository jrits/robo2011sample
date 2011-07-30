///////////////////////////////////////////////////////////
//  NormalDriver.cpp
//  Implementation of the Class NormalDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#include "NormalDriver.h"
#include "factory.h"

/**
 * �R���X�g���N�^
 */
NormalDriver::NormalDriver()
{
}

/**
 * �m�[�}�����s�A���S���Y��
 *
 * �����̃��C���g���[�X
 *
 * @retval true �ŏI��ԁB����ȍ~�̏�ԑJ�ڂȂ��B
 * @retval false �܂���ڂ��������Ă��܂���B
 */
bool NormalDriver::drive()
{
    mLineTrace.execute();
    return true;
}
