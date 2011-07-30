#include "VirtualLineTrace.h"
#include "factory.h"
#include <math.h>

/**
 * �R���X�g���N�^
 */
VirtualLineTrace::VirtualLineTrace() :
    Skill()
{
    mCurrentIndex = 0; // ���ݐ���_�C���f�b�N�X
    mControlPoints = (ControlPoint*)null; //����_���X�g(�Ȃ�)
    mNumOfControlPoints = 0; //����_�̌�(0)

    mCoordinateTrace.setForward(100); // �f�t�H���g�t�H���[�h�l
    mCoordinateTrace.setAllowableError(10.0); // �f�t�H���g���e�덷(mm)
    mAngleTrace.setAllowableError(2.0); // �f�t�H���g���e�덷(degree)
	
	mXoffset = -170; // mXoffset = 0; // @todo: ���̂��߂ɐ▭�Ȓ������K�v
	mYoffset = 0;
}

/**
 * ��Ԃ̃��Z�b�g
 *
 * ���ݐ���_�C���f�b�N�X�����Z�b�g����
 *
 * @return -
 */
void VirtualLineTrace::reset()
{
    mCurrentIndex = 0; // ���ݐ���_�C���f�b�N�X
}

/**
 * ����_�Z�b�g��ݒ肷��B
 *
 * @param[in] controlPoints ����_�Z�b�g
 * @param[in] numOfControlPoints ����_�̐�
 */
void VirtualLineTrace::setControlPoints(ControlPoint *controlPoints, int numOfControlPoints)
{
    mControlPoints = controlPoints;
    mNumOfControlPoints = numOfControlPoints;
};

/**
 * ���z���C���g���[�X���s���̂ɓK�؂ȑ��s�x�N�g�����v�Z����
 *
 * @return ���s�x�N�g��
 */
VectorT<float> VirtualLineTrace::calcCommand()
{
	gLineTrace = false;
    VectorT<float> command;

    // �C���f�b�N�X���ő�l�𒴂��ė������肵�Ȃ��悤�ɕی�
    mCurrentIndex = MIN(mCurrentIndex, mNumOfControlPoints - 1);

    // �ϐ����������̂ň�U�ʖ��ɕۑ�
    Point targetCoordinate = {mControlPoints[mCurrentIndex].X, mControlPoints[mCurrentIndex].Y};
    float direction = mControlPoints[mCurrentIndex].direction;
    float forward = mControlPoints[mCurrentIndex].forward;
    float allowableError = mControlPoints[mCurrentIndex].allowableError;
    //bool  slowdown = mControlPoints[mCurrentIndex].slowdown;
	bool slowdown = false; // ���O�����Bslowdown �t���O�����C�����A���t���O�Ƃ��Ďg���̂ŃL�����Z���B

    if (! isnan(targetCoordinate.X) && ! isnan(targetCoordinate.Y)) { 
        // ���W�w�葖�s
    	Point targetCoordinateOff = MakePoint(mControlPoints[mCurrentIndex].X + mXoffset, mControlPoints[mCurrentIndex].Y + mYoffset);
        mCoordinateTrace.setTargetCoordinate(targetCoordinateOff);
        if (! isnan(forward)) mCoordinateTrace.setForward(forward);
        if (! isnan(allowableError)) mCoordinateTrace.setAllowableError(allowableError);
        // �ʏ���W�w�葖�s
        if (! slowdown) { 
            command = mCoordinateTrace.calcCommand();
        }
        // �X���[�_�E�����W�w�葖�s
        else { 
            mSlowdownSkill.setSkill(&mCoordinateTrace);
            mSlowdownSkill.setTargetDistance(mGps.calcDistanceTo(targetCoordinateOff));
            command = mSlowdownSkill.calcCommand();
        }
        // ���̓_��
        if (mCoordinateTrace.isArrived()) mCurrentIndex++;
    }
    else if (! isnan(direction)) {
        // �����]��
        mAngleTrace.setTargetAngle(direction);
        command = mAngleTrace.calcCommand();
        command.mX = 0.0; // �����]���Ȃ̂Ńt�H���[�h�l0
        if (! isnan(allowableError)) mAngleTrace.setAllowableError(allowableError);
        // ���̓_��
        if (mAngleTrace.isArrived()) mCurrentIndex++;
    }
    else { // �������ȏꍇ�͂Ƃ肠�������C���g���[�X�����Ă����H�H
        command = mLineTrace.calcCommand();
    }

#if 1 // DEBUG
    static int count = 0;
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "VirtualLineTrace");
        lcd.putf("dn", mCurrentIndex);
        lcd.putf("dn", (int)targetCoordinate.X);
        lcd.putf("dn", (int)targetCoordinate.Y);
        lcd.putf("dn", (int)mGps.getXCoordinate());
        lcd.putf("dn", (int)mGps.getYCoordinate());
        lcd.putf("dn", (int)mGps.getDirection());
        lcd.disp();
    }
#endif

    return command;
};

/**
 * �ŏI����_��Ԃ��ǂ����𔻒肷��
 *
 * @retval true �ŏI����_��Ԃł���
 * @retval false �܂��܂�
 */
bool VirtualLineTrace::isLast()
{
    //return (mCurrentIndex >= mNumOfControlPoints - 1);
	return (mCurrentIndex >= mNumOfControlPoints - 1) || mControlPoints[mCurrentIndex].slowdown; // ���O�����Bslowdown �t���O�����C�����A���t���O�Ƃ��Ďg���B
}
