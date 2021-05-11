using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor FL;
extern motor FR;
extern motor BL;
extern motor BR;
extern inertial iner;
extern line indexSensor;
extern encoder trackingLateralRight;
extern encoder trackingLateralLeft;
extern motor leftInt;
extern motor rightInt;
extern motor midInt;
extern motor fly;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );