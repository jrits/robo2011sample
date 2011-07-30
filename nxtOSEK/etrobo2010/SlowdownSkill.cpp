#include "SlowdownSkill.h"
#include "factory.h"

/**
 * �R���X�g���N�^
 */
SlowdownSkill::SlowdownSkill() :
    Skill()
{
    mTargetDistance = 0.0; // mm
    mSkill = &mLineTrace; // �Ƃ肠����
    mMinimumForward = 10.0;
    mAllowableError = 0.0;
}

/**
 * �����@�\��ǉ�����X�L����ݒ肷��
 *
 * @param[in] skill �����@�\��ǉ������X�L��
 */
void SlowdownSkill::setSkill(Skill *skill)
{
    if (skill == 0) skill = &mLineTrace; // �Ƃ肠����
    mSkill = skill;
}

/**
 * ��~�ڕW������ݒ肷��B
 *
 * @param[in] targetDistance ��~�ڕW����(���݂̈ʒu����̑��΋���)
 */
void SlowdownSkill::setTargetDistance(float targetDistance)
{
    mTargetDistance = mGps.getDistance() + targetDistance;
};

/**
 * �����Œᑬ�x��ݒ肷��B
 *
 * @param[in] minimumForward �����Œᑬ�x
 */
void SlowdownSkill::setMinimumForward(float minimumForward)
{
    mMinimumForward = minimumForward;
};

/**
 * ���e�G���[�l��ݒ肷��B
 *
 * @param[in] allowableError ���e�G���[�l
 */
void SlowdownSkill::setAllowableError(float allowableError)
{
    mAllowableError = allowableError;
};

/**
 * �ڕW�����ƌ��݋�������A����������s���̂ɓK�؂ȑ��s�x�N�g�����v�Z����
 *
 * @return ���s�x�N�g��
 */
VectorT<float> SlowdownSkill::calcCommand()
{
    // �X�L���̗��p�B�t�H���[�h�l��(�K�v�������)�㏑������B
    VectorT<float> command = mSkill->calcCommand();

    // �t�H���[�h�l��PID����
    float P = mTargetDistance -  mGps.getDistance();
    float X = mSlowdownPid.control(P);
    if (X > command.mX) X = command.mX;
    if (X < mMinimumForward) X = mMinimumForward;
    command.mX = X;

#if 0
    //DESK_DEBUG = true; // ���[�^���񂳂Ȃ��f�o�O
    static int count = 0; // ������ static
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "SlowdownSkill");
        lcd.putf("dn", (int)mTargetDistance);
        lcd.putf("dn", (int)mAllowableError);
        lcd.putf("dn", (int)mGps.getDistance());
        lcd.putf("dn", (int)P);
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
bool SlowdownSkill::isArrived()
{
    // �ڕW�����ƌ��݋����Ƃ̍�
    float diff = mTargetDistance - mGps.getDistance();

    // ���e�͈͓�������臒l�ɂĔ���
    return (diff < mAllowableError);
}

/**
 * �ڕW�n�_��ʂ�߂������̔����Ԃ��B
 *
 * @return
 */
bool SlowdownSkill::isPassed()
{
    float diff = mTargetDistance -  mGps.getDistance();
    return (diff < 0);
};
