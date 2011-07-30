//
// Gps.h
//
#ifndef GPS_H_
#define GPS_H_

#include "Motor.h"
#include "constants.h"
#include "macro.h"
#include "struct.h"
#include <math.h>
#include <float.h>

//#define PI 3.14159265358979
#define X_LEFT  0    // ���{�R���R�[�X���x���W�ō��l
#define X_RIGHT 5450 // ���{�R���R�[�X���x���W�ŉE�l
#define Y_TOP   0    // ���{�R���R�[�X���y���W�ŏ�l
#define Y_DOWN -3600 // ���{�R���R�[�X���y���W�ŉ��l
// ex) SIGN(mTargetX - currentX) == PASSTO_LEFT
// ex) SIGN(mTargetX - currentX) == PASSTO_RIGHT
#define PASSTO_LEFT -1 // ���Ɍ������ĖڕW�n�_��ʂ�߂���
#define PASSTO_RIGHT 1 // �E�Ɍ������ĖڕW�n�_��ʂ�߂���
#define PASSTO_TOP   1 // ��Ɍ������ĖڕW�n�_��ʂ�߂���
#define PASSTO_DOWN -1 // ���Ɍ������ĖڕW�n�_��ʂ�߂���

using namespace ecrobot;

/**
 * GPS(���Ȉʒu����)�N���X
 */
class Gps
{
public:
    /**
     * �R�[�X���ʎq
     *
     * �����␳�ΏۃR�[�X�̎��ʂɗ��p
     */
    enum eCourse {
        IN, //!< �C���R�[�X
        OUT //!< �A�E�g�R�[�X
    };
private:
	float mWheelRadius; //!< �ԗ֔��a(mm)
	float mWheelDistance; //!< �ԗ֊Ԋu(mm)
	Motor &motorL; //!< �����[�^
	Motor &motorR; //!< �E���[�^
    Gps::eCourse mCourse; //!< �R�[�X���ʎq
	float mXCoordinate; //!< X���W
	float mYCoordinate; //!< Y���W
	float mDirection; //!< ����
	float mDistance; //!< ����
	float mXOffset; //!< X���W�␳�l
	float mYOffset; //!< Y���W�␳�l
	float mDirectionOffset; //!< �����␳�l
	float mDistanceOffset; //!< �����␳�l
	
	//�ȉ��␳�֌W�ϐ�
    float mXAverage; //!< X���W����
    float mYAverage; //!< Y���W����
    float mDirectionAverage; //!< ��������
	int mTimeCounter; //!< ���ԃJ�E���^
public:
	Gps(Motor &aMotorL, Motor &aMotorR, Gps::eCourse aCourse);
	~Gps(){}
	void update();
	float getXCoordinate();
	float getYCoordinate();
	float getDirection();
	float getDistance();
	void adjustXCoordinate(float trueValue);
	void adjustYCoordinate(float trueValue);
	void adjustDirection(float trueValue);
	void adjustDistance(float trueValue);
public:
	float calcAngle(float el, float er);
	float calcRadius(float encoderLeft, float angle);
	void calcDirection(float angle);
	void calcDistance();
	bool calcCenterCoordinates(float angle, float radius, float *circleX, float *circleY);
	void calcCoordinates(float angle, float distance, float encoderL, float encoderR);
private:
    // �����␳�֐�
	void adjustPositionOut(float avgX, float avgY, float avgD);
    void adjustPositionIn(float avgX, float avgY, float avgD);
public:
    // �N���X���\�b�h
	static float marge360(float margeTarget);
	static float marge180(float margeTarget);
    static float radianToDegree(float radian);
	static float degreeToRadian(float degree);
    static double atan2(double y, double x);
    float calcDistanceTo(Point targetCoordinate);
public:
    // obsolete 
	void setYCoordinate(float yCo);
	void setXCoordinate(float xCo);
    void setDirection(float direction);
    void setDistance(float distance);
};

#endif
