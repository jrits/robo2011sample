#include "CoordinateTrace.h"
#include "factory.h"
#include <math.h>

/**
 * �R���X�g���N�^
 */
CoordinateTrace::CoordinateTrace() :
    Skill()
{
    mTargetCoordinate = MakePoint(0, 0);
    setForward(100);
    mAllowableError = 100.0; // mm
}

/**
 * �ڕW�ʒu��ݒ肷��B
 *
 * @param[in] targetCoordinate �ڕW���W
 */
void CoordinateTrace::setTargetCoordinate(Point targetCoordinate)
{
    mTargetCoordinate = targetCoordinate;
};


/**
 * ���e�G���[�l��ݒ肷��B
 *
 * @param[in] allowableError ���e�G���[�l
 */
void CoordinateTrace::setAllowableError(float allowableError)
{
    mAllowableError = allowableError;
};

/**
 * �ڕW���W�ƌ��ݍ��W����A���W�w�葖�s���s���̂ɓK�؂ȑ��s�x�N�g�����v�Z����
 *
 * @return ���s�x�N�g��
 */
VectorT<float> CoordinateTrace::calcCommand()
{
    // ���ݍ��W�擾
    Point current;
    current.X = mGps.getXCoordinate();
    current.Y = mGps.getYCoordinate();

    // ���ݍ��W�ƖڕW���W�Ƃ̍�
    float diffX = mTargetCoordinate.X - current.X;
    float diffY = mTargetCoordinate.Y - current.Y;
 
#if 0
    //DESK_DEBUG = true; // ���[�^���񂳂Ȃ��f�o�O
    static int count = 0; // ������ static
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "CoordinateTrace");
        lcd.putf("dn", (int)mTargetCoordinate.X);
        lcd.putf("dn", (int)mTargetCoordinate.Y);
        lcd.putf("dn", (int)current.X);
        lcd.putf("dn", (int)current.Y);
        lcd.disp();
    }
#endif
    
    // �ڕW�ւ̊p�x���Z�o
    float targetDirection;
    if (diffX == 0.0 && diffY == 0.0) {
        targetDirection = mGps.getDirection();
    }
    else {
        double radian = Gps::atan2(diffY, diffX);
        if (isinf(radian)) targetDirection = mGps.getDirection();
        else targetDirection = Gps::radianToDegree(radian);
    }

    // �A���O���g���[�X�̗��p
    mAngleTrace.setTargetAngle(targetDirection);
    VectorT<float> command = mAngleTrace.calcCommand();
    command.mX = mForward; // �t�H���[�h�l�㏑��

    return command;
};

/**
 * �ڕW�ʒu�ɓ���������
 *
 * @retval true ��������
 * @retval false �܂�
 */
bool CoordinateTrace::isArrived()
{
    // ���ݍ��W�擾
    Point current;
    current.X = mGps.getXCoordinate();
    current.Y = mGps.getYCoordinate();

    // ���ݍ��W�ƖڕW���W�Ƃ̋���
    double diffX = mTargetCoordinate.X - current.X;
    double diffY = mTargetCoordinate.Y - current.Y;
    double distance = sqrt(pow(diffX, 2) + pow(diffY, 2));

    // ���e�͈͓�������臒l�ɂĔ���
    return (distance < mAllowableError);
}
