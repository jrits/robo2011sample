//
// Gps.cpp
//

#include "GpsTest.h"

//History�N���X������́A�e���\�b�h���Ő錾
float prevXCoordinate = 0.0;
float prevYCoordinate = 0.0;
float prevEl = 0.0;
float prevEr = 0.0;


Gps::Gps(Motor &aMotorL, Motor &aMotorR) :
	motorL(aMotorL), 
	motorR(aMotorR),
    mXCoordinate(100.0),
    mYCoordinate(-100.0),
    mDirection(180.0),
	mDistance(0.0),
	mWheelRadius(4.05),
	mXOffset(0.0),
	mWheelDistance(16.20),
	mYOffset(0.0),
	mDirectionOffset(0.0),
	mDistanceOffset(0.0),
	//�ȉ��␳�֌W�ϐ�,�����s��(�N���X�}�ɂ͂Ȃ��ϐ�)
    timeCounterHelper(0),timeCounter(0),xHistory(0),
	yHistory(0),mDirectionHistory(0),posFlag(0)
{

}

/**
 * PID����
 *
 * @param[in] P ��ᐬ��(�΍�)
 * @return �����
 */
 
void Gps::update()
{
	float angle = 0.0;  /* 4msec���Ƃ̎ԑ̊p�x�̕ω��� (�x) */
	float radius = 0.0; /* 4msec���Ƃ̎ԑ̂̕`���~�̔��a */
	float el = 0.0; /* 4msec�Ԃ̃G���R�[�_�[�l�̕ω���(left) */
	float er = 0.0; /* 4msec�Ԃ̃G���R�[�_�[�l�̕ω���(right) */
	float currEl = 0.0; /* ���݂̃G���R�[�_�[�l(left) */
	float currEr = 0.0; /* ���݂̃G���R�[�_�[�l(right) */
	timeCounter++;
	
		
/*********   �l�擾   *********/
	/* �G���R�[�_�[�l���擾 */
	motorL.setCount();
	motorR.setCount();
	currEl = motorL.getCount();
	currEr = motorR.getCount();
	
	/* 4msec�Ԃ̃G���R�[�_�[�l�̕ω��ʂ��v�Z */
	el = currEl - prevEl;
	er = currEr - prevEr;
/********* �l�擾�I�� *********/
	
/********* ���W�v�Z ***********/
	angle = calcAngle(el, er); /* �Ȃ������p�x���v�Z */
	
	radius = calcRadius(el, angle); /* ���s�̂̕`���~�̔��a���v�Z */
	
	calcCoordinates(angle, radius, el); /* ���W�̍X�V */
	
	calcDirection(angle); /* ���݌����Ă�������̍X�V */
/********* ���W�v�Z�I�� *******/

/******* prevEl,Er�̍X�V ******/
	prevEl = currEl;
	prevEr = currEr;
/**** prevEl,Er�̍X�V�̏I��****/
	
/************ ���W�␳ *****************/
	//lcd.clear();
	//lcd.putf("d", );
	//lcd .disp();
	/*
	if(timeCounter%50 !=0)
	{	
		xHistory += mXCoordinate;
		yHistory += mYCoordinate;
		mDirectionHistory += mDirection;
	}
	else
	{
		xHistory = (xHistory+mXCoordinate)/timeCounter;
		yHistory = (yHistory + mYCoordinate)/timeCounter;
		mDirectionHistory = (mDirectionHistory + mDirection)/timeCounter;
		if(!gAngleTraceFlag)
			adjustPosition(xHistory, yHistory, mDirectionHistory);
		xHistory = 0;
		yHistory = 0;
		mDirectionHistory = 0;
		timeCounter = 0;

	}
	*/
	prevEl = currEl;
	prevEr = currEr;
}

/******************************/
/* calcmDirection              */
/******************************/
/* ���s�̂̌������v�Z         */
/******************************/
void Gps::calcDirection(float angle)
{
	mDirection += angle;
}

/******************************/
/* calcAngle                  */
/******************************/
/* ���s�̂̋Ȃ������p�x���v�Z */
/******************************/
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

/******************************/
/* calcRadius                 */
/******************************/
/* ���s�̂̕`���~�̔��a���v�Z */
/******************************/
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

/******************************/
/* calcmDistance               */
/******************************/
/* ���s�̂̈ړ������������v�Z */
/*                            */
/******************************/ 
void Gps::calcDistance()
{
	
	float distance;
	float x = mXCoordinate - prevXCoordinate;
	float y = mYCoordinate - prevYCoordinate;
	distance = sqrt(x*x + y*y);
	mDistance += distance;
	if(isinf(mDistance))
	{
		//�G���[����
		//mDistance = -1;
	}
	
}

/**********************/
/* calcCoordinates    */
/**********************/
/* x���W�Ay���W���v�Z */
/**********************/
void Gps::calcCoordinates(float angle, float radius, float encoderL)
{	
	
	float x = 0;
	float y = 0;
	float *circleX = &x, *circleY = &y;
	bool result = false;
	
	/* ���s�̂̕`���~�̒��S�̍��W�����߂� */
	result = calcCircleCoordinates(angle, radius, circleX, circleY);
	
	/* ���W�̌v�Z */
	
	/* prev */
	prevXCoordinate = mXCoordinate;
	prevYCoordinate = mYCoordinate;
	if(!result)/* ���i�̏ꍇ */
	{
		mXCoordinate = prevXCoordinate + (encoderL / 360.0) * 2.0 * mWheelRadius * PI * (cos(degreeToRadian(marge360(mDirection))));
		mYCoordinate = prevYCoordinate + (encoderL / 360.0) * 2.0 * mWheelRadius * PI * (sin(degreeToRadian(marge360(mDirection))));
	}
	else/* �~��`���ꍇ */ 
	{
		mXCoordinate = (cos(degreeToRadian(angle))*(prevXCoordinate - *circleX)) - (sin(degreeToRadian(angle))*(mYCoordinate - *circleY)) + *circleX;
		mYCoordinate = (sin(degreeToRadian(angle))*(prevXCoordinate - *circleX)) + (cos(degreeToRadian(angle))*(mYCoordinate - *circleY)) + *circleY;
	}
	if(*circleX == -1.0 || *circleY  == 1)
	{
		//���S���W��inf�̏ꍇ�A�v�Z�������W���������Ȓl�ɂȂ邽�߁A�K���Ȓl�ŕ⑫
		//�q�X�g���N���X���璼�߂̒l���Q�Ƃ���`���ǂ����H
		mXCoordinate = -1.0;
		mYCoordinate = 1.0;
	}
	else{
		if(mXCoordinate <0)
		{
			mXCoordinate = 0;
		}
		if(mXCoordinate >545)
		{
			mXCoordinate = 545;
		}
		if(mYCoordinate >0)
		{
			mYCoordinate = 0;
		}
		if(mYCoordinate < -360)
		{
			mYCoordinate = -360;
		}
	}
}

float Gps::degreeToRadian(float degree)
{
	if(isinf(degree))
	{
		return 1001;
	}
		
	return (degree * M_PI / 180);
}

float Gps::radianToDegree(float radian)
{
	if(isinf(radian))
	{
		return 1000;
	}
	return (radian * 180 / M_PI);
}

float Gps::getXCoordinate()
{
	return mXCoordinate;
}

float Gps::getYCoordinate()
{
	return mYCoordinate;
}

float Gps::getDirection()
{
	return mDirection;
}

float Gps::getDistance()
{
	return mDistance;
}



void Gps::adjustXCoordinate(float offset)
{
	if(isinf(offset))
	{
		if(offset >=0)
		{
			mXOffset = FLT_MAX;
		}
		else
		{
			mXOffset = FLT_MAX*(-1);
		}
	}
	else
	{
		mXOffset = mXCoordinate + offset;
	}
}


void Gps::adjustYCoordinate(float offset)
{
	if(isinf(offset))
	{
		if(offset >=0)
		{
			mYOffset = FLT_MAX;
		}
		else
		{
			mYOffset = FLT_MAX*(-1);
		}
	}
	else
	{
		mYOffset = mYCoordinate + offset;
	}
	
}
	
void Gps::adjustDirection(float offset)
{
	if(isinf(offset))
	{
		if(offset >= 0)
		{
			mDirectionOffset = FLT_MAX;
		}
		else
		{
			mDirectionOffset = FLT_MAX*(-1);
		}
		
	}
	else
	{
		mDirectionOffset = mDirection + offset;
	}
	
}

void Gps::adjustDistance(float offset)
{
	if(isinf(offset))
	{
		if(offset >=0)
		{
			mDistanceOffset = FLT_MAX;
		}
		else
		{
			mDistanceOffset = FLT_MAX*(-1);
		}
		
	}
	else
	{
		mDistanceOffset = mDistance + offset;
	}
}

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

bool Gps::calcCircleCoordinates(float angle, float radius, float *circleX, float *circleY)
{
	/* ���s�̂̕`���~�̒��S�̍��W�����߂� */
	float dirAngle = 0.0;
	
	if(angle < 0)
	{
		dirAngle = marge360(mDirection + 90.0);
	}
	else if( angle >0)
	{
		dirAngle = marge360(mDirection - 90.0);
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
//�������������ȉ��N���X�}�ɂȂ��������i�K�ō쐬�����֐�,�����s����������������������
void Gps::adjustPosition(float avgX,float avgY,float avgD)
{
	/*������𑖍s������GPS�̍��W����A���s�ʒu�A������␳*/
	/*GPSVisualizer�̍��W�𗘗p���ă}�b�s���O���s��*/
	//���W�w�葖�s������A���W�␳���s������ɁAswitch���̒��g�ǉ��B
	//�����_�ł́A�����A��������A�ǂ�ȏꏊ�ł�������␳����͍̂���->��Ԃ����W�Ŏw�肷�邽�߁A���肪�V�r�A�ɂȂ�
	/**/
	avgY *= (-1);	//���W�������]
	int mDirectionDiv = (int)avgD%90;
	//int posFlag = (int)(((avgD + dirThresh))/90);
	posFlag = (int)(((avgD + dirThresh))/90);
	
	if(mDirectionDiv <= dirThresh)
	{
		//
	}
	else if( mDirectionDiv >= 90 - dirThresh)
	{
		mDirectionDiv = 90 - mDirectionDiv;
	}
	else
	{
		return;
	}
	//��Ԃ̏c��+-�R�[�X���A��Ԃ̉���->������ԋ���
	switch(posFlag){
		case 0:
			if( (((avgY >= 340 -courseWidth) && (avgY <= 340 + courseWidth)))  && (((avgX>=118.2) && (avgX <= 419.1 )) && ((mXCoordinate>=118.2) && (mXCoordinate <= 419.1))))
			{
//				beep(200);
				mDirection = 90 * posFlag;
				mYCoordinate = -340.0;
			}
			break;
		case 4:
			if( (((avgY >= 340 -courseWidth) && (avgY <= 340 + courseWidth)))  && (((avgX>=118.2) && (avgX <= 419.1 )) && ((mXCoordinate>=118.2) && (mXCoordinate <= 419.1))))
			{
			//	if(mXCoordinate > 260 && mXCoordinate < 419)beep(200);
				mDirection = 90 * posFlag;
				mYCoordinate = -340.0;
			}
			break;
		
		case 1:
			if( avgX >= 522.6 - courseWidth && avgX <= 522.6 + courseWidth  && avgY <= 270.3 && avgY >= 133.2
			&& mYCoordinate >= -293.0 && mYCoordinate <= -100)
			{
//				beep(210);
				mDirection = 90 * posFlag;
				mXCoordinate = 522.6;
			}
			break;
		case 5:
			if( avgX >= 522.6 -20 - courseWidth && avgX <= 522.6 +40 + courseWidth  
				&& avgY <= 290.0 && avgY >= 133.2 && mYCoordinate >= -293.0 && mYCoordinate <= -100)
			{
//				beep(210);
				mDirection = 90 * posFlag;
				mXCoordinate = 522.6;
			}
			break;
		case 2:
			if( avgY >= 23.1 -courseWidth && avgY <= 23.1 + courseWidth  && avgX>=150.6 && avgX <= 405.6 && mXCoordinate>=150.6 && mXCoordinate <= 405.6)
			{
//				beep(220);
				mDirection = 90 * posFlag;
				mYCoordinate = -23.1;
			}

			break;
		case 6:
			if( avgY >= 23.1 -courseWidth && avgY <= 23.1 + courseWidth  && avgX>=150.6 && avgX <= 405.6 && mXCoordinate>=150.6 && mXCoordinate <= 405.6)
			{
//				beep(220);
				mDirection = 90 * posFlag;
				mYCoordinate = -23.1;
			}

			break;
		case 3:
			if( avgX >= 17.0 -courseWidth && avgX <= 17.0 + courseWidth  && avgY>=131.1 && avgY <= 218.1 && mYCoordinate <= -131.1 && mYCoordinate >= -218.1)
			{
//				beep(230);
				mDirection = 90 * posFlag;
				mXCoordinate = 17.0;
			}
			break;
		case 7:
			if( avgX >= 17.0 -courseWidth && avgX <= 17.0 + courseWidth  && avgY>=131.1 && avgY <= 218.1 && mYCoordinate <= -131.1 && mYCoordinate >= -218.1)
			{
//				beep(230);
				mDirection = 90 * posFlag;
				mXCoordinate = 17.0;
			}
			break;
		/*case 4:
			if( avgY >= 340 -courseWidth && avgY <= 340 + courseWidth  && avgX>=118.2 && avgX <= 419.1 && mXCoordinate>=118.2 && mXCoordinate <= 419.1)
			{
				mDirection = 90 * posFlag;
				mYCoordinate = -340.0;
			}
			break;
		*/
		default:
			break;
	}
	
}

float Gps::getMR()
{
	return motorR.getCount();
}
float Gps::getML()
{
	return motorL.getCount();
}
void Gps::setXCoordinate(float xCo)
{
    mXCoordinate = xCo;
}
void Gps::setYCoordinate(float yCo)
{
	mYCoordinate = yCo;
}

float Gps::getXOffset()
{
	return mXOffset;
}
float Gps::getYOffset()
{
	return mYOffset;
}

float Gps::getDirectionOffset()
{
	return mDirectionOffset;
}
float Gps::getDistanceOffset()
{
	return mDistanceOffset;
}
void Gps::setDirection(float dir)
{
	mDirection = dir;
}
void Gps::setDistance(float distance)
{
	mDistance = distance;
}

