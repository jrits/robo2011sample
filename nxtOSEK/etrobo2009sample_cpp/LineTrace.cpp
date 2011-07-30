//
// LineTrace.cpp
//

#include "LineTrace.h"


#define WHITE		  500 // ���F�̌��Z���T�l
#define BLACK		  700 // ���F�̌��Z���T�l

#define FORWARD       50 // �O�i����

//=============================================================================
// ���C���g���[�X����
VectorT<S8> LineTrace::getCommand()
{
	if (mLight.get() <= (WHITE + BLACK)/2)
		return VectorT<S8>(FORWARD, 50); // �E��
	else
		return VectorT<S8>(FORWARD,-50); // ����
}
