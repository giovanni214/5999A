using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor top_right_motor;
extern motor top_left_motor;
extern motor right_bottom_motor;
extern motor right_top_motor;
extern motor left_bottom_motor;
extern motor left_top_motor;
extern controller Controller1;
extern motor left_cata_motor;
extern motor right_cata_motor;
extern digital_out left_pneumatic;
extern digital_out right_pneumatic;
extern digital_in cata_switch;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );