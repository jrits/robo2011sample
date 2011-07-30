#ifndef STRUCT_H_
#define STRUCT_H_

#include <math.h>

/**
 * �_
 */
typedef struct _TAGPOINT{
	float X; //!< X���W �P��mm ���_�̓R�[�X���[ �E�ɍs���ɂ��+
	float Y; //!< Y���W �P��mm ���_�̓R�[�X��[ ���ɍs���ɂ��-
} Point;

/**
 * GpsVisualizer����_
 */
typedef struct _TAGCONTROLPOINT {
    float X;              //!< �ڕWX���W(mm)�B���W�w�葖�s���Ȃ��ꍇ��FP_NAN���w��B
    float Y;              //!< �ڕWY���W(mm)�B���W�w�葖�s���Ȃ��ꍇ��FP_NAN���w��B
    float direction;      //!< �ڕW�����B�����]�����Ȃ��ꍇ��FP_NAN���w��B
    float forward;        //!< ���W�w�葖�s�p�B�t�H���[�h�l�B
    float allowableError; //!< ���p�B���e�덷(mm)�B
    bool  slowdown;       //!< ���W�w�葖�s�p�BSlowdownSkill ���g�p���邩�ǂ����B
} ControlPoint;

/**
 * �̈�
 */
typedef struct _TAGREGION {
    float startX; //!< �n�_X(����)
    float startY; //!< �n�_Y(����)
    float endX;   //!< �I�_X(�E��)
    float endY;   //!< �I�_Y(�E��)
} Region;

/**
 * GPS�l
 */
typedef struct _TAGGPSVALUE {
    float X;         //!< x���W
    float Y;         //!< y���W
    float direction; //!< �ԑ̌���
    float distance;  //!< ����
} GpsInfo;

/**
 * �����␳���\����
 */
typedef struct _TAGGPSOFFSET {
    Region region;   //!< ���̗̈�ɓ�������␳���s��
    GpsInfo gpsInfo; //!< NaN�̃f�[�^�͕␳�ΏۊO
} GpsOffset;

//=============== Utility �֐�(�����I�Ƀt�@�C���𕪂���\������) ==========

/**
 * Point ���쐬����
 *
 * @param[in] X x���W(mm)�B���_�̓R�[�X���[�B�E�ɍs���ɂ��+
 * @param[in] Y y���W(mm)�B���_�̓R�[�X��[�B���ɍs���ɂ��-
 * @return point �_
 */
Point MakePoint(float X, float Y)
{
    Point point = {X, Y};
    return point;
}

/**
 * Region ���쐬����
 *
 * @param[in] startX �n�_X(����)
 * @param[in] startY �n�_Y(����)
 * @param[in] endX �I�_X(����)
 * @param[in] endY �I�_Y(����)
 * @return �̈�
 */
Region MakeRegion(float startX, float startY, float endX, float endY)
{
    Region region = {startX, startY, endX, endY};
    return region;
}

/**
 * GpsInfo ���쐬����
 *
 * @param[in] X x���W
 * @param[in] Y y���W
 * @param[in] direction �ԑ̌���
 * @param[in] distance ����
 * @return GpsInfo
 */
GpsInfo MakeGpsInfo(float X, float Y, float direction, float distance)
{
    GpsInfo gpsInfo = {X, Y, direction, distance};
    return gpsInfo;
}

/**
 * �w�肵���_���̈���ɑ��݂��邩���肷��
 *
 * @param[in] aRegion �̈�
 * @param[in] aPoint  �_
 *
 * @retval true �̈��
 * @retval false �̈�O
 */
bool inRegion(Region aRegion, Point aPoint)
{
    return (aRegion.startX <= aPoint.X && aPoint.X <= aRegion.endX &&
            aRegion.endY <= aPoint.Y && aPoint.Y <= aRegion.startY);
}

/**
 * �Q�_�Ԃ̋������v�Z����
 *
 * @param[in] a �_A
 * @param[in] b �_B
 *
 * @return ����
 */
double calcDistance(Point a, Point b)
{
    return sqrt(pow(a.X - b.X, 2) + pow(a.Y - b.Y, 2));
}

#endif
