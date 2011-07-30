#include "constants.h"
#include "macro.h"
#include <math.h>

//#define KACHIROBO
#define KACHIICHI
//#define KACHINEW

#ifdef KACHIROBO
char BT_NAME[16] = "KACHIROBO";
int USER_GYRO_OFFSET = 605; // ���̒l�͓K�X�������邱��
//�������
//W:510 G:615 B:697
// int WHITE = 518; // ���F�̌��Z���T�l //�����߂ɂ��Ă�
// int BLACK = 698; // ���F�̌��Z���T�l //�傫�߂ɂ��Ă�
// int LINE_THRESHOLD = 570; // ���C���[�̐F������臒l�B���ƊD�F�̊Ԃ̒l
//3F����
//W:520 G:610 B:700
//int WHITE = 510; // ���F�̌��Z���T�l //�����߂ɂ��Ă�
//int BLACK = 700; // ���F�̌��Z���T�l //�傫�߂ɂ��Ă�
//int LINE_THRESHOLD = 560; // ���C���[�̐F������臒l�B���ƊD�F�̊Ԃ̒l
//������
//W:540 G:625 B:720
//int WHITE = 540; // ���F�̌��Z���T�l //�����߂ɂ��Ă�
//int BLACK = 720; // ���F�̌��Z���T�l //�傫�߂ɂ��Ă�
//int LINE_THRESHOLD = 580; // ���C���[�̐F������臒l�B���ƊD�F�̊Ԃ̒l
//CS���~�j�z�R�[�X
//W:540 G:632 B:714
//int WHITE = 540; // ���F�̌��Z���T�l //�����߂ɂ��Ă�
//int BLACK = 720; // ���F�̌��Z���T�l //�傫�߂ɂ��Ă�
//int LINE_THRESHOLD = 586; // ���C���[�̐F������臒l�B���ƊD�F�̊Ԃ̒l
//CS���{�R�[�X
//W:525 G:617 B:675
int WHITE = 520; // ���F�̌��Z���T�l //�����߂ɂ��Ă�
int BLACK = 675; // ���F�̌��Z���T�l //�傫�߂ɂ��Ă�
int LINE_THRESHOLD = 570; // ���C���[�̐F������臒l�B���ƊD�F�̊Ԃ̒l
#endif

#ifdef KACHIICHI
char BT_NAME[16] = "KACHIICHI";
int USER_GYRO_OFFSET = 585; // ���̒l�͓K�X�������邱��
//3F����
int WHITE = 540; // ���F�̌��Z���T�l
int BLACK = 730; // ���F�̌��Z���T�l
int LINE_THRESHOLD = 600; // ���C���[�̐F������臒l�B���ƊD�F�̊Ԃ̒l
#endif

#ifdef KACHINEW
char BT_NAME[16] = "KACHINEW";
int USER_GYRO_OFFSET = 608; // ���̒l�͓K�X�������邱��
//3F����
int WHITE = 540; // ���F�̌��Z���T�l
int BLACK = 730; // ���F�̌��Z���T�l
int LINE_THRESHOLD = 600; // ���C���[�̐F������臒l�B���ƊD�F�̊Ԃ̒l
#endif

// GPS
// float WHEEL_RADIUS = 38.5; // �ԗ֔��a(mm)
// float WHEEL_DISTANCE = 168.0; // �ԗ֊Ԋu(mm)
float WHEEL_RADIUS = 40.5; // �ԗ֔��a(mm)
float WHEEL_DISTANCE = 162.0; // �ԗ֊Ԋu(mm)

// LineTrace
int INIT_FORWARD = 50;    // �����O�i���x
int INIT_SAMPLECOUNT = 200;  // �����O�i�p���J�E���^����
int FORWARD = 100;        // �O�i���x
float LIGHT_PID_KP = 150; // K_PHIDOT = 25.0F
float LIGHT_PID_KI = 0;   // K_PHIDOT = 25.0F
float LIGHT_PID_KD = 300; // K_PHIDOT = 25.0F
float LIGHT_ONOFF_K = 40; // K_PHIDOT = 25.0F
// float LIGHT_PID_KP = 40; // K_PHIDOT = 25.0F*2.5F
// float LIGHT_PID_KI = 0;   // K_PHIDOT = 25.0F*2.5F
// float LIGHT_PID_KD = 120; // K_PHIDOT = 25.0F*2.5F

//AngleTrace
int TARGETANGLE = 180;//(360*2);  //�ڕW�p�x
float ANGLE_PID_KP = 10.0; // K_PHIDOT = 25.0F
float ANGLE_PID_KI = 0.0;  // K_PHIDOT = 25.0F
float ANGLE_PID_KD = 30.0; // K_PHIDOT = 25.0F
// float ANGLE_PID_KP = 4.0; // K_PHIDOT = 25.0F*2.5F
// float ANGLE_PID_KI = 0.0;  // K_PHIDOT = 25.0F*2.5F
// float ANGLE_PID_KD = 12.0; // K_PHIDOT = 25.0F*2.5F

//SlowdownSkill
float SLOWDOWN_PID_KP = 1.0; // 100mm��O�Ńt�H���[�h�l100
float SLOWDOWN_PID_KI = 0;
float SLOWDOWN_PID_KD = 0;

//CoordinateStop
float STOP_PID_KP = 0.01; 
float STOP_PID_KI = 0.0001;
float STOP_PID_KD = 0.001;
//float STOP_PID_KP = 0.0625;
//float STOP_PID_KI = 0.001;
//float STOP_PID_KD = 0.015;

//�]�|���o
int FAILSAFE_SAMPLECOUNT = 200; // �]�|���o�̃T���v�����Ƃ���(*4ms)
int FAILSAFE_THRESHOLD = 600; //FAILSAFE_SAMPLECOUNT�����藼�ւ�����ȏ��]������]�|�Ƃ݂Ȃ�

//���m�n
float WALL_THRESHOLD = -10;
float DOWNSLOPE_THRESHOLD = 5;

//Bluetooth���O���M
int LOGGER_SEND = 0;
//0: ����Ȃ�
//1: void send(S8 dataS08[2], S32 dataS32)
//2: void send(S8 dataS08[2], U16 dataU16, S16 dataS16[4], S32 dataS32[4])
S8  LOGGER_DATAS08[2] = {0, 0};
U16 LOGGER_DATAU16 = 0;
S16 LOGGER_DATAS16[4] = {0,0,0,0};
S32 LOGGER_DATAS32[4] = {0,0,0,0};

//Activator
bool DESK_DEBUG = false; // ���[�^���񂳂Ȃ�

//LineTrace�t���O
bool gLineTrace = false;
//�u���[�L���s(������)
int PWMR = 0;
int PWML = 0;
