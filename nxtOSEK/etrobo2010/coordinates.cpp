#include "factory.h"
#include "coordinates.h"

//#define INCOURSE
#define OUTCOURSE

// �R�[�X
#ifdef INCOURSE
Gps mGps(mLeftMotor, mRightMotor, Gps::IN);
//InCourse mInCourse(InCourse::TESTDRIVE); // �e�X�g�h���C�o�N��
InCourse mInCourse(InCourse::START);     // �J�n�ʒu�͉��Q��
//InCourse mInCourse(InCourse::ENIGMA);    // �J�n�ʒu�͉��Q��
//InCourse mInCourse(InCourse::MYSTERY);   // �J�n�ʒu�͉��Q��
//InCourse mInCourse(InCourse::GARAGEIN);  // �J�n�ʒu�͉��Q��
Course *mCourse(&mInCourse);
//int TRACE_EDGE = LEFT;
int TRACE_EDGE = RIGHT;
#endif
#ifdef OUTCOURSE
Gps mGps(mLeftMotor, mRightMotor, Gps::OUT);
//OutCourse mOutCourse(OutCousrse::TESTDRIVE); // �e�X�g�h���C�o�N��
OutCourse mOutCourse(OutCourse::START);      // �J�n�ʒu�͉��Q��
//OutCourse mOutCourse(OutCourse::SEESAW);     // �J�n�ʒu�͉��Q��
//OutCourse mOutCourse(OutCourse::STAIRWAY);   // �J�n�ʒu�͉��Q��
//OutCourse mOutCourse(OutCourse::GARAGEIN);   // �J�n�ʒu�͉��Q��
Course *mCourse(&mOutCourse);
int TRACE_EDGE = RIGHT;
#endif

// ���W
#ifdef INCOURSE
// �C���R�[�X�X�^�[�g���W
float GPS_COURSE_START_X = 2322.0;
float GPS_COURSE_START_Y = -525.0;
float GPS_COURSE_START_DIRECTION = 180.0;

// �G�j�O�}�X�^�[�g���W
// �C���R�[�X�K���[�W��O(�E)��
float GPS_ENIGMA_START_X = 1860.0;
float GPS_ENIGMA_START_Y = -490.0;
float GPS_ENIGMA_START_DIRECTION = 180.0;

// �~�X�e���[�T�[�N���J�n���W
// �C���R�[�X�A�A�E�g�R�[�X�V�[�\�[��}�[�J�[�I���n�_
float GPS_MYSTERY_START_X = 3000.0;
float GPS_MYSTERY_START_Y = -3150.0;
float GPS_MYSTERY_START_DIRECTION = 0.0;

// �C���R�[�X�K���[�W�E�C���J�n���W
// �C���R�[�X���O�J�[�u�̈�ԏo�������Ă��鏊
float GPS_GARAGEIN_START_X = 5019.0;
float GPS_GARAGEIN_START_Y = -957.0;
float GPS_GARAGEIN_START_DIRECTION = 90.0;

// �C���R�[�X�K���[�W���S���W
float GPS_GARAGE_X = 1737;
float GPS_GARAGE_Y = -744;
float GPS_GARAGE_DIRECTION = 270;

Region GPS_ENIGMA_START = MakeRegion(X_LEFT, Y_TOP, GPS_ENIGMA_START_X, Y_DOWN);
Region GPS_MYSTERY_START = MakeRegion(GPS_MYSTERY_START_X, Y_TOP, X_RIGHT, Y_DOWN);
Region GPS_GARAGEIN_START = MakeRegion(4500, Y_TOP, X_RIGHT, GPS_GARAGEIN_START_Y);
#endif

#ifdef OUTCOURSE
// �A�E�g�R�[�X�X�^�[�g���W
float GPS_COURSE_START_X = 2020.0;
float GPS_COURSE_START_Y = -237.0;
float GPS_COURSE_START_DIRECTION = 180.0;

// �V�[�\�[�J�n���W
// �A�E�g�R�[�X�՗��Q�{�ږڂ̑O
float GPS_SEESAW_START_X = 1200.0;
float GPS_SEESAW_START_Y = -3393.0;
float GPS_SEESAW_START_DIRECTION = 360.0;
/*
float GPS_SEESAW_START_X = 1500.0;
float GPS_SEESAW_START_Y = -3393.0;
float GPS_SEESAW_START_DIRECTION = 360.0;
*/
// �K�i�J�n���W
// �V�[�\�[�㒼���Ax���W�͗΂̓��̉E�[
//float GPS_STAIRWAY_START_X = 5000.0;
//float GPS_STAIRWAY_START_Y = -2800.0;
//float GPS_STAIRWAY_START_DIRECTION = 450.0;
float GPS_STAIRWAY_START_X = 4500.0;
float GPS_STAIRWAY_START_Y = -3400.0;
float GPS_STAIRWAY_START_DIRECTION = 360.0;

// �A�E�g�R�[�X�K���[�W�E�C���J�n���W
// �K�i�㒼���A�΂̒[
float GPS_GARAGEIN_START_X = 5238.0;
float GPS_GARAGEIN_START_Y = -900.0;
float GPS_GARAGEIN_START_DIRECTION = 450.0;

// �A�E�g�R�[�X�K���[�W���S���W
float GPS_GARAGE_X = 156.0;
float GPS_GARAGE_Y = -229.0;
float GPS_GARAGE_DIRECTION = 540.0;

//Region GPS_SEESAW_START = MakeRegion(X_LEFT, GPS_SEESAW_START_Y, X_RIGHT, Y_DOWN);
Region GPS_SEESAW_START = MakeRegion(GPS_SEESAW_START_X, -1500, X_RIGHT, Y_DOWN);
Region GPS_STAIRWAY_START = MakeRegion(GPS_STAIRWAY_START_X, Y_TOP, X_RIGHT, Y_DOWN);
Region GPS_GARAGEIN_START = MakeRegion(X_LEFT, Y_TOP, X_RIGHT, GPS_GARAGEIN_START_Y);
#endif
