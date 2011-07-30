#ifndef IN_OUT_IN_COURSE_H_
#define IN_OUT_IN_COURSE_H_

#include "Course.hpp"

/**
 * Course Manager
 * 
 * @todo Out Course would be an object of a course. 
 */
class InOutInCourse : public Course
{
public:
    enum SectionId {
        START,
        NEW_DASHED,
        AFTER_NEW_DASHED,
        TREASURE,
        AFTER_TREASURE,
        END
    };

public:
    InOutInCourse(Direction &g, Distance &d, Time &t,
              SectionId si = InOutInCourse::START,
              CourseId ci = Course::IN
        ) : Course(g, d, t, ci) {
        this->sectionId = si;
    };
    ~InOutInCourse() {};

public:
    Section* detect()
    {
        int nextSectionId = this->sectionId; ///<���񌈒肷���ԁB�Ƃ肠�����O��Ɠ�����Ԃ�
        Section* nextSection = 0;
        static int flagCount = 0;

        static int globalInit    = false;
        static int newDashedInit = false;
        static int treasureInit  = false;
        static int endInit       = false;

        /// �ԑ̌����F���̒���
        if (this->sectionId == InOutInCourse::START) {
            direction.calcDirectionOffset90();
        }
        if (this->sectionId == InOutInCourse::AFTER_NEW_DASHED) {
            direction.calcDirectionOffset180();
        }

        F32 directionOff = direction.getDirectionOff();
        F32 newAvgDirectionOff = direction.getNewAvgDirectionOff();
        F32 derivativeOfDirection = direction.getDerivativeOfDirection();

#if PRINT_STATE
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

        if (this->sectionId == InOutInCourse::START) {
            nextSection = &normalLineSection;

            // 180�x���o�Ŕj�����s��
            countFlag(flagCount,newAvgDirectionOff > NEW_DASHED_LINE_SECTION_TRIGER_DEGREE);
            if (flagCount >= 5
                && newAvgDirectionOff >= NEW_DASHED_LINE_SECTION_TRIGER_RAW_DEGREE_LOWER
                && newAvgDirectionOff <= NEW_DASHED_LINE_SECTION_TRIGER_RAW_DEGREE_UPPER
                
                // && directionOff >= DASHED_LINE_STRATEGY_TRIGER_RAW_DEGREE_LOWER
                // && directionOff <= DASHED_LINE_STRATEGY_TRIGER_RAW_DEGREE_UPPER
                ) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InOutInCourse::NEW_DASHED;
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InOutInCourse::NEW_DASHED) {
            if (newDashedInit == false) {
                newDashedLineSection.initialize();
                newDashedInit = true;
            }
            nextSection = &newDashedLineSection;

            // �j�����s�𔲂���
            if (nextSection->isEnd()) { 
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InOutInCourse::AFTER_NEW_DASHED;
                distance.reset();
                flagCount = 0;
                
                normalLineSection.initialize();
            }
        } else if (this->sectionId == InOutInCourse::AFTER_NEW_DASHED) {
            // normalLineSection.setEdge(RIGHT);
            nextSection = &normalLineSection;

            // �j�����s�𔲂�����A����270�x���o�Ńg���W���[�n���g���s��
            countFlag(flagCount, newAvgDirectionOff > TREASURE_HUNT_STRATEGY_TRIGER_DEGREE
                      && std::abs(derivativeOfDirection) < TREASURE_HUNT_STRATEGY_TRIGER_DEGREE_DERIVATIVE_BOUNDS);
//            countFlag(flagCount, newAvgDirectionOff > TREASURE_HUNT_STRATEGY_TRIGER_DEGREE);
            
            if (flagCount >= TREASURE_HUNT_STRATEGY_TRIGER_FLAGCOUNT
                // && directionOff >= TREASURE_HUNT_STRATEGY_TRIGER_RAW_DEGREE_LOWER
                // && directionOff <= TREASURE_HUNT_STRATEGY_TRIGER_RAW_DEGREE_UPPER
                ) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InOutInCourse::TREASURE;
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InOutInCourse::TREASURE) {
            if (treasureInit == false) {
                treasureHuntSection.initialize();
                treasureInit = true;
            }
            nextSection = &treasureHuntSection;

            // �g���W���[�n���g���s�𔲂���
            if (nextSection->isEnd()) { 
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InOutInCourse::AFTER_TREASURE;
                distance.reset();
                flagCount = 0;
                normalLineSection.initialize();
            }
        } else if (this->sectionId == InOutInCourse::AFTER_TREASURE) {
            // normalLineSection.setEdge(RIGHT);
            nextSection = &normalLineSection;

            // �g���W���[�n���g�𔲂�����A360�x�������o�ŃS�[���O������
            countFlag(flagCount, newAvgDirectionOff > GOAL_STOP_STRATEGY_TRIGER_DEGREE
                      && std::abs(derivativeOfDirection) < GOAL_STOP_STRATEGY_TRIGER_DEGREE_DERIVATIVE_BOUNDS);
            if (flagCount >= GOAL_STOP_STRATEGY_TRIGER_FLAGCOUNT) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InOutInCourse::END;
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InOutInCourse::END) {
            if (endInit == false) {
                newGoalStopSection.initialize();
                newGoalStopSection.setEdge(RIGHT);
                newGoalStopSection.setDistanceToGoal(OUT_DISTANCE_TO_GOAL);
                newGoalStopSection.setSpeed(GOAL_STOP_STRATEGY_SPEED);
                endInit = true;
            }
            nextSection = &newGoalStopSection;
        }

        this->sectionId = nextSectionId;
        return nextSection;
    };
    
};

#endif // IN_OUT_IN_COURSE_H_

