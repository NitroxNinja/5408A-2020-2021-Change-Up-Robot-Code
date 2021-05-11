#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor FL = motor(PORT1, ratio6_1, true);
motor FR = motor(PORT2, ratio6_1, false);
motor BL = motor(PORT3, ratio6_1, true);
motor BR = motor(PORT4, ratio6_1, false);
inertial iner = inertial(PORT9);
line indexSensor = line(Brain.ThreeWirePort.E);
encoder trackingLateralRight = encoder(Brain.ThreeWirePort.C);
encoder trackingLateralLeft = encoder(Brain.ThreeWirePort.A);
motor leftInt = motor(PORT5, ratio6_1, false);
motor rightInt = motor(PORT6, ratio6_1, true);
motor midInt = motor(PORT7, ratio6_1, false);
motor fly = motor(PORT8, ratio6_1, true);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}