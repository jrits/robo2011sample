//
// Gps.cpp
//

#include "Gps.h"
//���O�擾�p
#include "Bluetooth.h"
#include "Daq.h"
#include "Gps.h"
#include "Lcd.h"
extern Bluetooth bt;
extern Daq mDaq;
//--------------------
//History�N���X������́A�e���\�b�h���Ő錾
float prevXCoordinate = 0.0;
float prevYCoordinate = 0.0;
float prevEl = 0.0;
float prevEr = 0.0;

// compile �G���[���
bool gAngleTraceFlag; 

//	�␳�֌W�萔
#define DIRECTION_THRESHOLD 25
#define COURSE_WIDTH 500

//outcourse�p
#define X_CASE_0_4_START (1500)
#define X_CASE_0_4_END (4500)
#define Y_CASE_0_4 (-3400)
#define Y_CASE_1_5_START (-1132)
#define Y_CASE_1_5_END (-2703)
#define X_CASE_1_5 (5226)
#define X_CASE_2_6_START (1506)
#define X_CASE_2_6_END (4056)
#define Y_CASE_2_6 (-231)
#define Y_CASE_3_7_START (-1311)
#define Y_CASE_3_7_END (-2181)
#define X_CASE_3_7 (170)
//incourse�p
#define X_IN_2_6_START (1500)
#define X_IN_2_6_END (-4080)
#define Y_IN_2_6 (-525)
#define X_IN_3 (462)
#define Y_IN_3_START (-1230)
#define Y_IN_3_END (-2082)
#define X_IN_4_START (3010)
#define X_IN_4_END (3690)
#define Y_IN_4 (-3111)

//================== �����o���\�b�h ===================
/**
 * �R���X�g���N�^
 *
 * @param[in] aMotorL �����[�^
 * @param[in] aMotorR �E���[�^
 * @param[in] aCourse �R�[�X���ʎq
 */
Gps::Gps(Motor &aMotorL, Motor &aMotorR, Gps::eCourse aCourse) :
	mWheelRadius(WHEEL_RADIUS),
	mWheelDistance(WHEEL_DISTANCE),
	motorL(aMotorL), 
	motorR(aMotorR),
    mCourse(aCourse),
    mXCoordinate(GPS_COURSE_START_X),
    mYCoordinate(GPS_COURSE_START_Y),
    mDirection(GPS_COURSE_START_DIRECTION),
	mDistance(0.0),
	mXOffset(0.0),
	mYOffset(0.0),
	mDirectionOffset(0.0),
	mDistanceOffset(0.0),
	//�ȉ��␳�֌W�ϐ�
    mXAverage(0.0),
    mYAverage(0.0),
    mDirectionAverage(0.0),
    mTimeCounter(0)
{

}

/**
 * Gps���̍X�V
 */
void Gps::update()
{
	float angle = 0.0;  /* 4msec���Ƃ̎ԑ̊p�x�̕ω��� (�x) */
	float radius = 0.0; /* 4msec���Ƃ̎ԑ̂̕`���~�̔��a */
	float el = 0.0; /* 4msec�Ԃ̃G���R�[�_�[�l�̕ω���(left) */
	float er = 0.0; /* 4msec�Ԃ̃G���R�[�_�[�l�̕ω���(right) */
	float currEl = 0.0; /* ���݂̃G���R�[�_�[�l(left) */
	float currEr = 0.0; /* ���݂̃G���R�[�_�[�l(right) */
		
 /*********   �l�擾   *********/
	/* �G���R�[�_�[�l���擾 */
	currEl = motorL.getCount();
	currEr = motorR.getCount();
	
	/* 4msec�Ԃ̃G���R�[�_�[�l�̕ω��ʂ��v�Z */
	el = currEl - prevEl;
	er = currEr - prevEr;
/********* �l�擾�I�� *********/
	
/********* ���W�v�Z ***********/
	angle = calcAngle(el, er); /* �Ȃ������p�x���v�Z */
	
	radius = calcRadius(el, angle); /* ���s�̂̕`���~�̔��a���v�Z */
	
	calcCoordinates(angle, radius, el, er); /* ���W�̍X�V */
	calcDistance();
	
	calcDirection(angle); /* ���݌����Ă�������̍X�V */


/********* ���W�v�Z�I�� *******/

/************ ���W�����␳ *****************/

    mXAverage += getXCoordinate();
    mYAverage += getYCoordinate();
    mDirectionAverage += getDirection();
    /**
     * ���W�A�����␳�֐��Ăяo���B�Ăяo�������͓K���Ȃ��̂ɕύX����ׂ��B
     */
    if (mTimeCounter % 50 == 0) {
        mXAverage /= 50.0;
        mYAverage /= 50.0;
        mDirectionAverage /= 50.0;
        
        if (mCourse == Gps::OUT && gLineTrace ==true) {
            adjustPositionOut(mXAverage, mYAverage, mDirectionAverage);
        }
    	else if(mCourse == Gps::IN && gLineTrace == true){
            adjustPositionIn(mXAverage, mYAverage, mDirectionAverage);
        }

        mXAverage = 0.0;
        mYAverage = 0.0;
        mDirectionAverage = 0.0;
        mTimeCounter = 0;
    }
	mTimeCounter++;

/************ ���W�����␳�I�� *****************/

/******* prevEl,Er�̍X�V ******/
	prevEl = currEl;
	prevEr = currEr;
/**** prevEl,Er�̍X�V�̏I��****/

}

/**
 * ���s�̂̌���(��Ίp�x�A���p)���v�Z
 *
 * @param[in] angle �P�ʎ��Ԃ�����̑��s�̂̋Ȃ������p�x
 */
void Gps::calcDirection(float angle)
{
	mDirection += angle;
}

/**
 * ���s�̂̋Ȃ������p�x���v�Z
 *
 * @param[in] el �P�ʎ��Ԃ�����̍����[�^�G���R�[�_�l�ω���
 * @param[in] er �P�ʎ��Ԃ�����̉E���[�^�G���R�[�_�l�ω���
 */
float Gps::calcAngle(float el, float er)
{
	/* �X�^�[�g���̐��ʂ�0�x�Ƃ��āA�ォ�猩�Ĕ����v��肷�邽�тɒl������ */
	/* ���v���Ō��� */
	/* ���360�x */

	float angle = (mWheelRadius * (er - el)) / mWheelDistance;
	if(isinf(angle))
	{
		angle = 0;
	}

	return angle;
}

/**
 * ���s�̂̕`���~�̔��a���v�Z
 *
 * @param[in] encoderLeft �P�ʎ��Ԃ�����̍����[�^�G���R�[�_�l�ω���
 * @param[in] angle �P�ʎ��Ԃ�����̑��s�̂̋Ȃ������p�x
 */
float Gps::calcRadius(float encoderLeft, float angle)
{
	/* ����_���炠��_�ֈړ�����ۂɕ`���~�̔��a���v�Z */
	
	float gpsRadius = 0.0;
	
	if(angle > 0)
	{
		gpsRadius = ((encoderLeft * mWheelRadius) / angle) + (mWheelDistance / 2);
	}
	else if(angle < 0)
	{
		gpsRadius = ((encoderLeft * mWheelRadius / angle)) * (-1) - (mWheelDistance / 2);
	}
	else //angle = 0
	{
		gpsRadius = 0;
	}
	if(isinf(gpsRadius))
	{
		//�G���[�����B�����I�ɂ̓q�X�g���N���X���璼�߂̒l���Q�Ƃ��đ}������̂��ǂ����H
		gpsRadius = 0;
	}
	
	return gpsRadius;
}

/**
 * ���s�̂̈ړ������������v�Z
 */
void Gps::calcDistance()
{
    //// �G���R�[�_�l�x�[�X
	float el = motorL.getCount();
    float er = motorR.getCount();
    float e  = (el + er)/2.0; // ����
    mDistance = (e / 360.0) * 2.0 * mWheelRadius * M_PI;
    //// ���W�x�[�X(�o�b�N���Ă����Z����Ă��܂��B�B�B)
	// float distance;
	// float x = mXCoordinate - prevXCoordinate;
	// float y = mYCoordinate - prevYCoordinate;
	// distance = sqrt(x*x + y*y);
	// mDistance += distance;
}

/**
 * x���W�Ay���W���v�Z
 *
 * @param[in] angle �P�ʎ��Ԃ�����̑��s�̂̋Ȃ������p�x
 * @param[in] radius �ԗ֔��a
 * @param[in] encoderL �P�ʎ��Ԃ�����̍����[�^�G���R�[�_�l�ω���
 * @param[in] encoderR �P�ʎ��Ԃ�����̉E���[�^�G���R�[�_�l�ω���
 */
void Gps::calcCoordinates(float angle, float radius, float encoderL, float encoderR)
{	
	float circleX = 0.0, circleY = 0.0;
	prevXCoordinate = mXCoordinate;
	prevYCoordinate = mYCoordinate;
    
    if(encoderL*encoderR >= 0)
    {  
        /* ���s�̂̕`���~�̒��S�̍��W�����߂� */
        if(angle < 0)
        {
            circleX = mXCoordinate - radius * cos(degreeToRadian(marge360(mDirection) + 90.0));
            circleY = mYCoordinate - radius * sin(degreeToRadian(marge360(mDirection) + 90.0));
        }
        else
        {
            circleX = mXCoordinate - radius * cos(degreeToRadian(marge360(mDirection) - 90.0));
            circleY = mYCoordinate - radius * sin(degreeToRadian(marge360(mDirection) - 90.0));
        }
        
        /* ���W�̌v�Z */
        if(angle == 0)/* ���i�̏ꍇ */
        {
            mXCoordinate = mXCoordinate + (encoderL / 360.0) * 2.0 * mWheelRadius * M_PI * (cos(degreeToRadian(((int)mDirection)%360)));
            mYCoordinate = mYCoordinate + (encoderL / 360.0) * 2.0 * mWheelRadius * M_PI * (sin(degreeToRadian(((int)mDirection)%360)));
        }
        else/* �~��`���ꍇ */ 
        {
            mXCoordinate = (cos(degreeToRadian(angle))*(prevXCoordinate - circleX)) - (sin(degreeToRadian(angle))*(mYCoordinate - circleY)) + circleX;
            mYCoordinate = (sin(degreeToRadian(angle))*(prevXCoordinate - circleX)) + (cos(degreeToRadian(angle))*(mYCoordinate - circleY)) + circleY;
        }	
    }else
    {
        float rightWheelXCoordinate = 0.0;
        float rightWheelYCoordinate = 0.0;
        float leftWheelXCoordinate = 0.0;
        float leftWheelYCoordinate = 0.0;
        /* �E�ԗֈʒu���W�����߂� */
        rightWheelXCoordinate = mXCoordinate + (mWheelDistance/2.0) * cos(degreeToRadian(marge360(getDirection() - 90.0)));
        rightWheelYCoordinate = mYCoordinate + (mWheelDistance/2.0) * sin(degreeToRadian(marge360(getDirection() - 90.0)));
       	
        /* ���ԗֈʒu���W�����߂� */
        leftWheelXCoordinate = mXCoordinate - (mWheelDistance/2.0)*cos(degreeToRadian(marge360(getDirection() - 90.0)));
        leftWheelYCoordinate = mYCoordinate - (mWheelDistance/2.0)*sin(degreeToRadian(marge360(getDirection() - 90.0)));
        /* ��]���S���W�����߂�B */
        circleX = (float)(leftWheelXCoordinate + (fabs(encoderL)/(fabs(encoderL)+fabs(encoderR)))*(rightWheelXCoordinate - leftWheelXCoordinate));
        circleY = (float)(leftWheelYCoordinate + (fabs(encoderL)/(fabs(encoderL)+fabs(encoderR)))*(rightWheelYCoordinate - leftWheelYCoordinate));
        /* �ړ���̃��{�b�g�̈ʒu���W�����߂�B */
        mXCoordinate = (cos(degreeToRadian(angle))*(prevXCoordinate - circleX)) - (sin(degreeToRadian(angle))*(mYCoordinate - circleY)) + circleX;
    	mYCoordinate = (sin(degreeToRadian(angle))*(prevXCoordinate - circleX)) + (cos(degreeToRadian(angle))*(mYCoordinate - circleY)) + circleY;
    }
	if(circleX == -1.0 || circleY  == 1)
	{
		/*���S���W��inf�̏ꍇ�A�v�Z�������W���������Ȓl�ɂȂ邽�߁A�K���Ȓl�ŕ⑫ */
		/*�q�X�g���N���X���璼�߂̒l���Q�Ƃ���`���ǂ����H */
		mXCoordinate = -1.0;
		mYCoordinate = 1.0;
	}
	else{
		// if(mXCoordinate < 0)
		// {
		// 	mXCoordinate = 0;
		// }
		// if(mXCoordinate > X_RIGHT)
		// {
		// 	mXCoordinate = X_RIGHT;
		// }
		// if(mYCoordinate > 0)
		// {
		// 	mYCoordinate = 0;
		// }
		// if(mYCoordinate < Y_DOWN)
		// {
		// 	mYCoordinate = Y_DOWN;
		// }
	}
}

/**
 * X���W���擾����
 *
 * @return X���W
 */
float Gps::getXCoordinate()
{

	return mXCoordinate + mXOffset;

}

/**
 * Y���W���擾����
 *
 * @return Y���W
 */
float Gps::getYCoordinate()
{
	return mYCoordinate + mYOffset;
}

/**
 * �ԑ̌������擾����
 *
 * @return �ԑ̌���
 */
float Gps::getDirection()
{
	return mDirection + mDirectionOffset;
}

/**
 * �������擾����
 *
 * @return ����
 */
float Gps::getDistance()
{
	return mDistance + mDistanceOffset;
}


/**
 * X���W��␳����
 *
 * @param[in] trueValue �^�l
 */
void Gps::adjustXCoordinate(float trueValue)
{
	if (isinf(trueValue)) {
		if(trueValue >= 0) {
			trueValue = FLT_MAX;
		}
		else {
			trueValue = FLT_MAX*(-1);
		}
	}
    mXCoordinate = trueValue;
    // @todo: �e��v�Z�����I�t�Z�b�g�Ή��ɏC��
    //mXOffset = trueValue - mXCoordinate;
}


/**
 * Y���W��␳����
 *
 * @param[in] trueValue �^�l
 */
void Gps::adjustYCoordinate(float trueValue)
{
	if (isinf(trueValue)) {
		if(trueValue >= 0) {
			trueValue = FLT_MAX;
		}
		else {
			trueValue = FLT_MAX*(-1);
		}
	}
    mYCoordinate = trueValue;
    // @todo: �e��v�Z�����I�t�Z�b�g�Ή��ɏC��
    //mYOffset = trueValue - mYCoordinate;
}
	
/**
 * ������␳����
 *
 * @param[in] trueValue �^�l
 */
void Gps::adjustDirection(float trueValue)
{
	if (isinf(trueValue)) {
		if(trueValue >= 0) {
			trueValue = FLT_MAX;
		}
		else {
			trueValue = FLT_MAX*(-1);
		}
	}
    mDirection = trueValue;
    // @todo: �e��v�Z�����I�t�Z�b�g�Ή��ɏC��
    //mDirectionOffset = trueValue - mDirection;
}

/**
 * ������␳����
 *
 * @param[in] trueValue �^�l
 */
void Gps::adjustDistance(float trueValue)
{
	if (isinf(trueValue)) {
		if(trueValue >= 0) {
			trueValue = FLT_MAX;
		}
		else {
			trueValue = FLT_MAX*(-1);
		}
	}
    mDistance = trueValue;
    // @todo: �e��v�Z�����I�t�Z�b�g�Ή��ɏC��
    //mDistanceOffset = trueValue - mDistance;
}

/**
 * ���s�̂̕`���~�̒��S�̍��W�����߂�
 *
 * @param[in] angle �P�ʎ��Ԃ�����̑��s�̂̋Ȃ������p�x
 * @param[in] radius �ԗ֔��a
 * @param[out] circleX ���S��X���W
 * @param[out] circleY ���S��X���W
 * @retval false ��Ή��̓���
 */
bool Gps::calcCenterCoordinates(float angle, float radius, float *circleX, float *circleY)
{
	/* ���s�̂̕`���~�̒��S�̍��W�����߂� */
	float dirAngle = 0.0;
	
	if(angle < 0)
	{
		dirAngle = marge360(getDirection() + 90.0);
	}
	else if( angle >0)
	{
		dirAngle = marge360(getDirection() - 90.0);
	}
	else
	{
		return false;
	}
	*circleX = mXCoordinate - radius * cos(degreeToRadian(dirAngle));
	if(isinf(*circleX))
	{
		*circleX = -1.0;
	}
	*circleY = mYCoordinate - radius * sin(degreeToRadian(dirAngle));
	if(isinf(*circleY))
	{
		*circleY = 1.0;
	}
	return true;
}

/**
 * �A�E�g�R�[�X�����␳�֐�
 *
 * @param[in] avgX x���W��莞�ԕ��ϒl
 * @param[in] avgY y���W��莞�ԕ��ϒl
 * @param[in] avgD ������]���ԕ��ϒl
 * @author tatsuno
 */
void Gps::adjustPositionOut(float avgX,float avgY,float avgD)
{
	/**
     * ������𑖍s������GPS�̍��W����A���s�ʒu�A������␳
	 * GPSVisualizer�̍��W�𗘗p���ă}�b�s���O���s��
	 * ���W�w�葖�s������A���W�␳���s������ɁAswitch���̒��g�ǉ��B
	 * �����_�ł́A�����A��������A�ǂ�ȏꏊ�ł�������␳����͍̂���->��Ԃ����W�Ŏw�肷�邽�߁A���肪�V�r�A�ɂȂ�
	 */
    int direction = (int)marge360(avgD);
	int mDirectionDiv = direction%90;
	int posFlag = (int)(((direction + DIRECTION_THRESHOLD))/90);
	
	if(mDirectionDiv <= DIRECTION_THRESHOLD)
	{
		//
	}
	else if( mDirectionDiv >= 90 - DIRECTION_THRESHOLD)
	{
		mDirectionDiv = 90 - mDirectionDiv;
	}
	else
	{
		return;
	}
	/* ��Ԃ̏c��+-�R�[�X���A��Ԃ̉���->������ԋ��� */
	switch(posFlag){
		case 0:
			if( (((avgY >= Y_CASE_0_4 -COURSE_WIDTH) && (avgY <= Y_CASE_0_4 + COURSE_WIDTH)))  && (((avgX>=X_CASE_0_4_START) && (avgX <= X_CASE_0_4_END )) && ((getXCoordinate()>=X_CASE_0_4_START) && (getXCoordinate() <= X_CASE_0_4_END))))
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(Y_CASE_0_4);
			    adjustXCoordinate(getXCoordinate());
			}
			break;
		case 1:
			if( avgX >= X_CASE_1_5 - COURSE_WIDTH && avgX <= X_CASE_1_5 + COURSE_WIDTH  && avgY <= Y_CASE_1_5_START && avgY >= Y_CASE_1_5_END
			&& getYCoordinate() >= Y_CASE_1_5_END && getYCoordinate() <= Y_CASE_1_5_START)
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(getYCoordinate());
			    adjustXCoordinate(X_CASE_1_5);
			}
			break;
		case 2:
			if( avgY >= Y_CASE_2_6 -COURSE_WIDTH && avgY <= Y_CASE_2_6 + COURSE_WIDTH  && avgX>=X_CASE_2_6_START && avgX <= X_CASE_2_6_END && getXCoordinate()>=X_CASE_2_6_START && getXCoordinate() <= X_CASE_2_6_END)
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(Y_CASE_2_6);
			    adjustXCoordinate(getXCoordinate());
			}

			break;
		case 3:
			if( avgX >= X_CASE_3_7 -COURSE_WIDTH && avgX <= X_CASE_3_7 + COURSE_WIDTH  && avgY>=Y_CASE_3_7_END && avgY <= Y_CASE_3_7_START && getYCoordinate() <= Y_CASE_3_7_START && getYCoordinate() >= Y_CASE_3_7_END)
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(getYCoordinate());
			    adjustXCoordinate(X_CASE_3_7);
			}
			break;
		case 4:
			if( (((avgY >= Y_CASE_0_4 -COURSE_WIDTH) && (avgY <= Y_CASE_0_4 + COURSE_WIDTH)))  && (((avgX>=X_CASE_0_4_START) && (avgX <= X_CASE_0_4_END )) && ((getXCoordinate()>=X_CASE_0_4_START) && (getXCoordinate() <= X_CASE_0_4_END))))
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(Y_CASE_0_4);
			    adjustXCoordinate(getXCoordinate());
			}
			break;
		default:
			break;
	}
}

/**
 * �C���R�[�X�����␳�֐�
 *
 * @param[in] avgX x���W��莞�ԕ��ϒl
 * @param[in] avgY y���W��莞�ԕ��ϒl
 * @param[in] avgD ������]���ԕ��ϒl
 * @author tatsuno
 */
void Gps::adjustPositionIn(float avgX, float avgY, float avgD)
{
	/**
     * ������𑖍s������GPS�̍��W����A���s�ʒu�A������␳
	 * GPSVisualizer�̍��W�𗘗p���ă}�b�s���O���s��
	 * ���W�w�葖�s������A���W�␳���s������ɁAswitch���̒��g�ǉ��B
	 * �����_�ł́A�����A��������A�ǂ�ȏꏊ�ł�������␳����͍̂���->��Ԃ����W�Ŏw�肷�邽�߁A���肪�V�r�A�ɂȂ�
	 */
    int direction = (int)marge360(avgD);
	int mDirectionDiv = direction%90;
	int posFlag = (int)(((direction + DIRECTION_THRESHOLD))/90);
	
	if(mDirectionDiv <= DIRECTION_THRESHOLD)
	{
		//
	}
	else if( mDirectionDiv >= 90 - DIRECTION_THRESHOLD)
	{
		mDirectionDiv = 90 - mDirectionDiv;
	}
	else
	{
		return;
	}
	/* ��Ԃ̏c��+-�R�[�X���A��Ԃ̉���->������ԋ��� */
	switch(posFlag){
		case 0:
			if( (((avgY >= Y_IN_4 -COURSE_WIDTH) && (avgY <= Y_IN_4 + COURSE_WIDTH)))  && (((avgX>= X_IN_4_START ) && (avgX <= X_IN_4_END )) && ((getXCoordinate()>=X_IN_4_START) && (getXCoordinate() <= X_IN_4_END))))
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(Y_IN_4);
			    adjustXCoordinate(getXCoordinate());
			}
			break;
		case 1:
			break;
		case 2:
			if( avgY >= Y_IN_2_6 -COURSE_WIDTH && avgY <= Y_IN_2_6 + COURSE_WIDTH  && avgX>=X_IN_2_6_START && avgX <= X_IN_2_6_END && getXCoordinate()>=X_IN_2_6_START && getXCoordinate() <= X_IN_2_6_END)
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(Y_IN_2_6);
			    adjustXCoordinate(getXCoordinate());
			}
			break;
		case 3:
			if( avgX >= X_IN_3 -COURSE_WIDTH && avgX <= X_IN_3 + COURSE_WIDTH  && avgY>=Y_IN_3_END && avgY <= Y_IN_3_START && getYCoordinate() <= Y_IN_3_START && getYCoordinate() >= Y_IN_3_END)
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(getYCoordinate());
			    adjustXCoordinate(X_IN_3);
			}
			break;
   		case 4:
			if( (((avgY >= Y_IN_4 -COURSE_WIDTH) && (avgY <= Y_IN_4 + COURSE_WIDTH)))  && (((avgX>= X_IN_4_START ) && (avgX <= X_IN_4_END )) && ((getXCoordinate()>=X_IN_4_START) && (getXCoordinate() <= X_IN_4_END))))
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(Y_IN_4);
			    adjustXCoordinate(getXCoordinate());
			}
			break;
		default:
			break;
	}
}

//================== �N���X���\�b�h ===================
/**
 * ���[�e�B���e�B�֐�:�p�x�� [0, 360] �܂��� [0, -360]�ɗ}����
 *
 * @param[in] margeTarget �p�x
 * @return [0, 360] �܂��� [0, -360] �̑Ή�����p�x
 */
float Gps::marge360(float margeTarget)
{
    int sign = 1;//�������ʕϐ� 0�ȏ�Ȃ�1, 0�����Ȃ�-1
	float margeT = fabs(margeTarget);
	if(margeTarget >= 0)
	{
		sign = 1;
	}
	else
	{
		sign = -1;
	}
	while(margeT >= 360)
	{
		margeT -= 360;
	}
	
	margeT = (float)sign*margeT;
	
	return margeT;
}

/**
 * ���[�e�B���e�B�֐�:�p�x�� [-180, 180] �ɗ}����
 *
 * @param[in] margeTarget �p�x
 * @return [-180, 180] �̑Ή�����p�x
 */
float Gps::marge180(float margeTarget)
{
	float margeResult = margeTarget;
	while(margeResult >= 180)
    {
		margeResult -= 360;
	}
	while(margeResult < -180)
    {
		margeResult += 360;
	}
	return margeResult;
}

/**
 * ���[�e�B���e�B�֐�:�p�x(degree) �����W�A��(radian)�ɕϊ�
 *
 * @param[in] degree �p�x
 * @return ���W�A��
 */
float Gps::degreeToRadian(float degree)
{
	if(isinf(degree))
	{
		return 1001;
	}
		
	return (degree * M_PI / 180);
}

/**
 * ���[�e�B���e�B�֐�:���W�A��(radian)���p�x(degree)�ɕϊ�
 *
 * @param[in] radian ���W�A��
 * @return �p�x
 */
float Gps::radianToDegree(float radian)
{
	if(isinf(radian))
	{
		return 1000;
	}
	return (radian * 180 / M_PI);
}

/**
 * ���[�e�B���e�B�֐�:atan ���g�������� atan2 �֐�
 *
 * atan2 ���g�p����ƌł܂邱�Ƃ����邽��
 *
 * @param[in] y
 * @param[in] x
 * @return ���W�A��
 */
double Gps::atan2(double y, double x)
{
    double radian;
    if (x == 0.0) {
        if (y > 0) radian = M_PI/2.0;
        else radian = -M_PI/2.0;
    }
    else {
        radian = atan(y / x);
        if (x < 0 && y < 0) radian = (-M_PI/2.0) - radian;
        if (x < 0 && y >= 0) radian = M_PI + radian;
    }
    return radian;
}

/**
 * ���ݍ��W�Ǝw����W�Ԃ̋������v�Z����
 *
 * @param[in] targetCoordinate �w����W
 *
 * @return ����
 */
float Gps::calcDistanceTo(Point targetCoordinate)
{
    Point current =  MakePoint(getXCoordinate(), getYCoordinate());
    double dX = current.X - targetCoordinate.X;
    double dY = current.Y - targetCoordinate.Y;
    return sqrt(dX*dX + dY*dY);
}


//============== Obsolete =============
/**
 * x���W�Z�b�^
 *
 * @param[in] xCo �w��x���W
 *
 */
void Gps::setXCoordinate(float xCo)
{
    mXCoordinate = xCo;
}
/**
 * y���W�Z�b�^
 *
 * @param[in] yCo �w��y���W
 *
 */
void Gps::setYCoordinate(float yCo)
{
	mYCoordinate = yCo;
}
/**
 * direction�Z�b�^
 *
 * @param[in] direction �w�����
 *
 */
void Gps::setDirection(float direction)
{
    mDirection = direction;
}
/**
 * �����Z�b�^
 *
 * @param[in] distance �w�苗��
 *
 */
void Gps::setDistance(float distance)
{
    mDistance = distance;
}

