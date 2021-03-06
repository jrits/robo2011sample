#ifndef IN_TWINLOOP_COURSE_H_
#define IN_TWINLOOP_COURSE_H_

#include "Course.hpp"
#include "TwinloopSection.hpp"
extern TwinloopSection twinloopSection;

/**
 * Course Manager
 *
 * @todo In Course would be an object of a course. 
 */
class InTwinloopCourse : public Course
{
public:
    enum SectionId {
        START,
        BEFORE_TWINLOOP,
        TWINLOOP,
        AFTER_TWINLOOP,
        AFTER_FIRST360,
        END
    };

public:
    InTwinloopCourse(Direction &g, Distance &d, Time &t,
              SectionId si = InTwinloopCourse::START,
              CourseId ci = Course::IN
        ) : Course(g, d, t, ci) {
        this->sectionId = si;
    };
    ~InTwinloopCourse() {};

public:
    Section* detect()
    {
        int nextSectionId = this->sectionId; ///<今回決定する状態。とりあえず前回と同じ状態に
        Section* nextSection = 0;
        static int flagCount = 0;
        static int globalInit = false;
        bool doneOffset180 = false;

        /// 車体向き認識の調整
        if (this->sectionId == InTwinloopCourse::START) {
            direction.calcDirectionOffset90();
        }
        if (this->sectionId == InTwinloopCourse::START) {
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

        if (this->sectionId == InTwinloopCourse::START) {
            nextSection = &normalLineSection;

            // ドルフィン走行を抜けた後、直線180度検出でツインループ走行へ
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

            if (distance.computeDistance() > TWINLOOP_STRATEGY_LINETRACE_DISTANCE) {
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

            // ツインループ走行を抜ける
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

            // 一度目の360度状態を抜ける
            countFlag(flagCount,newAvgDirectionOff < 210);
            if (flagCount >= 20) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InDolphinCourse::AFTER_FIRST360;
                normalLineSection.setSpeed(SPEED); // 速度を戻す
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InDolphinCourse::AFTER_FIRST360) {
            nextSection = &normalLineSection;

            // ２度目の360度直線検出でゴール前直線へ
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
};

#endif // IN_TWINLOOP_COURSE_H_

