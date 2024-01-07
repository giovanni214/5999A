/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// top_right_motor      motor         20              
// top_left_motor       motor         11              
// right_bottom_motor   motor         16              
// right_top_motor      motor         19              
// left_bottom_motor    motor         12              
// left_top_motor       motor         15              
// Controller1          controller                    
// left_cata_motor      motor         14              
// right_cata_motor     motor         17              
// left_pneumatic       digital_out   B               
// right_pneumatic      digital_out   C   
// cata_switch          digital_in    A
// ---- END VEXCODE CONFIGURED DEVICES ----
#include <cmath>
using namespace vex;

competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

//This code sets up the motors for driving, the second half gives the motors ability to spin
void setupMotors (){
  top_right_motor.setStopping(coast);
  top_left_motor.setStopping(coast);
  right_bottom_motor.setStopping(coast);
  right_top_motor.setStopping(coast);
  left_bottom_motor.setStopping(coast);
  left_top_motor.setStopping(coast);
  left_cata_motor.setStopping(coast);
  right_cata_motor.setStopping(coast);

  top_right_motor.spin(forward);
  top_left_motor.spin(forward);
  right_bottom_motor.spin(forward);
  right_top_motor.spin(forward);
  left_bottom_motor.spin(forward);
  left_top_motor.spin(forward);
  left_cata_motor.spin(forward);
  right_cata_motor.spin(forward);

  top_right_motor.setVelocity(0, percent);
  top_left_motor.setVelocity(0, percent);
  right_bottom_motor.setVelocity(0, percent);
  right_top_motor.setVelocity(0, percent);
  left_bottom_motor.setVelocity(0, percent);
  left_top_motor.setVelocity(0, percent);
  left_cata_motor.setVelocity(0, percent);
  right_cata_motor.setVelocity(0, percent);

}

//This moves the robot with the right and left side, it is made as a function for the
//ability to change the orientation of the robot
void moveRobotTankMode(int leftSide, int rightSide) {
  //These are the left side of the robot with the intake facing forward
  top_left_motor.setVelocity(leftSide, percent);
  left_bottom_motor.setVelocity(leftSide, percent);
  left_top_motor.setVelocity(leftSide, percent);
  
  //These are the right side of the robot with the intake facing forward
  top_right_motor.setVelocity(rightSide, percent);
  right_bottom_motor.setVelocity(rightSide, percent);
  right_top_motor.setVelocity(rightSide, percent);
}
  


void usercontrol(void) {
  // User control code here, inside the loop
  setupMotors();
  bool isBackward = false; //checks if robot is facing backward to swap controls
  bool isPneumaticsOn = false; //checks if robot has switched pneumatics
  bool automaticFiring = false; //Tells it to turn on or off automatic firing

  int minSwitchTime = 600; //milliseconds, mininumum wait time for the next button press
  int lastSwitchTime = 0; //Backwards Switch time
  int lastPneumaticTime = 0; //Pneumatic switch time
  int lastCataBtnTime = 0; //last time cata button was pressed
  int cataMaxWaitTime = 1500; //3 seconds
  int cataWaitTime = 0;

  char drivingMode[] = "TANK";

  while (1) {
    //Get the axis values in percent (-100% for down and 100% for up)
    int leftUpAndDown = Controller1.Axis3.position(percent);
    int rightUpAndDown = Controller1.Axis2.position(percent);

    //SWITCH Side
    //Check the the L2 button is being pressed, if so swap the isBackward from true --> false or false --> true
    //We also check that is has been more than a second since the last switch
    //This prevents a bug of switching back and forth constantly
    if(Controller1.ButtonL2.pressing() && lastSwitchTime > minSwitchTime) {
      lastSwitchTime = 0;
      isBackward = !isBackward;
      Controller1.rumble("."); // "." is a short pulse while "-" are long
    }

    //WINGS
    //The same as the code above, but we check R2 and release both pneumatics
    if(Controller1.ButtonR2.pressing() && lastPneumaticTime > minSwitchTime) {
      lastPneumaticTime = 0;
      isPneumaticsOn = !isPneumaticsOn;
      left_pneumatic.set(isPneumaticsOn);
      right_pneumatic.set(isPneumaticsOn);
    }

    //Automatic Firing of catapult
    if(Controller1.ButtonL1.pressing() && lastCataBtnTime > minSwitchTime) {
      lastCataBtnTime = 0;
      cataWaitTime = 0;
      automaticFiring = !automaticFiring; //false --> true, true --> false
    }

    int isSwitchPressed = !cata_switch.value();
    if(isSwitchPressed) {
      //if the limit switch is pressed, stop the cata (this means it is primed)
      left_cata_motor.setVelocity(0, percent);
      right_cata_motor.setVelocity(0, percent);

      //if automatic firing is on, wait a certain amount of time then fire the cata
      //This allows the placement of the triball
      if(automaticFiring) {
        //If it has been enough time, fire the cata, if not, keep waiting
        if(cataWaitTime > cataMaxWaitTime) {
          left_cata_motor.setVelocity(60, percent);
          right_cata_motor.setVelocity(60, percent);
        } else {
          cataWaitTime += 20;
        }
      } 
    } else {
      //If the limit switch is not being pressed, prime the catapult
      cataWaitTime = 0;
      left_cata_motor.setVelocity(60, percent);
      right_cata_motor.setVelocity(60, percent);
    }

    //if the robot is going backward, then we need to reverse the direction of the robot and swap the left and right section
    if (isBackward) {
      leftUpAndDown *= -1; //5 --> -5, -5 --> 5
      rightUpAndDown *= -1;
      moveRobotTankMode(rightUpAndDown, leftUpAndDown);
    } else {
      // // moving the robot normally
      moveRobotTankMode(leftUpAndDown, rightUpAndDown);
    }


    //Debug to display the value of stuff
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(5, 25, automaticFiring ? "AUTO FIRE: ON" : "AUTO FIRE: OFF");
    Brain.Screen.printAt(5, 50, isBackward ? "CONTROLS: REVERSED" : "CONTROLS: NORMAL");
    Brain.Screen.printAt(5, 75, isPneumaticsOn ? "WINGS: ON" : "WINGS: OFF");

    lastSwitchTime += 20;
    lastPneumaticTime += 20;
    lastCataBtnTime += 20;
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
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
