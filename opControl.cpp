#include "vex.h"
#include "main.h"
#include <cmath>

using namespace vex;

//Arcade Drive in usercontrol

void joystickControl()
{
  int X,Y;
 
  if(abs(Controller1.Axis2.value())>9)
  {
  Y = Controller1.Axis2.value() * 0.11;
  }    
  
  else
  {
    Y = 0;
  }
   
  if(abs(Controller1.Axis1.value()) >9)
  {
  X = Controller1.Axis1.value() * 0.095 ;
  }    

  else
  {
  X = 0;
  } 
  FL.spin(forward,(X + Y), volt);
  FR.spin(forward,(Y - X), volt);
  BL.spin(forward,(X + Y), volt);
  BR.spin(forward,(Y - X), volt);  

  BL.setStopping(brakeType::hold);
  BR.setStopping(brakeType::hold);
  FL.setStopping(brakeType::hold);
  FR.setStopping(brakeType::hold);
}

//Intake control and "Flywheel"
bool intakeToggle = false;
bool latch = false;

void intakeControl()
{ 
  //intake systems 
  if(Controller1.ButtonL1.pressing())
  {
    midInt.spin(forward,8.5,volt);
    fly.spin(forward,12,volt);
  }

  //threeBallMacro

  else if(Controller1.ButtonR1.pressing())
  {
      if(intakeToggle){

      fly.spin(forward,8,volt);

      if(indexSensor.value(range12bit) <= 2400){
      //fly.spin(reverse,12,volt);
      fly.stop(brakeType::brake);
      }
    
  }
    rightInt.spin(forward,12,volt);
    leftInt.spin(forward,12,volt);
    midInt.spin(forward,12,volt);
  }

  else if(Controller1.ButtonR2.pressing())
  {
    rightInt.spin(reverse,12,volt);
    leftInt.spin(reverse,12,volt);
    midInt.spin(reverse,10,volt);
    fly.spin(reverse,9,volt);
  }

  else
  {
    rightInt.stop(brakeType::coast);
    leftInt.stop(brakeType::coast);
    midInt.stop(brakeType::coast);
    fly.stop(brakeType::coast);
  }
 
}



void indexSystem(){

  if(Controller1.ButtonL2.pressing()){
    Controller1.rumble(".");

    if(!latch){
    intakeToggle = !intakeToggle;
    latch = true;
    }

    }
  else {
  latch = false;
  }

  if(Controller1.ButtonLeft.pressing()){
    fly.spin(reverse, 12, volt); 
  }

}
