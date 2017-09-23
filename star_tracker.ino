/*
Original Author:- mr.sneezy

 Hardware:
 - Arduino Nano
 - 28BYJ-48 Stepper
 - 2 momentary buttons
 - 
 
 Software:
 - Arduino 1.8.2
 - 

NOTES :-
18-08-2017 got inital code running, with two buttons for fast forward and reverse.
Trimmed step rate by testing for a few hours with angle gauge attached to output.
   
*/

/*Gearing calculations
28BYJ-48 Stepper 64:1 - 4076 steps/rev (@ half stepping)
Secondary = 12:1 (?)
Final = 180:1
2605500 step per output revolution.
sidereal rate = about 250 step/sec.
*/

#include <AccelStepper.h>  //Stepper motor library http://www.airspayce.com/mikem/arduino/AccelStepper/

#define DEBUG_PROG //***********Remark to disable serial DEBUG ***************
#ifdef DEBUG_PROG  //cool way to have debug serial ONLY when you want it, and replaced with effectively no statement when not needed. Supports basic formatting of vars.
#define DEBUG_PRINT(x,...) Serial.print(x, ##__VA_ARGS__)
#define DEBUG_PRINTLN(x,...) Serial.println(x, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(x,...)
#define DEBUG_PRINTLN(x,...)
#endif

//Set Arduino pin aliases to match attached devices
const int ForwardPin = 6;     // the number of the pushbutton pin for fast forward (pressed = 0)
const int BackwardPin = 7;     // the number of the pushbutton pin for fast reverse (pressed = 0)
//note that if no button pressed stepper runs at standard rate forwards.

//AccelStepper stepper; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5 digital I/O Arduino Nano
AccelStepper stepper(AccelStepper::HALF4WIRE, 2, 3, 4, 5);  //note - swapped some wires on O/P of driver module to suit this order.


float speedMultiplier = 1.0001;  //multiplier used for stepper acceleration
const float normal = 256;          //normal speed to obtain sidereal rate, trim this as required.

//SETUP #############################################################################################
void setup(){
  Serial.begin(115200);
  DEBUG_PRINTLN("Starting");

  stepper.setMaxSpeed(1200);    //the maximum step speed my stepper seems to handle without stalling (with acceleration).
//  stepper.setAcceleration(50);    //function only works if using positioning mode, not in constant speed mode:)
  stepper.setSpeed(normal);     //setup step speed to 'normal' speed
  pinMode(ForwardPin,INPUT);		//Button input via pullup (0 = active) - Fast forward
  pinMode(BackwardPin,INPUT);		//Button input via pullup (0 = active)- Fast reverse
  
  DEBUG_PRINT("Normal step rate = ");
  DEBUG_PRINTLN(normal);  
}
//LOOP  ###########################################################################################
void loop(){
  stepper.runSpeed();   //run at speed set by stepper.setSpeed
  buttons();            //go check my fast forward or back buttons
//  stepper.setSpeed(stepper.speed() * speedMultiplier);
}

void buttons()      //test for a button press and do stuff
{
  if (digitalRead(ForwardPin)==0){	//Button pressed is a LOW - Forwards
    stepper.setSpeed(stepper.speed() * speedMultiplier);  //increase setSpeed by the multiplier constant
    return;    // exit out to loop
  }

  if (digitalRead(BackwardPin)==0){	//Button pressed is a LOW - Backwards
    if (stepper.speed() > 0){                 //check to see if current speed is positive
      stepper.setSpeed(stepper.speed() *(-1));  //if positive then make it negative
     }
    stepper.setSpeed(stepper.speed() * speedMultiplier);    //increase setSpeed by the multiplier constant
    return;   // exit out to loop
  }
    stepper.setSpeed(normal);  //reset the step speed to 'normal' constant.
}