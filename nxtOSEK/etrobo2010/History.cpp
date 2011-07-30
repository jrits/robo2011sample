///////////////////////////////////////////////////////////
//  Histrory.cpp
//  Implementation of the Class Histrory
//  Created on:      06-10-2010
//  Original author: yhayama
///////////////////////////////////////////////////////////

//#include<iostream>
#include "History.h"

/**
 * �R���X�g���N�^
 *
 * @param[in] buffer �o�b�t�@�Ƃ��Ďg���z����w���|�C���^
 * @param[in] size �o�b�t�@�T�C�Y
 */
History::History(float* buffer, int size):
    mBuffer(buffer),
    mSize(size),
    mIndex(0)
{
}

/**
 * �f�X�g���N�^
 */
History::~History()
{
}

/**
 * �o�b�t�@�T�C�Y�̎擾
 *
 * @return �o�b�t�@�T�C�Y
 */
int History::getSize()
{
    return mSize;
}

/**
 * �ړ����όv�Z�֐�
 *
 * �o�b�t�@�S�̂̕��ϒl���v�Z����
 *
 * @return ���ϒl
 */
float History::calcAverage()
{
    float sum = 0.0;
    float ave = 0.0;
    for (int i = 0; i < mSize ; i++) {
    	sum += mBuffer[i];
    }
    ave = sum / (float)mSize;
    return ave;
}

/**
 * ���ԍ����v�Z�֐�
 *
 * �o�b�t�@�ŐV�l�ƍŌÒl�̍����v�Z����
 *
 * @return �����l
 */
float History::calcDifference()
{
    return get(LATEST) - get(OLDEST);
}

/**
 * �l�̎擾
 *
 * @param[in] time �ǂ̎��Ԃ̒l���擾���邩�w��B
 * - 0 ���ŐV�B-1 �����O�B���̕����ɐi�ނɂ�ČÂ��l�ɂȂ�B
 * - �ȈՓI�� 1 ���w�肷�邱�ƂōŌÒl���擾�ł���B2 ���Q�ԖڂɌÂ��l�B3 ���c
 */
float History::get(int time)
{
    unsigned int index = (time + mIndex + mSize) % mSize;
    // std::cout << "time: " << time << " index: " << index <<  " value: " << mBuffer[index] << std::endl;
    return mBuffer[index];
}

/**
 * �q�X�g���̍X�V
 *
 * @return -
 */
void History::update(float data)
{
    mIndex = (mIndex + 1) % mSize;
    //float oldestValue = mBuffer[mIndex];
    mBuffer[mIndex] = data;
}
