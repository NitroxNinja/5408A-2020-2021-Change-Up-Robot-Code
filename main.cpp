/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author(s):    Team 5408D                                                */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// FL                   motor         1               
// FR                   motor         2               
// BL                   motor         3               
// BR                   motor         4               
// iner                 inertial      9               
// indexSensor          line          E               
// trackingLateralRight encoder       C, D            
// trackingLateralLeft  encoder       A, B            
// leftInt              motor         5               
// rightInt             motor         6               
// midInt               motor         7               
// fly                  motor         8               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <cmath>
#include "main.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

int autonomousSelection = -1;

typedef struct _button {
    int    xpos;
    int    ypos;
    int    width;
    int    height;
    bool   state;
    vex::color offColor;
    vex::color onColor;
    const char *label;
} button;

button buttons[] = {
    {   25,  30, 75, 75,  false, 0x404040, 0xE00000, "Half R"},
    {  145,  30, 75, 75,  false, 0x404040, 	0x00808000, "Big L" },
    {  265,  30, 75, 75,  false, 0x404040, 0xE00000, "Home R" },
    {  385,  30, 75, 75,  false, 0x404040, 0xE00000, "Skills" },
    {   25, 140, 75, 75,  false, 0x404040, 0x0000E0, "Blue 1" },
    {  145, 140, 75, 75,  false, 0x404040, 0x0000E0, "Blue 2" },
    {  265, 140, 75, 75,  false, 0x404040, 0x0000E0, "Blue 3" },
    {  385, 140, 75, 75,  false, 0x404040, 0xC0C0C0, "Simple" }
};

void displayButtonControls( int index, bool pressed );

int findButton(  int16_t xpos, int16_t ypos ) {
    int nButtons = sizeof(buttons) / sizeof(button);

    for( int index=0;index < nButtons;index++) {
      button *pButton = &buttons[ index ];
      if( xpos < pButton->xpos || xpos > (pButton->xpos + pButton->width) )
        continue;

      if( ypos < pButton->ypos || ypos > (pButton->ypos + pButton->height) )
        continue;

      return(index);
    }
    return (-1);
}

/*-----------------------------------------------------------------------------*/
/** @brief      Init button states                                             */
/*-----------------------------------------------------------------------------*/
void initButtons() {
    int nButtons = sizeof(buttons) / sizeof(button);

    for( int index=0;index < nButtons;index++) {
      buttons[index].state = false;
    }
}

/*-----------------------------------------------------------------------------*/
/** @brief      Screen has been touched                                        */
/*-----------------------------------------------------------------------------*/
void userTouchCallbackPressed() {
    int index;
    int xpos = Brain.Screen.xPosition();
    int ypos = Brain.Screen.yPosition();

    if( (index = findButton( xpos, ypos )) >= 0 ) {
      displayButtonControls( index, true );
    }

}

/*-----------------------------------------------------------------------------*/
/** @brief      Screen has been (un)touched                                    */
/*-----------------------------------------------------------------------------*/
void userTouchCallbackReleased() {
    int index;
    int xpos = Brain.Screen.xPosition();
    int ypos = Brain.Screen.yPosition();

    if( (index = findButton( xpos, ypos )) >= 0 ) {
      // clear all buttons to false, ie. unselected
      //      initButtons(); 

      // now set this one as true
      if( buttons[index].state == true) {
      buttons[index].state = false; }
      else    {
      buttons[index].state = true;}

      // save as auton selection
      autonomousSelection = index;

      displayButtonControls( index, false );
    }
}

/*-----------------------------------------------------------------------------*/
/** @brief      Draw all buttons                                               */
/*-----------------------------------------------------------------------------*/
void displayButtonControls( int index, bool pressed ) {
    vex::color c;
    Brain.Screen.setPenColor( vex::color(0xe0e0e0) );

    for(int i=0;i<sizeof(buttons)/sizeof(button);i++) {

      if( buttons[i].state )
        c = buttons[i].onColor;
      else
        c = buttons[i].offColor;

      Brain.Screen.setFillColor( c );

      // button fill
      if( i == index && pressed == true ) {
        Brain.Screen.drawRectangle( buttons[i].xpos, buttons[i].ypos, buttons[i].width, buttons[i].height, c );
      }
      else
        Brain.Screen.drawRectangle( buttons[i].xpos, buttons[i].ypos, buttons[i].width, buttons[i].height );

      // outline
      Brain.Screen.drawRectangle( buttons[i].xpos, buttons[i].ypos, buttons[i].width, buttons[i].height, vex::color::transparent );

// draw label
      if(  buttons[i].label != NULL )
        Brain.Screen.printAt( buttons[i].xpos + 8, buttons[i].ypos + buttons[i].height - 8, buttons[i].label );
    }
}

double speedCap = 1; 
double speedCapTurn = 1;

double kP = 0.128; //.13
double kI = 0.0; //integral control causes the robot to jitter
double kD = 0.27; //.22
double turnkP = 0.378;//0.14
double turnkI = 0.0; //integral control causes the robot to jitter
double turnkD = 0.485;//0.0015; 0.44
 
float dV = 0;
int dTV = 0;
 
int error; //sensor-desired value positional value
int prevError = 0; //error 20 milliseconds ago
int derivative;
int totalError = 0; //what in the world is this
 
int turnError; //sensor-desired value positional value
int turnPrevError = 0; //error 20 milliseconds ago
int turnDerivative;
int turnTotalError = 0; //what in the world is this
 
bool resetDriveSensors = false;
bool enablePID = false;


/*if(resetShootSensors){ //you must reset the drive sensors everytime before you run a base action
  fly.setRotation(0,degrees);
  midInt.setRotation(0,degrees);
  iner.setRotation(0,degrees);
  resetShootSensors=false; //this is a bool, once the sensors have been reset, it will return as "true"
}*/
 
int drivePID(){
 
 while(enablePID){
 
   if(resetDriveSensors){ //you must reset the drive sensors everytime before you run a base action
     FL.setPosition(0,degrees); //we reset 5 sensors because the more sensor readings you take, 
     FR.setPosition(0,degrees); //- the more accurate the program will react to changes in the enviornment and ERROR
     BL.setPosition(0,degrees); //this is also known as the Kalman Filter
     BR.setPosition(0,degrees);

     trackingLateralRight.setPosition(0, degrees);
     trackingLateralLeft.setPosition(0, degrees);
     iner.setRotation(0,degrees);
     resetDriveSensors=false; //this is a bool, once the sensors have been reset, it will return as "true"
   }

   int trackingWheelPosition = ((-trackingLateralLeft.rotation(degrees) + trackingLateralRight.rotation(degrees) ) /2 ) * 1.4545;
  
   int inertialPosition = iner.rotation(degrees);
   
   int averagePosition = trackingWheelPosition;
   error = averagePosition - ((360 * dV) * 1.1); 
 
   derivative = error - prevError; 

   totalError += error;  
 
   double lateralMotorPower = ((error * kP + derivative * kD + totalError * kI) / 12 );
  
   int turnDifference = inertialPosition; 
   
   turnError = turnDifference - dTV;
 
   turnDerivative = turnError - turnPrevError; 
 
   turnTotalError += turnError; 
 
   double turnMotorPower = (turnError * turnkP + turnDerivative * turnkD + turnTotalError * turnkI) / 2 ;
 
   FL.spin(reverse , (lateralMotorPower * speedCap) + (turnMotorPower * speedCapTurn) , voltageUnits::volt);
   FR.spin(reverse , (lateralMotorPower * speedCap) - (turnMotorPower * speedCapTurn) , voltageUnits::volt);
   BL.spin(reverse , (lateralMotorPower * speedCap) + (turnMotorPower * speedCapTurn) , voltageUnits::volt);
   BR.spin(reverse , (lateralMotorPower * speedCap) - (turnMotorPower * speedCapTurn) , voltageUnits::volt);

 
   prevError = error;
   turnPrevError = turnError;
 
   vex::task::sleep(25);
 }
 return 1; 
}

void pre_auton(void) {
  calib(); 
  vexcodeInit(); 
}

void autonomous(void) {
  vex::task bill(drivePID);
  enablePID = true;
  resetDriveSensors = true;

  int Red1   = buttons[0].state; //auton selector 
  int Red2   = buttons[1].state;
  int Red3   = buttons[2].state;
  int Skills = buttons[3].state;
  int Blue1  = buttons[4].state;
  int Blue2  = buttons[5].state;
  int Blue3  = buttons[6].state;
  int Simple = buttons[7].state;

  if(Red1){
  deploy();
  task::sleep(500);
  
  spinUp(true);
  dV=7;
  dTV=75;
  task::sleep(2250); //tune for later, previous value was 2500 

  resetDriveSensors = true;

  spinUp(false);
  dV=1.2;
  dTV=0;
  task::sleep(1250);

  //shoot(10);

  cycle(true);
  task::sleep(1200);
  cycle(false);

  task::sleep(250);

  slowSpinUpReverse(true);
  resetDriveSensors = true;
  task::sleep(500);
  
  dV=-6;
  dTV=0;
  task::sleep(1250);
  resetDriveSensors = true;

  spinUpReverse(false);

  dV=0;
  dTV=135;
  task::sleep(1000);
  resetDriveSensors = true;

  dV=6;
  dTV=0;
  task::sleep(1250);
  resetDriveSensors = true;

  dV=0;
  dTV=90;
  task::sleep(750);
  resetDriveSensors = true;

  dV=2;
  dTV=0;
  task::sleep(750);
  resetDriveSensors = true;

  dV=0;
  dTV=0;
  task::sleep(10);
  resetDriveSensors = true;

  }

  if(Red2){
    //Big L
    /*deploy();
    task::sleep(500);

    resetDriveSensors=true;
    dV=5.8;
    dTV=0;
    task::sleep(2000);

    resetDriveSensors = true; 
    dV=0;
    dTV=-83;
    task::sleep(1000);

    resetDriveSensors = true;
    spinUp(true);
    dV=2.2;
    dTV=0;
    task::sleep(1500);

    resetDriveSensors = true;
    spinUp(false);
    dV=1.4;
    dTV=0;
    task::sleep(2000);

    shoot(20);
    task::sleep(2000);
  
    resetDriveSensors = true;
    dV=-2.2;
    dTV=0;
    task::sleep(1500);

resetDriveSensors = true;
    dV = 0;
    dTV =-124;
  task::sleep(1500);

  resetDriveSensors = true;

  dV = 13;
  dTV = 0;
  task::sleep(4000); //goes to alliance partner side 

  resetDriveSensors = true;

  dV = 0;
  dTV = 45;
  task::sleep(1000); //turns toward other goal 

  resetDriveSensors = true;

  spinUp(true); 
  dV = 3;
  dTV = 0;
  task::sleep(1500); //collects the ball 

   resetDriveSensors = true;

  spinUp(false);
  dV=1.4;
  dTV=0;
  task::sleep(1000);

  fly.resetRotation();
  fly.resetRotation();
  shoot(20);

  resetDriveSensors = true;

  spinUp(false);
  dV=-1.4;
  dTV=0;
  task::sleep(1000);

  //what is this auton? 
*/
  }
  
  if(Red3){
  deploy();
  task::sleep(500);
  
  spinUp(true);
  dV=7;
  dTV=75;
  task::sleep(1500); //tune for later, previous value was 2500 

  resetDriveSensors = true;

  spinUp(false);
  dV=0.8;
  dTV=0;
  task::sleep(500);
  resetDriveSensors = true;


  //shoot(10);

  cycle(true);
  task::sleep(1300);
  cycle(false);

  task::sleep(250);

  slowSpinUpReverse(true);
  resetDriveSensors = true;
  task::sleep(500);

  speedCap=0.7;
  dV=-4;
  dTV=0;
  task::sleep(1400);
  resetDriveSensors = true;

  spinUpReverse(false);
   
  turnkP = 0.78; //intialize constants for short turning 
  turnkD = 0.49;
  speedCap=1;
  dV=0;
  dTV=135;
  task::sleep(1350);
  resetDriveSensors = true;

  speedCap=0.95;
  dV=13.5;
  dTV=0;
  task::sleep(2000);
  resetDriveSensors = true;

  turnkP = 0.79; //intialize constants for short turning 
  turnkD = 0.5;

  dV = 0;
  dTV = -45;
  task::sleep(750);
  resetDriveSensors = true;

  turnkP = 0.378; //return back to original turning 
  turnkD = 0.485;

  spinUp(true);
  dV = 5;
  dTV = 0;
  task::sleep(750);
  spinUp(false);
  resetDriveSensors=true;

  spinUp(false);
  dV=0.5;
  dTV=0;
  task::sleep(500);
  resetDriveSensors=true;

  //shoot(10);

  cycle(true);
  task::sleep(1300);
  cycle(false);

  task::sleep(250);

  slowSpinUpReverse(true);
  resetDriveSensors = true;
  task::sleep(500);

  spinUp(false);
  dV=-2;
  dTV=0;
  task::sleep(800);

  


  /*dV=6;
  dTV=-45;
  task::sleep(1000);
  resetDriveSensors=true;*/

  }

  if(Blue1){
    deploy();
  task::sleep(500);
  
  spinUp(true);
  dV=7;
  dTV=-75;
  task::sleep(2250); //tune for later, previous value was 2500 

  resetDriveSensors = true;

  spinUp(false);
  dV=1.2;
  dTV=0;
  task::sleep(1250);

  //shoot(10);

  cycle(true);
  task::sleep(1200);
  cycle(false);

  task::sleep(250);

  slowSpinUpReverse(true);
  resetDriveSensors = true;
  task::sleep(500);
  
  dV=-6;
  dTV=0;
  task::sleep(1250);
  resetDriveSensors = true;

  spinUpReverse(false);

  dV=0;
  dTV=-135;
  task::sleep(1000);
  resetDriveSensors = true;

  dV=6;
  dTV=0;
  task::sleep(1250);
  resetDriveSensors = true;

  dV=0;
  dTV=-90;
  task::sleep(750);
  resetDriveSensors = true;

  dV=2;
  dTV=0;
  task::sleep(750);
  resetDriveSensors = true;

  dV=0;
  dTV=0;
  task::sleep(10);
  resetDriveSensors = true;
  }


 
  if(Blue2){}
  if(Blue3){}

  if(Skills){
  //QUADRANT 1 OF PROGRAMMING SKILLS
  
  
  deploy();
  task::sleep(175);

  spinUp(true); //collects 2 balls
  dV = 7.2;
  task::sleep(2000);
  resetDriveSensors = true;

  spinUp(false); //curves to align to 1st goal
  dV = -2.5;
  dTV = 78;
  task::sleep(1400); //1750
  resetDriveSensors = true;

  //
  dV=4.7;
  dTV=0;
  task::sleep(1300); //1750,1400
  //resetDriveSensors = true;
  //

  cycle(true);
  //shoot(2.5); //shoots 1 ball into 1st goal
  task::sleep(900); //2000
  cycle(false);
  resetDriveSensors = true;

  speedCap = 0.75;
  spinUpReverse(true);
  dV = -3.5; //backs out
  dTV = 0;
  task::sleep(880);
  spinUpReverse(false);
  resetDriveSensors = true;
  speedCap = 1;


  reAdjust(true);
  speedCapTurn = 0.8;
  dV = 0; //turns to align collect another ball 
  dTV = -145;
  task::sleep(1250);
  reAdjust(false);
  speedCapTurn = 1;
  resetDriveSensors = true;

  speedCap = 0.7;
  spinUp(true);
  dV = 5.35;
  dTV = 0;
  task::sleep(1900);
  resetDriveSensors = true;
  spinUp(false);
  speedCap = 1;

  speedCapTurn=0.85;
  dV = 0;
  dTV = 90;
  task::sleep(1000);
  speedCapTurn=1;
  resetDriveSensors = true;

  dV = 1.25;
  dTV = 0;
  task::sleep(1000);
  //resetDriveSensors = true;
  

  //QUADRANT 2
  
  cycle(true);
  task::sleep(800);
  cycle(false);

  spinUpReverse(true);
  dV = -2;
  dTV = 0;
  task::sleep(1050);
  spinUpReverse(false);
  resetDriveSensors = true;

  speedCapTurn=0.8;
  dV = 0;
  dTV = -89;
  task::sleep(950);
  resetDriveSensors = true;
   speedCapTurn=0.8;

  speedCap = 0.7;
  spinUp(true);
  dV = 6.4; //intakes next ball 
  dTV = 0;
  task::sleep(1700);
  spinUp(false);
  resetDriveSensors = true;
  speedCap = 1;

  dV = -1.5; //aligns to goal 
  dTV = 45;
  task::sleep(1200);
  resetDriveSensors = true;

  dV = 5.25; //slighlty pushes up the goal 
  dTV = 0;
  task::sleep(1200);

  cycle(true); //cycles ball, gets rid of 2 blue balls, scores 1 
  task::sleep(1050);
  cycle(false);
  resetDriveSensors = true;

  spinUpReverse(true); //backs out while spitting out 
  dV = -6.3;
  dTV = 0;
  task::sleep(1600);
  spinUpReverse(false);
  resetDriveSensors = true;

  dV = 0; //turns to align to next ball 
  dTV = -130;
  task::sleep(1050);
  resetDriveSensors = true;

  spinUp(true); //collects ball 
  dV = 4.9;
  dTV = 0;
  task::sleep(1250);
  spinUp(false);
  resetDriveSensors = true;

  dV = 0; //turns to align to goal 
  dTV = 90;
  task::sleep(950);
  resetDriveSensors = true;


  dV = 4.7; //slighlty pushes up the goal 
  dTV = 0;
  task::sleep(1000);
  //resetDriveSensors = true;



  cycle(true); //cycles ball, gets rid of 1 blue ball, scores 1 
  task::sleep(950);
  cycle(false);
  
  
  //QUADRANT 3

  //spinUpReverse(true);
  dV = -0.8;
  dTV = 0;
  task::sleep(850);
  resetDriveSensors = true;
  //spinUpReverse(false);

  turnkP = 0.376;//0.14
  turnkD = 0.485;//0.0015; 0.44

  poop(true);
  dV = 0;
  dTV = -90; //adjusted to 91 for undershoot
  task::sleep(1000);
  poop(false);
  resetDriveSensors = true;

  turnkP = 0.378;//0.14
  turnkD = 0.485;//0.0015; 0.44

  speedCap = 0.65;
  spinUp(true);
  dV = 7.55; //intakes next ball 
  dTV = 0;
  task::sleep(1800);
  spinUp(false);
  resetDriveSensors = true;
  speedCap = 1;

  turnkP = 0.79; //intialize constants for short turning 
  turnkD = 0.5;

  dV = 0;
  dTV = 45;
  task::sleep(600);
  resetDriveSensors = true;

  turnkP = 0.378; //return back to original turning 
  turnkD = 0.485;
  
  dV = 2.28; //pushes up the goal 
  dTV = 0;
  task::sleep(1050);

  cycle(true); //cycles ball, gets rid of 2 blue balls, scores 1 
  task::sleep(1050);
  cycle(false);
  //resetDriveSensors = true;

  
  //
  spinUpReverse(true); //backs out while spitting out 
  dV = -5.7;
  dTV = 0;
  task::sleep(1650);
  spinUpReverse(false);
  resetDriveSensors = true;

  dV = 0; //turns to align to next ball 
  dTV = -136;
  task::sleep(1050);
  resetDriveSensors = true;

  speedCap = 0.7;
  spinUp(true); //collects ball 
  dV = 3;
  dTV = 0;
  task::sleep(1150);
  //spinUp(false);
  speedCap = 1;
  resetDriveSensors = true;

  turnkP = 0.38; //return back to original turning 
  turnkD = 0.485;

  dV = 0;
  dTV = 92;
  task::sleep(1050);
  resetDriveSensors =true;

  turnkP = 0.378; //return back to original turning 
  turnkD = 0.485;

  dV = 4.5;
  dTV = 0;
  task::sleep(1150);
  spinUp(false);

  turnkP = 0.378; //return back to original turning 
  turnkD = 0.485;


//QUADRANT 4
  shoot(15);

   turnkP = 0.378; //return back to original turning 
  turnkD = 0.485;

  dV = -2.25;
  dTV = 0;
  task::sleep(1150);
  spinUp(false);
  resetDriveSensors=true;

   turnkP = 0.38; //return back to original turning 
  turnkD = 0.485;

  dV = 0;
  dTV = -90;
  task::sleep(1000);
  resetDriveSensors =true;
  
   turnkP = 0.375; //return back to original turning 
  turnkD = 0.485;
  speedCap=0.75;
  spinUp(true);
  dV = 7;
  dTV = 0;
  task::sleep(1250);
  resetDriveSensors=true;

  turnkP = 0.38; //return back to original turning 
  turnkD = 0.485;

  dV = -1;
  dTV = 55;
  task::sleep(900);
  spinUp(false);
  resetDriveSensors=true;

  turnkP = 0.38; //return back to original turning 
  turnkD = 0.485;

  dV =5.05;
  dTV = 0;
  task::sleep(1000);
  //spinUp(true);
  resetDriveSensors=true;

  turnkP = 0.38; //return back to original turning 
  turnkD = 0.485;
  dV = 0;
  dTV = 0;
  task::sleep(50);
  spinUp(false);
  resetDriveSensors=true;


  cycle(true);
  task::sleep(1000);
  cycle(false);
  resetDriveSensors=true;
  
   turnkP = 0.375; //return back to original turning 
  turnkD = 0.485;
  
  dV = -7.5;
  dTV = 0;
  task::sleep(1250);
  spinUp(false);
  resetDriveSensors=true;


  turnkP = 0.38; //return back to original turning 
  turnkD = 0.485;

  dV=0;
  dTV=-135;
  task::sleep(1250);
  spinUp(true);
  resetDriveSensors=true;

  turnkP = 0.38; //return back to original turning 
  turnkD = 0.485;


  dV=3.75;
  dTV=0;
  task::sleep(1000);
  spinUp(true);
  resetDriveSensors=true;

  turnkP = 0.38; //return back to original turning 
  turnkD = 0.485;

 dV=0;
  dTV=-90;
  task::sleep(1000);
  spinUp(false);
  resetDriveSensors=true;

  turnkP = 0.38; //return back to original turning 
  turnkD = 0.485;

  dV=2.25;
  dTV=0;
  task::sleep(750);
  spinUp(false);
  resetDriveSensors=true;

  turnkP = 0.38; //return back to original turning 
  turnkD = 0.485;

  enablePID=false;

   whackTheBall(true);
   driveSetVelocity(100);
   rotateDrive(1);
   task::sleep(100);
   rotateDrive(-1);

    rotateDrive(1);
   task::sleep(100);
   rotateDrive(-1);

    rotateDrive(1);
   task::sleep(100);
   rotateDrive(-1);

    rotateDrive(1);
   task::sleep(100);
   rotateDrive(-0.5);

  pivotRight(1);
  task::sleep(100);



  dV = 0;
  dTV = 0;
  task::sleep(10);
  spinUp(false);
  resetDriveSensors=true;

  cycle(true);
  task::sleep(1000);
  cycle(false);
  resetDriveSensors=true;



  
  }
  if(Simple){
    
  turnkP = 0.79; //intialize constants for short turning 
  turnkD = 0.5;
  speedCap=1;
  dV=0;
  dTV=130;
  task::sleep(1350);
  resetDriveSensors = true;

  
  /*
    cycle(true);
  task::sleep(1000);
  cycle(false);
  resetDriveSensors=true;

 task::sleep(1000);
  turnkP = 0.38; //return back to original turning 
  turnkD = 0.485;
  dV = 3;
  dTV = 0;
  task::sleep(1000);
  spinUp(false);
  resetDriveSensors=true;

  turnkP = 0.38; //return back to original turning 
  turnkD = 0.485;
  dV = 0;
  dTV = 0;
  task::sleep(50);
  spinUp(false);
  resetDriveSensors=true;


  cycle(true);
  task::sleep(1000);
  cycle(false);
  
 */

  }
}

void usercontrol(void) {

  enablePID=false;
  while (1) {
    
    joystickControl();

    intakeControl();

    indexSystem();
  
    if(Controller1.ButtonRight.pressing()){
      deploy();
    }

    /*if(Controller1.ButtonX.pressing()){
      threeBallMacro();
    }*/


    

  }
}

int main() {
  
  Brain.Screen.pressed(userTouchCallbackPressed);
  Brain.Screen.released( userTouchCallbackReleased);
  displayButtonControls(0,false);
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {

    wait(100, msec);
  }
}