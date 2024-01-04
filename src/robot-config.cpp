#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor top_right_motor = motor(PORT20, ratio6_1, false);
motor top_left_motor = motor(PORT11, ratio6_1, true);
motor right_bottom_motor = motor(PORT16, ratio6_1, false);
motor right_top_motor = motor(PORT19, ratio6_1, false);
motor left_bottom_motor = motor(PORT12, ratio6_1, true);
motor left_top_motor = motor(PORT15, ratio6_1, true);
controller Controller1 = controller(primary);
motor left_cata_motor = motor(PORT14, ratio18_1, true);
motor right_cata_motor = motor(PORT17, ratio18_1, false);
digital_out left_pneumatic = digital_out(Brain.ThreeWirePort.B);
digital_out right_pneumatic = digital_out(Brain.ThreeWirePort.C);
digital_in cata_switch = digital_in(Brain.ThreeWirePort.A);

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