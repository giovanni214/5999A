#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor top_right_motor = motor(PORT5, ratio6_1, true);
motor top_left_motor = motor(PORT15, ratio6_1, false);

motor right_bottom_motor = motor(PORT10, ratio6_1, false);
motor right_top_motor = motor(PORT9, ratio6_1, false);

motor left_bottom_motor = motor(PORT20, ratio6_1, true);
motor left_top_motor = motor(PORT19, ratio6_1, true);

motor left_cata_motor = motor(PORT12, ratio18_1, false);
motor right_cata_motor = motor(PORT2, ratio18_1, true);

inertial robot_rotation = inertial(PORT4); //fake port
// vision kicker_vision_seneor = vision(PORTiskyetttt, 255);

controller Controller1 = controller(primary);

digital_out left_pneumatic = digital_out(Brain.ThreeWirePort.B);
digital_out right_pneumatic = digital_out(Brain.ThreeWirePort.C);

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
