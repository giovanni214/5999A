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
using namespace vex;

competition Competition;

// define your global instances of motors and other devices here
//This code sets up the motors for driving, the second half gives the motors ability to spin
void setupMotors () {
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
}

void moveLeftSide(int speed) {
  top_left_motor.setVelocity(speed, percent);
  left_bottom_motor.setVelocity(speed, percent);
  left_top_motor.setVelocity(speed, percent);
}

void moveRightSide(int speed) {
  top_right_motor.setVelocity(speed, percent);
  right_bottom_motor.setVelocity(speed, percent);
  right_top_motor.setVelocity(speed, percent);
}

//This moves the robot with the right and left side, it is made as a function for the
//ability to change the orientation of the robot
void moveRobotTankMode(int leftTread, int rightTread, bool isBackwards) {
  moveLeftSide(!isBackwards ? leftTread : rightTread * -1);
  moveRightSide(!isBackwards ? rightTread : leftTread * -1);
}

//may create in the future
void moveRobotArcadeMode(int forwardValue, int turnValue) {
  
}

void stopRobot(int time) {
  wait(time, msec);
  moveRobotTankMode(0, 0, false);
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
  setupMotors();
  
  //far side auton
  moveRobotTankMode(50, 50, true); //back up to release sick
  stopRobot(70);

  moveRobotTankMode(90, -90, false); //turn to push triball in
  stopRobot(130);

  moveRobotTankMode(100, 100, false);
  stopRobot(700);

  stopRobot(1000); //wait one second

  moveRobotTankMode(30, 20, true);
  stopRobot(800);

  stopRobot(1000); //wait one second

  moveRobotTankMode(-50, 50, true);
  stopRobot(500);

  stopRobot(1000); //wait one second
  
  moveRobotTankMode(100, 100, true);
  stopRobot(1000);

  // //close side
  // moveRobotTankMode(50, 50, true); //back up to release sick
  // stopRobot(70);

  // moveRobotTankMode(100, 100, false); //back up from wall
  // stopRobot(400);

  // moveRobotTankMode(100, -100, true); //turn to face corner
  // stopRobot(200);

  // moveRobotTankMode(100, 100, true); //move towards corner
  // stopRobot(300);

  // moveRobotTankMode(-100, 100, true); //hit triball
  // stopRobot(500);



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



void newDebugLine(const char *text, int &pos) {
  Brain.Screen.printAt(5, pos, text);
  pos += 25; //prints at the next new line
}

double keepTurning(double desiredAngle, double actualAngle) {
  double kP = 0.8;
  double changeInAngle = desiredAngle - actualAngle;
  if(changeInAngle < 1 && changeInAngle > -1) {
    return 0;
  }

  double motorSpeed = kP * (changeInAngle);
  return motorSpeed;
}

void usercontrol(void) {
  // User control code here, inside the loop
  setupMotors();
  robot_rotation.calibrate();
  robot_rotation.setRotation(0, degrees);
  while(robot_rotation.isCalibrating()) {
   wait(100, msec);
  }
  
  bool isBackward = false; //checks if robot is facing backward to swap controls
  bool leftPneumaticsOn = false; //checks if robot has switched pneumatics on or off
  bool rightPneumaticsOn = false; //checks if robot has switched pneumatics on or off
  bool firePuncher = false; //Tells it to turn on or off puncher

  int minSwitchTime = 600; //milliseconds, mininumum wait time for the next button press
  int lastSwitchTime = minSwitchTime; //Backwards Switch time
  int leftPneumaticTime = minSwitchTime; //left pneumatic switch time
  int rightPneumaticTime = minSwitchTime; //right pneumatic switch time
  int lastUpTime = minSwitchTime; //Up button press switching time
  int lastDownTime = minSwitchTime; //Down button press switching time
  int lastCataBtnTime = minSwitchTime; //The last time the button was pressed

  int cataSpeed = 50; //in percent
  char drivingMode[] = "TANK";
   
  while (1) {
    //Get the axis values in percent (-100% for down and 100% for up)
    int leftJoystickVertical = Controller1.Axis3.position(percent);
    int rightJoystickVertical = Controller1.Axis2.position(percent);

    //Reverse Controls
    if(Controller1.ButtonL1.pressing() && lastSwitchTime >= minSwitchTime) {
      lastSwitchTime = 0;
      isBackward = !isBackward;
      Controller1.rumble("."); // "." is a short pulse while "-" are long
    }

    //LEFT SIDE WINGS
    //The same as the code above, but we check L2 and release left side pneumatics
    if(Controller1.ButtonL2.pressing() && leftPneumaticTime >= minSwitchTime) {
      leftPneumaticTime = 0;
      leftPneumaticsOn = !leftPneumaticsOn;
      left_pneumatic.set(leftPneumaticsOn);
    }

    //RIGHT SIDE WINGS
    //The same as the code above, but we check R2 and release right side pneumatics
    if(Controller1.ButtonR2.pressing() && rightPneumaticTime >= minSwitchTime) {
      rightPneumaticTime = 0;
      // rightPneumaticsOn = !rightPneumaticsOn;
      // right_pneumatic.set(rightPneumaticsOn);
      // double turnValue = keepTurning(90, robot_rotation.rotation());
    }

    //Automatic Firing of catapult
    if(Controller1.ButtonR1.pressing() && lastCataBtnTime >= minSwitchTime) {
      lastCataBtnTime = 0;
      firePuncher = !firePuncher; //false --> true, true --> false
      Controller1.rumble("-"); // "." is a short pulse while "-" are long
    }

    //Lower or raise the sped of the catapult
    if(Controller1.ButtonUp.pressing() && lastUpTime >= minSwitchTime) {
      lastUpTime = 0;
      cataSpeed += 5;
      if(cataSpeed > 100) {
        cataSpeed = 100;
      }
    }

    if(Controller1.ButtonDown.pressing() && lastDownTime >= minSwitchTime) {
      lastDownTime = 0;
       cataSpeed -= 15;
      if(cataSpeed < 40) {
        cataSpeed = 40;
      }
    }

    if(firePuncher) {
      left_cata_motor.setVelocity(cataSpeed, percent);
      right_cata_motor.setVelocity(cataSpeed, percent);
    } else {
      left_cata_motor.setVelocity(0, percent);
      right_cata_motor.setVelocity(0, percent);

    }

    if(strcmp(drivingMode, "TANK") == 0) {
      moveRobotTankMode(leftJoystickVertical, rightJoystickVertical, isBackward);
    }

    //Debug to display the value of stuff
    Brain.Screen.clearScreen();
    int position = 25;
    newDebugLine(isBackward ? "CONTROLS: REVERSED" : "CONTROLS: NORMAL", position);
    newDebugLine(leftPneumaticsOn ? "LEFT WINGS: ON" : "LEFT WINGS: OFF", position);
    newDebugLine(rightPneumaticsOn ? "RIGHT WINGS: ON" : "RIGHT WINGS: OFF", position);
    
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%g", robot_rotation.rotation());
    newDebugLine(buffer, position);

    //Add 20 milleseconds of wait time
    lastCataBtnTime += 20;
    lastSwitchTime += 20;
    leftPneumaticTime += 20;
    rightPneumaticTime += 20;
    lastUpTime += 20;
    lastDownTime += 20;
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
