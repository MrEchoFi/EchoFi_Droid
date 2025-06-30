#include <Servo.h>


const uint8_t PIN_L_HIP   = 2;
const uint8_t PIN_L_KNEE  = 3;
const uint8_t PIN_L_ANKLE = 4;
const uint8_t PIN_R_HIP   = 5;
const uint8_t PIN_R_KNEE  = 6;
const uint8_t PIN_R_ANKLE = 7;

Servo servos[6];
enum { L_HIP, L_KNEE, L_ANKLE, R_HIP, R_KNEE, R_ANKLE };

// Angles
const int NEUTRAL[6] = {
  122, // L_HIP
  160, // L_KNEE
  120, // L_ANKLE
  88, // R_HIP
  10, // R_KNEE
  50  // R_ANKLE
};

// Step Angles
const int  LIFT_ANGLE       = 21;  // how much to lift foot [degree]
const int  SWING_ANGLE      = 22;  // hip swing forward/back (°)
const int  KNEE_BEND        = 9;  // knee flex on stepping leg (°)
const int  STANCE_KNEE_STIFF=  11;  // slight stance-knee extension (°)
const int  INTERP_STEPS     = 14;  // interpolation steps for smooth motion
const int  INTERP_DELAY_MS  = 16;  // delay between interp. moves

void setup() {
  
  servos[L_HIP]   .attach(PIN_L_HIP);
  servos[L_KNEE]  .attach(PIN_L_KNEE);
  servos[L_ANKLE] .attach(PIN_L_ANKLE);
  servos[R_HIP]   .attach(PIN_R_HIP);
  servos[R_KNEE]  .attach(PIN_R_KNEE);
  servos[R_ANKLE] .attach(PIN_R_ANKLE);

  
  for (int i = 0; i < 6; i++) {
    servos[i].write(NEUTRAL[i]);
    delay(50);
  }
  delay(500);
}

void loop() {
  step(true);   // left leg 
  step(false);  // right leg 
}

/
void smoothMove(uint8_t idx, int target) {
  int start = servos[idx].read();
  for (int i = 1; i <= INTERP_STEPS; i++) {
    float frac = i / float(INTERP_STEPS);
    int pos = start + (target - start) * frac;
    servos[idx].write(pos);
    delay(INTERP_DELAY_MS);
  }
}


void step(bool leftLeg) {
 
  uint8_t hipStep   =  leftLeg ? L_HIP   : R_HIP;
  uint8_t kneeStep  =  leftLeg ? L_KNEE  : R_KNEE;
  uint8_t ankleStep =  leftLeg ? L_ANKLE : R_ANKLE;
  uint8_t hipStance =  leftLeg ? R_HIP   : L_HIP;
  uint8_t kneeStance=  leftLeg ? R_KNEE  : L_KNEE;

  
  smoothMove(kneeStep,   NEUTRAL[kneeStep]   + KNEE_BEND);
  smoothMove(kneeStance, NEUTRAL[kneeStance] - STANCE_KNEE_STIFF);

  
  smoothMove(ankleStep, NEUTRAL[ankleStep] - LIFT_ANGLE);

  
  smoothMove(hipStep,   NEUTRAL[hipStep]   + SWING_ANGLE);
  smoothMove(hipStance, NEUTRAL[hipStance] - SWING_ANGLE/2);

  
  smoothMove(ankleStep, NEUTRAL[ankleStep]);

  
  smoothMove(kneeStep,   NEUTRAL[kneeStep]);
  smoothMove(kneeStance, NEUTRAL[kneeStance]);

  
  smoothMove(hipStep,   NEUTRAL[hipStep]);
  smoothMove(hipStance, NEUTRAL[hipStance]);

  
  delay(100);
}
