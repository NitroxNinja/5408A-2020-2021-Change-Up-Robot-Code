#include "vex.h"
#include "main.h"

using namespace vex;

//auton base control, no PID 


void whackTheBall(bool spinIndex){ 
  if(spinIndex == true){
    rightInt.spin(reverse,8,voltageUnits::volt);
    leftInt.spin(reverse,8,voltageUnits::volt);

  }

  if(spinIndex == false){
    rightInt.stop(brakeType::brake);
    leftInt.stop(brakeType::brake);
  }

  task::sleep(5);
}


void pivotLeft(float L)
{
    leftInt.spin(directionType::fwd,25,velocityUnits::pct);
    rightInt.spin(directionType::fwd,25,velocityUnits::pct);

    FL.setVelocity(90,velocityUnits::pct);
    BL.setVelocity(90,velocityUnits::pct);
    FL.rotateFor(L, rotationUnits::rev,false);
    BL.rotateFor(L, rotationUnits::rev);

    leftInt.stop(brakeType::hold);
    rightInt.stop(brakeType::hold);


    task::sleep(50);
}
void pivotRight(float R)
{
    leftInt.spin(directionType::fwd,25,velocityUnits::pct);
    rightInt.spin(directionType::fwd,25,velocityUnits::pct);


    FR.setVelocity(90,velocityUnits::pct);
    BR.setVelocity(90,velocityUnits::pct);
    FR.rotateFor(R,rotationUnits::rev,false);
    BR.rotateFor(R,rotationUnits::rev);

    leftInt.stop(brakeType::hold);
    rightInt.stop(brakeType::hold);
    FR.stop(brakeType::hold);
    BR.stop(brakeType::hold);



    task::sleep(50);
}
void spinUp(bool spinIndex){ 
  if(spinIndex == true){
    rightInt.spin(forward,12,voltageUnits::volt);
    leftInt.spin(forward,12,voltageUnits::volt);
    midInt.spin(forward,10,voltageUnits::volt);
    //fly.spin(reverse,12,voltageUnits::volt);
  }

  if(spinIndex == false){
    rightInt.stop(brakeType::brake);
    leftInt.stop(brakeType::brake);
    midInt.stop(brakeType::brake);
    fly.stop(brakeType::brake);
  }

  task::sleep(5);
}

void spinUpReverse(bool spinIndex){ 
  if(spinIndex == true){
    rightInt.spin(reverse,8,voltageUnits::volt);
    leftInt.spin(reverse,8,voltageUnits::volt);
    midInt.spin(reverse,12,voltageUnits::volt);
    fly.spin(reverse,12,voltageUnits::volt);
  }

  if(spinIndex == false){
    rightInt.stop(brakeType::brake);
    leftInt.stop(brakeType::brake);
    midInt.stop(brakeType::brake);
    fly.stop(brakeType::brake);
  }

  task::sleep(5);
}

void slowSpinUpReverse(bool spinIndex){ 
  if(spinIndex == true){
    rightInt.spin(reverse,6,voltageUnits::volt);
    leftInt.spin(reverse,6,voltageUnits::volt);
    midInt.spin(reverse,12,voltageUnits::volt);
    fly.spin(reverse,10,voltageUnits::volt);
  }

  if(spinIndex == false){
    rightInt.stop(brakeType::brake);
    leftInt.stop(brakeType::brake);
    midInt.stop(brakeType::brake);
    fly.stop(brakeType::brake);
  }

  task::sleep(5);
}

void reAdjust(bool spinIndex){ 
  if(spinIndex == true){
    rightInt.spin(forward,8,voltageUnits::volt);
    leftInt.spin(forward,8,voltageUnits::volt);
    midInt.spin(reverse,10,voltageUnits::volt);
    fly.spin(reverse,10,voltageUnits::volt);
  }

  if(spinIndex == false){
    rightInt.stop(brakeType::brake);
    leftInt.stop(brakeType::brake);
    midInt.stop(brakeType::brake);
    fly.stop(brakeType::brake);
  }

  task::sleep(5);
}

void poop(bool poop){
  if(poop == true){
    //rightInt.spin(forward,8,voltageUnits::volt);
    //leftInt.spin(forward,8,voltageUnits::volt);
    midInt.spin(forward,10,voltageUnits::volt);
    fly.spin(reverse,10,voltageUnits::volt);
  }

  if(poop == false){
    
    midInt.stop(brakeType::brake);
    fly.stop(brakeType::brake);
  }

}
void cycle(bool Cycling){
  if(Cycling == true){
    rightInt.spin(forward,12,voltageUnits::volt);
    leftInt.spin(forward,12,voltageUnits::volt);
    midInt.spin(forward,8,voltageUnits::volt);
    fly.spin(forward,12,voltageUnits::volt);
  }

  if(Cycling == false){
    rightInt.stop(brakeType::brake);
    leftInt.stop(brakeType::brake);
    midInt.stop(brakeType::brake);
    fly.stop(brakeType::brake);
  }

  task::sleep(5);
}


/*void ballSort(bool getCase){
  while(true){
  if(getCase){
    rightInt.spin(forward,12,voltageUnits::volt);
    leftInt.spin(forward,12,voltageUnits::volt);
    midInt.spin(forward,10,voltageUnits::volt);
    fly.spin(forward,7,voltageUnits::volt);
    
    
    if(indexSensor.value(range12bit) <= 2400){
      //fly.spin(reverse,12,volt);
      fly.stop(brakeType::brake);
      task::sleep(250);
      }
  }
  }

  if(!getCase){
    rightInt.stop(brakeType::brake);
    leftInt.stop(brakeType::brake);
    midInt.stop(brakeType::brake);
    fly.stop(brakeType::brake);
  }

  task::sleep(5);

}*/

void ballSort(bool getCase)
{
  while(getCase == true){

    fly.spin(forward,8,volt);

    if(indexSensor.value(range12bit) <= 2400){
    //fly.spin(reverse,12,volt);
    fly.stop(brakeType::brake);
    
    }

    rightInt.spin(forward,12,volt);
    leftInt.spin(forward,12,volt);
    midInt.spin(forward,12,volt);
    task::sleep(25);

    break;   
  }

  while(getCase == false) {
    rightInt.stop(brakeType::brake);
    leftInt.stop(brakeType::brake);
    midInt.stop(brakeType::brake);
    fly.stop(brakeType::brake);
    break;
  }

}
//backup controls

void deploy(){
  //Tray.setVelocity(100,percent);
  rightInt.setVelocity(100,percent);
  leftInt.setVelocity(100,percent);
  midInt.setVelocity(100,percent);
  fly.setVelocity(100,percent);

  fly.spinFor(forward,2,turns,false);
  midInt.spinFor(reverse,1,turns);
  task::sleep(25);
  rightInt.spinFor(reverse,2,turns,false);
  leftInt.spinFor(reverse,2,turns,false);
  task::sleep(250);
}

//shooting in auton


void shoot(float turnCount){
  midInt.setVelocity(80,percent);
  fly.setVelocity(100,percent);

  midInt.spinFor(forward,turnCount,rotationUnits::rev,false);
  fly.spinFor(forward,turnCount,rotationUnits::rev,false);

  //midInt.resetRotation();
  //fly.resetRotation();
}

void shoot2(float turnCount2){
  midInt.setVelocity(80,percent);
  fly.setVelocity(100,percent);

  midInt.spinFor(forward,turnCount2,rotationUnits::rev,false);
  fly.spinFor(forward,turnCount2,rotationUnits::rev,false);
}
void threeBallMacro()
{
  midInt.setVelocity(75,percent);
  fly.setVelocity(100,percent);

  midInt.spinFor(forward,6,rotationUnits::rev,false);
  fly.spinFor(forward,6,rotationUnits::rev,false);
}

void driveSetVelocity( double speed){
  FL.setVelocity(speed,percent); 
  FR.setVelocity(speed,percent);
  BL.setVelocity(speed,percent); 
  BR.setVelocity(speed,percent); 
}

void rotateDrive(float count){
  FL.spinFor(count, rotationUnits::rev,false);
  BL.spinFor(count, rotationUnits::rev,false);
  BR.spinFor(count, rotationUnits::rev,false);
  FR.spinFor(count, rotationUnits::rev);       // block until complete
  task::sleep(50);   
}

void createTag(){
  Brain.Screen.clearLine(1, black);
  Brain.Screen.setFont(mono20);
  Brain.Screen.setCursor(1, 14);
}

//calibration of sensor 

void calib(){
  iner.calibrate();
  
  while(iner.isCalibrating()){
    createTag();
    Brain.Screen.print("Inertial is Calibrating...");
    task::sleep(20);

    if(iner.isCalibrating() == false){
    createTag();
    Brain.Screen.print("Inertial is Calibrated!");
    }
  }
}



