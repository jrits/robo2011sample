#ifndef IN_COURSE_H_
#define IN_COURSE_H_

#include "Course.hpp"

/**
 * Course Manager
 *
 * @todo In Course would be an object of a course. 
 */
class InCourse : public Course
{
public:
    enum SectionId {
        START,
        FIRST360,
        AFTER_FIRST360,
        END
    };

public:
    InCourse(Direction &g, Distance &d, Time &t,
              SectionId si = InCourse::START,
              CourseId ci = Course::IN
        ) : Course(g, d, t, ci) {
        this->sectionId = si;
    };
    ~InCourse() {};

public:
    Section* detect()
    {
        int nextSectionId = this->sectionId; ///<���񌈒肷���ԁB�Ƃ肠�����O��Ɠ�����Ԃ�
        Section* nextSection = 0;
        static int flagCount = 0;

        static int globalInit = false;
        static int endInit   = false;

        /// �ԑ̌����F���̒���
        if (this->sectionId == InCourse::START) {
            direction.calcDirectionOffset90();
        }
        if (this->sectionId == InCourse::START) {
            direction.calcDirectionOffset180();
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

        if (this->sectionId == InCourse::START) {
            nextSection = &normalLineSection;

            // �Q�x�ڂ�360�x���o�ŃS�[���O�����ƂȂ邽�߁A�P�x�ڂ͕ʂ̏�Ԃ�
            countFlag(flagCount,newAvgDirectionOff > GOAL_STOP_STRATEGY_TRIGER_DEGREE);
            if (flagCount >= 20) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InCourse::FIRST360;
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InCourse::FIRST360) {
            nextSection = &normalLineSection;

            // ��x�ڂ�360�x��Ԃ𔲂���
            countFlag(flagCount,newAvgDirectionOff < 210);
            if (flagCount >= 20) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InCourse::AFTER_FIRST360;
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InCourse::AFTER_FIRST360) {
            nextSection = &normalLineSection;

            // �Q�x�ڂ�360�x�������o�ŃS�[���O������
            countFlag(flagCount, newAvgDirectionOff > GOAL_STOP_STRATEGY_TRIGER_DEGREE
                      && std::abs(derivativeOfDirection) < GOAL_STOP_STRATEGY_TRIGER_DEGREE_DERIVATIVE_BOUNDS);
            if (flagCount >= GOAL_STOP_STRATEGY_TRIGER_FLAGCOUNT) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InCourse::END;
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InCourse::END) {
            if (endInit == false) {
                goalStopSection.initialize();
                goalStopSection.setEdge(RIGHT);
                goalStopSection.setDistanceToGoal(IN_DISTANCE_TO_GOAL);
                goalStopSection.setSpeed(GOAL_STOP_STRATEGY_SPEED);
                endInit = true;
            }
            nextSection = &goalStopSection;
        }

        this->sectionId = nextSectionId;
        return nextSection;
    };
    
};

#endif // IN_COURSE_H_

