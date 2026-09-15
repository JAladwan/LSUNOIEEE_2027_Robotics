#include <Wire.h>

#define I2C_ADDR 0x34
#define ADC_BAT_ADDR 0
#define MOTOR_TYPE_ADDR 20                 // Magnetic encoder (44 pulses per revolution), gear ratio: 131 (default)
#define MOTOR_ENCODER_POLARITY_ADDR   21   // Range: 0 or 1, default is 0
#define MOTOR_FIXED_PWM_ADDR      31      // range -100~100, open loop ctrl
#define MOTOR_FIXED_SPEED_ADDR    51      // closed loop ctrl, pulses per 10ms, range typically ±50
#define MOTOR_ENCODER_TOTAL_ADDR  60      // total pulse count, travel distance can be determined

// Distance-Traveled = (Pulse-Count/Pulses-per-Revolution) * (3.14159*Wheel-Diameter)

uint8_t MotorType = 3;                   // JGB style motor = 3
uint8_t MotorEncoderPolarity = 0;

int8_t car_forward[4] = {30, -30, -30, 30};       // Forward movement
int8_t car_back[4] = {-30, 30, 30, -30};          // Backward movement
int8_t car_stop[4] = {0, 0, 0, 0};                // Full stop 

bool WireWriteDataArray(uint8_t reg, uint8_t *val, unsigned int len) {
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(reg);
  for (unsigned int i = 0; i < len; i++) {
    Wire.write(val[i]);
  }
  return (Wire.endTransmission() == 0);
}

// handles signed input --> unsigned input casting
bool WireWriteDataArray(uint8_t reg, int8_t *val, unsigned int len) {
  return WireWriteDataArray(reg, (uint8_t*)val, len);
}

void Running(int8_t running_mode[4]) // input is an array of 4 integers
{
  WireWriteDataArray(MOTOR_FIXED_SPEED_ADDR,running_mode,4);  //Perform the corresponding action.
  delay(2000);
  WireWriteDataArray(MOTOR_FIXED_SPEED_ADDR,car_stop,4);      // stop, works bc global var
  delay(1000);

}

void setup() {
  Wire.begin();
  delay(200);
  WireWriteDataArray(MOTOR_TYPE_ADDR,&MotorType,1);
  delay(5);
  WireWriteDataArray(MOTOR_ENCODER_POLARITY_ADDR,&MotorEncoderPolarity,1);
  delay(2000);

}

void loop() {
  Running(car_forward);
  Running(car_back);
  Running(car_stop);

}
