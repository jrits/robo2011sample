#include "InDolphinCourse.hpp"

extern SensorHistory history;

Section* InDolphinCourse::detect()
{
    {
        int nextSectionId = this->sectionId; ///<���񌈒肷���ԁB�Ƃ肠�����O��Ɠ�����Ԃ�
        Section* nextSection = 0;
        static int flagCount = 0;
        static int globalInit = false;
        bool doneOffset180 = false;

        /// �ԑ̌����F���̒���
        if (this->sectionId == InDolphinCourse::START) {
            direction.calcDirectionOffset90();
        }
        if (this->sectionId == InDolphinCourse::AFTER_DOLPHIN) {
            doneOffset180 = direction.calcDirectionOffset180();
        }

        F32 newAvgDirectionOff = direction.getNewAvgDirectionOff();
        F32 derivativeOfDirection = direction.getDerivativeOfDirection();

#if PRINT_STATE
        F32 directionOff = direction.getDirectionOff();
        lcd.clear();
        lcd.putf("sdn", "Raw", (S32)directionOff);
        lcd.putf("sdn", "Avg", (S32)newAvgDirectionOff);
        lcd.putf("sdn", "SID", this->sectionId);
        lcd.putf("sdn", "Cnt", flagCount);
        lcd.disp();
#endif

        if (globalInit) {
            normalLineSection.setEdge(RIGHT);
            normalLineSection.setSpeed(SPEED);
            globalInit = false;
        }

        if (this->sectionId == InDolphinCourse::START) {
            nextSection = &normalLineSection;

            // 270�x���o�Ńh���t�B�����s��
            countFlag(flagCount,newAvgDirectionOff > DOLPHIN_STRATEGY_TRIGER_DEGREE);
            if (flagCount >= DOLPHIN_STRATEGY_TRIGER_FLAGCOUNT) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InDolphinCourse::DOLPHIN;
                dolphinSection.initialize();
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InDolphinCourse::DOLPHIN) {
            nextSection = &dolphinSection;

            // �h���t�B�����s�𔲂���
            if (dolphinSection.isEnd()) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InDolphinCourse::AFTER_DOLPHIN;
                normalLineSection.initialize();
                normalLineSection.setEdge(RIGHT);
                normalLineSection.setBootSpeed(DOLPHIN_STRATEGY_LANDING_BOOT_SPEED);
                normalLineSection.setBootTime(DOLPHIN_STRATEGY_LANDING_BOOT_TIME);
                normalLineSection.setSpeed(DOLPHIN_STRATEGY_LANDING_SPEED);
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InDolphinCourse::AFTER_DOLPHIN) {
            nextSection = &normalLineSection;

            // �h���t�B�����s�𔲂�����A����180�x���o�Ńc�C�����[�v���s��
            //countFlag(flagCount, newAvgDirectionOff > TWINLOOP_STRATEGY_TRIGER_DEGREE
            //&& std::abs(derivativeOfDirection) < TWINLOOP_STRATEGY_TRIGER_DEGREE_DERIVATIVE_BOUNDS);
            //if (flagCount >= TWINLOOP_STRATEGY_TRIGER_FLAGCOUNT) {
            if (doneOffset180) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InDolphinCourse::BEFORE_TWINLOOP;
                normalLineSection.setSpeed(60);
                //normalLineSection.initialize(); // DO NOT
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InDolphinCourse::BEFORE_TWINLOOP) {
            nextSection = &normalLineSection;

            if (distance.computeDistance() > TWINLOOP_STRATEGY_LINETRACE_DISTANCE_FOR_DOLPHIN) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InDolphinCourse::TWINLOOP;
                twinloopSection.initialize();
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InDolphinCourse::TWINLOOP) {
            nextSection = &twinloopSection;

            // �c�C�����[�v���s�𔲂���
            if (twinloopSection.isEnd()) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InDolphinCourse::AFTER_TWINLOOP;
                normalLineSection.initialize();
                normalLineSection.setBootSpeed(TWINLOOP_STRATEGY_LANDING_BOOT_SPEED);
                normalLineSection.setBootTime(TWINLOOP_STRATEGY_LANDING_BOOT_TIME);
                normalLineSection.setBootEdge(TWINLOOP_STRATEGY_LANDING_BOOT_EDGE);
                normalLineSection.setSpeed(TWINLOOP_STRATEGY_LANDING_SPEED);
                normalLineSection.setEdge(TWINLOOP_STRATEGY_LANDING_EDGE);
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InDolphinCourse::AFTER_TWINLOOP) {
            nextSection = &normalLineSection;

            // ��x�ڂ�360�x��Ԃ𔲂���
            countFlag(flagCount,newAvgDirectionOff < 210);
            if (flagCount >= 20) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InDolphinCourse::AFTER_FIRST360;
                normalLineSection.setSpeed(SPEED); // ���x��߂�
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InDolphinCourse::AFTER_FIRST360) {
            nextSection = &normalLineSection;

            // �Q�x�ڂ�360�x�������o�ŃS�[���O������
            countFlag(flagCount, newAvgDirectionOff > GOAL_STOP_STRATEGY_TRIGER_DEGREE
                      && std::abs(derivativeOfDirection) < GOAL_STOP_STRATEGY_TRIGER_DEGREE_DERIVATIVE_BOUNDS);
            if (flagCount >= GOAL_STOP_STRATEGY_TRIGER_FLAGCOUNT) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InDolphinCourse::END;
                goalStopSection.initialize();
                goalStopSection.setEdge(RIGHT);
                goalStopSection.setDistanceToGoal(IN_DISTANCE_TO_GOAL);
                goalStopSection.setSpeed(GOAL_STOP_STRATEGY_SPEED);
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InDolphinCourse::END) {
            nextSection = &goalStopSection;
        }

        this->sectionId = nextSectionId;
        return nextSection;
    };
    
}
