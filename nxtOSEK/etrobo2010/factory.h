#ifndef __FACTORY_H_
#define __FACTORY_H_

// ECRobot C++ API�̐錾
#include "TouchSensor.h"
#include "LightSensor.h"
#include "GyroSensor.h"
#include "SonarSensor.h"
#include "Motor.h"
#include "Lcd.h"
#include "Clock.h"
#include "Nxt.h"
#include "Bluetooth.h"
#include "Daq.h"
#include "Vector.h"
// GPS/�q�X�g���Ƃ��̃A�_�v�^�Q
#include "Gps.h"
#include "History.h"
// ���o
#include "Detector.h"
#include "DownSlopeDetector.h"
#include "WallDetector.h"
#include "LineDetector.h"
#include "StraightDetector.h"
#include "FailDetector.h"
// �X�L��
#include "Skill.h"
#include "LineTrace.h"
#include "AngleTrace.h"
#include "CoordinateStop.h"
#include "CoordinateTrace.h"
#include "VirtualLineTrace.h"
#include "SlowdownSkill.h"
#include "StopSkill.h"
// �h���C�o
#include "Driver.h"
#include "NormalDriver.h"
#include "SeesawDriver.h"
#include "StairwayDriver.h"
#include "OutGarageDriver.h"
#include "EnigmaDriver.h"
#include "MysteryDriver.h"
#include "InGarageDriver.h"
#include "TestDriver.h"
// �R�[�X
#include "Course.h"
#include "InCourse.h"
#include "OutCourse.h"
// ���̑�
#include "Activator.h"
#include "Pid.h"
#include "struct.h"
// �f�o�C�X�I�u�W�F�N�g
extern TouchSensor mTouchSensor;
extern SonarSensor mSonarSensor;
extern LightSensor mLightSensor;
extern GyroSensor mGyroSensor;
extern Motor mLeftMotor;
extern Motor mRightMotor;
extern Lcd mLcd;
extern Clock mClock;
extern Nxt mNxt;
extern Bluetooth mBluetooth;
extern Daq mDaq;
// GPS
extern Gps mGps;
// History
extern History mLightHistory;
extern History mRightMotorHistory;
extern History mLeftMotorHistory;
extern History mRightMotorSpeedHistory;
extern History mLeftMotorSpeedHistory;
extern History mDirectionHistory;
extern History mDirectionAverageHistory;
extern History mGyroHistory;
// ���̑�
extern Activator mActivator;
extern Pid mLightPid;
extern Pid mAnglePid;
extern Pid mSlowdownPid;
extern Pid mStopPid;
// ���o
extern WallDetector mWallDetector;
extern DownSlopeDetector mDownSlopeDetector;
extern LineDetector mLineDetector;
extern StraightDetector mStraightDetector;
extern FailDetector mFailDetector;
// Skill
extern LineTrace mLineTrace;
extern AngleTrace mAngleTrace;
extern CoordinateTrace mCoordinateTrace;
extern VirtualLineTrace mVirtualLineTrace;
extern SlowdownSkill mSlowdownSkill;
extern StopSkill mStopSkill;
extern CoordinateStop mCoordinateStop;
// Driver
extern NormalDriver mNormalDriver;
extern SeesawDriver mSeesawDriver;
extern StairwayDriver mStairwayDriver;
extern OutGarageDriver mOutGarageDriver;
extern EnigmaDriver mEnigmaDriver;
extern MysteryDriver mMysteryDriver;
extern InGarageDriver mInGarageDriver;
extern TestDriver mTestDriver;
// Course
extern Course *mCourse;
extern InCourse mInCourse;
extern OutCourse mOutCourse;
#endif
