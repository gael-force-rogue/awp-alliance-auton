/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       humanfriend22                                             */
/*    Created:      10/21/2024, 11:19:17 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

competition Competition;
brain Brain;
controller Controller(primary);

// Drivetrain
vex::motor left1(vex::PORT1, false);
vex::motor left2(vex::PORT2, false);
// vex::motor left3(vex::PORT3, false);
vex::motor right1(vex::PORT4, true);
vex::motor right2(vex::PORT5, true);
// vex::motor right3(vex::PORT6, true);

vex::motor_group leftMotors(left1, left2);
vex::motor_group rightMotors(right1, right2);

// Extra Blessings
vex::motor intake(vex::PORT7, false);
pneumatics clamp(Brain.ThreeWirePort.A);

void autonomous(void) {
    leftMotors.spinFor(forward, 1000, msec);
    rightMotors.spinFor(forward, 1000, msec);
    leftMotors.stop();
    rightMotors.stop();
}

void clampThreadF(void) {
    bool state = clamp.value();

    while (1) {
        if (Controller.ButtonA.pressing()) {
            state = !state;
            clamp.set(state);

            while (Controller.ButtonA.pressing()) wait(20, msec);
        }

        wait(20, msec);
    }
};

void usercontrol(void) {
    // false makes it tank drive
    bool arcadeDrive = true;

    intake.setStopping(brake);
    vex::thread clampThread(clampThreadF);

    while (1) {
        if (arcadeDrive) {
            float y = Controller.Axis3.position(pct);
            float x = Controller.Axis1.position(pct);

            leftMotors.spin(forward, y + x, pct);
            rightMotors.spin(forward, y - x, pct);
        } else {
            float l = Controller.Axis3.position(pct);
            float r = Controller.Axis2.position(pct);

            leftMotors.spin(forward, l, pct);
            rightMotors.spin(forward, r, pct);
        }

        if (Controller.ButtonR1.pressing()) {
            intake.spin(forward, 100, pct);
        } else if (Controller.ButtonR2.pressing()) {
            intake.spin(forward, -100, pct);
        } else {
            intake.stop();
        }

        wait(20, msec);
    }
}

int main() {
    Competition.autonomous(autonomous);
    Competition.drivercontrol(usercontrol);

    while (true) wait(100, msec);
}
