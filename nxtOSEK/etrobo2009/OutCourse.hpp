#ifndef OUT_COURSE_H_
#define OUT_COURSE_H_

#include "Course.hpp"

/**
 * Course Manager
 * 
 * @todo Out Course would be an object of a course. 
 */
class OutCourse : public Course
{
public:
    enum SectionId {
        START,
        DASHED,
        AFTER_DASHED,
        TREASURE,
        AFTER_TREASURE,
        END
    };

public:
    OutCourse(Direction &g, Distance &d, Time &t,
              SectionId si = OutCourse::START,
              CourseId ci = Course::OUT
        ) : Course(g, d, t, ci) {
        this->sectionId = si;
    };
    ~OutCourse() {};

public:
    Section* detect()
    {
        int nextSectionId = this->sectionId; ///<���񌈒肷���ԁB�Ƃ肠�����O��Ɠ�����Ԃ�
        Section* nextSection = 0;
        static int flagCount = 0;

        static int globalInit   = false;
        static int dashedInit   = false;
        static int treasureInit = false;
        static int endInit      = false;

        /// �ԑ̌����F���̒���
        if (this->sectionId == OutCourse::START) {
            direction.calcDirectionOffset90();
        }
        if (this->sectionId == OutCourse::AFTER_DASHED) {
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

        if (this->sectionId == OutCourse::START) {
            nextSection = &normalLineSection;

            // 180�x���o�Ŕj�����s��
            countFlag(flagCount,newAvgDirectionOff > DASHED_LINE_STRATEGY_TRIGER_DEGREE);
            if (flagCount >= 5
                && directionOff >= DASHED_LINE_STRATEGY_TRIGER_RAW_DEGREE_LOWER
                && directionOff <= DASHED_LINE_STRATEGY_TRIGER_RAW_DEGREE_UPPER) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = OutCourse::DASHED;
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == OutCourse::DASHED) {
            if (dashedInit == false) {
                dashedLineSection.initialize();
                dashedInit = true;
            }
            nextSection = &dashedLineSection;

            // �j�����s�𔲂���
            //if (isDashedLineSectionFinished) { 
            if (nextSection->isEnd()) { 
                normalLineSection.initialize(); // �X�s�[�h���x����Ԃ��獂���ɂ���Ɠ|���̂ŏ�����
                
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = OutCourse::AFTER_DASHED;
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == OutCourse::AFTER_DASHED) {
            // normalLineSection.setEdge(RIGHT);
            nextSection = &normalLineSection;

            // �j�����s�𔲂�����A����270�x���o�Ńg���W���[�n���g���s��
            countFlag(flagCount, newAvgDirectionOff > TREASURE_HUNT_STRATEGY_TRIGER_DEGREE
                      && std::abs(derivativeOfDirection) < TREASURE_HUNT_STRATEGY_TRIGER_DEGREE_DERIVATIVE_BOUNDS);
            
            if (flagCount >= TREASURE_HUNT_STRATEGY_TRIGER_FLAGCOUNT) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = OutCourse::OutCourse::TREASURE;
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == OutCourse::TREASURE) {
            if (treasureInit == false) {
                treasureHuntSection.initialize();
                treasureInit = true;
            }
            nextSection = &treasureHuntSection;

            // �g���W���[�n���g���s�𔲂���
            //if (isTreasureHuntSectionFinished) { 
            if (nextSection->isEnd()) { 
                normalLineSection.initialize(); // �X�s�[�h���x����Ԃ��獂���ɂ���Ɠ|���̂ŏ�����
                
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = OutCourse::AFTER_TREASURE;
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == OutCourse::AFTER_TREASURE) {
            // normalLineSection.setEdge(RIGHT);
            nextSection = &normalLineSection;

            // �g���W���[�n���g�𔲂�����A360�x�������o�ŃS�[���O������
            countFlag(flagCount, newAvgDirectionOff > GOAL_STOP_STRATEGY_TRIGER_DEGREE
                      && std::abs(derivativeOfDirection) < GOAL_STOP_STRATEGY_TRIGER_DEGREE_DERIVATIVE_BOUNDS);
            if (flagCount >= GOAL_STOP_STRATEGY_TRIGER_FLAGCOUNT) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = OutCourse::END;
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == OutCourse::END) {
            if (endInit == false) {
                goalStopSection.initialize();
                goalStopSection.setEdge(RIGHT);
                goalStopSection.setDistanceToGoal(OUT_DISTANCE_TO_GOAL);
                goalStopSection.setSpeed(GOAL_STOP_STRATEGY_SPEED);
                endInit = true;
            }
            nextSection = &goalStopSection;
        }

        this->sectionId = nextSectionId;
        return nextSection;
    };
    
};

#endif // OUT_COURSE_H_

