///////////////////////////////////////////////////////////
//  Detector.h
//  Implementation of the Interface Detector
//  Created on:      27-9-2010 10:12:12
//  Original author: nseo
///////////////////////////////////////////////////////////

#if !defined(EA_3E87576D_66A3_4a67_87CD_9668CD4E1E41__INCLUDED_)
#define EA_3E87576D_66A3_4a67_87CD_9668CD4E1E41__INCLUDED_

/**
 * ���o�n�N���X�C���^�[�t�F�[�X
 */
class Detector
{
public:
    /**
     * �R���X�g���N�^
     */
	Detector(){}
    /**
     * �f�X�g���N�^
     */
	~Detector(){}
    /**
     * ���o����
     *
     * @retval true ���o
     * @retval false �����o
     */
	bool detect();
};

#endif // !defined(EA_3E87576D_66A3_4a67_87CD_9668CD4E1E41__INCLUDED_)
