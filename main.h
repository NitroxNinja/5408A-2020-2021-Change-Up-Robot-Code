#ifndef MAIN_H
#define MAIN_H

//from below here is the methods from opCpmtrp.cpp

void joystickControl(void);

void intakeControl(void);

void indexSystem(void);

//from below here is the methods from functions.cpp 

void whackTheBall(bool spinIndex);

void pivotLeft(float L);

void pivotRight(float R);

void driveSetVelocity(double speed);

void rotateDrive(float count);

void deploy(void);

void shoot(float turnCount);

void shoot2(float turnCount2);

void calib(void);

void spinUp(bool);

void spinUpReverse(bool);

void slowSpinUpReverse(bool spinIndex);

void ballSort(bool getCase);

void reAdjust(bool spinIndex);

void poop(bool poop);

void cycle(bool Cycling);

void threeBallMacro();

#endif