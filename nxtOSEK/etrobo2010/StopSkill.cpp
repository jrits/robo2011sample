#include "StopSkill.h"
#include "factory.h"

/**
 * �R���X�g���N�^
 */
StopSkill::StopSkill() :
    Skill()
{
    mTargetDistance = 0.0; // mm
    mAllowableError = 0.0;
    mSkill = &mLineTrace; // �Ƃ肠����
}

/**
 * �}��~�@�\��ǉ�����X�L����ݒ肷��
 *
 * @param[in] skill �}��~�@�\��ǉ������X�L��
 */
void StopSkill::setSkill(Skill *skill)
{
    if (skill == 0) skill = &mLineTrace; // �Ƃ肠����
    mSkill = skill;
}

/**
 * ��~�ڕW������ݒ肷��B
 *
 * @param[in] targetDistance ��~�ڕW����(���݂̈ʒu����̑��΋���)
 */
void StopSkill::setTargetDistance(float targetDistance)
{
    mTargetDistance = mGps.getDistance() + targetDistance;
};

/**
 * ���e�G���[�l��ݒ肷��B
 *
 * @param[in] allowableError ���e�G���[�l
 */
void StopSkill::setAllowableError(float allowableError)
{
    mAllowableError = allowableError;
};

/**
 * �ڕW�����ƌ��݋�������A�}��~������s���̂ɓK�؂ȑ��s�x�N�g�����v�Z����
 *
 * @return ���s�x�N�g��
 */
VectorT<float> StopSkill::calcCommand()
{
    // �X�L���̗��p�B�t�H���[�h�l��(�K�v�������)�㏑������B
    VectorT<float> command = mSkill->calcCommand();

    // �t�H���[�h�l�̐���B�ڕW�n�_�ɂ��ǂ蒅������t�H���[�h�l0
    if (isArrived()) command.mX = 0;

#if 0
    //DESK_DEBUG = true; // ���[�^���񂳂Ȃ��f�o�O
    static int count = 0; // ������ static
    float diff = mTargetDistance -  mGps.getDistance();
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "StopSkill");
        lcd.putf("dn", (int)mTargetDistance);
        lcd.putf("dn", (int)mAllowableError);
        lcd.putf("dn", (int)mGps.getDistance());
        lcd.putf("dn", (int)diff);
        lcd.putf("dn", (int)command.mX);
        lcd.disp();
    }
#endif

	return command;
}

/**
 * �ڕW�����ɓ���������
 *
 * @retval true ��������
 * @retval false �܂�
 */
bool StopSkill::isArrived()
{
    // �ڕW�����ƌ��݋����Ƃ̍�
    float diff = mTargetDistance -  mGps.getDistance();

    // ���e�͈͓�������臒l�ɂĔ���
    return (diff < mAllowableError);
}
