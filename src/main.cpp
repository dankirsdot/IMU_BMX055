/* BMX055_t3 Basic Example Code

 SDA and SCL should have external pull-up resistors (to 3.3V).
 
 Hardware setup:
 BMX055 -------------------- Teensy 3.2
 VDD ----------------------- 3.3V
 SDA ----------------------- 18
 SCL ----------------------- 19
 GND ----------------------- GND
 
 Note: The BMX055 is an I2C sensor and uses the Teensy 3.1 i2c_t3.h Wire library. 
 Because the sensor is not 5V tolerant, we are using a 3.3 V 8 MHz Pro Mini or a 3.3 V Teensy 3.1.
 We have disabled the internal pull-ups used by the Wire library in the Wire.h/twi.c utility file.
 We are also using the 400 kHz fast I2C mode by setting the TWI_FREQ  to 400000L /twi.h utility file.
 */

#include <Arduino.h>
#include <BMX055.h>

float G = 9.80666;

// BMX055 variables
int16_t accelCount[3];                                                           // Stores the 16-bit signed accelerometer sensor output
int16_t gyroCount[3];                                                            // Stores the 16-bit signed gyro sensor output
int16_t magCount[3];                                                             // Stores the 13/15-bit signed magnetometer sensor output
float gyroBias[3] = {0, 0, 0}, accelBias[3] = {0, 0, 0}, magBias[3] = {0, 0, 0}; // Bias corrections for gyro, accelerometer, mag

float ax, ay, az, gx, gy, gz, mx, my, mz; // variables to hold latest sensor data values

uint32_t delt_t = 0, count = 0; // used to control display output rate !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void setup()
{
  // Setup for Master mode, pins 18/19, internal pullups, 400kHz for Teensy 3.2
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, I2C_RATE_400);
  delay(5000);
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Read the BMX-055 WHO_AM_I registers, this is a good test of communication
  Serial.println("Communication test...");

  byte c = readByte(BMX055_ACC_ADDRESS, BMX055_ACC_WHOAMI);   // Read ACC WHO_AM_I register for acclerometer
  byte d = readByte(BMX055_GYRO_ADDRESS, BMX055_GYRO_WHOAMI); // Read GYRO WHO_AM_I register for gyroscope

  writeByte(BMX055_MAG_ADDRESS, BMX055_MAG_PWR_CNTL1, 0x01); // wake up magnetometer first thing
  delay(100);
  byte e = readByte(BMX055_MAG_ADDRESS, BMX055_MAG_WHOAMI); // Read MAG WHO_AM_I register for magnetometer

  if ((c == 0xFA) && (d == 0x0F) && (e == 0x32)) // WHO_AM_I should always be ACC = 0xFA, GYRO = 0x0F, MAG = 0x32
  {
    Serial.println("BMX055 is online\n");
    delay(1000);

    initBMX055();
    Serial.println("BMX055 initialized for active data mode\n"); // Initialize device for active mode read of acclerometer, gyroscope, and temperature
    delay(1000);

    // get sensor resolutions, only need to do this once
    getAres();
    getGres();
    // magnetometer resolution is 1 microTesla/16 counts or 1/1.6 milliGauss/count
    mRes = 1. / 1.6;
    trimBMX055(); // read the magnetometer calibration data
    delay(1000);

    fastcompaccelBMX055(accelBias);
    Serial.println("accel biases (mg)");
    Serial.println(1000. * accelBias[0]);
    Serial.println(1000. * accelBias[1]);
    Serial.println(1000. * accelBias[2]);

    Serial.println("gyro biases (dps)");
    Serial.println(gyroBias[0]);
    Serial.println(gyroBias[1]);
    Serial.println(gyroBias[2]);

    magcalBMX055(magBias);
    Serial.println("mag biases (mG)");
    Serial.println(magBias[0]);
    Serial.println(magBias[1]);
    Serial.println(magBias[2]);
  }
  else
  {
    Serial.println("Could not connect to BMX055");
    while (1)
      ; // Loop forever if communication doesn't happen
  }
}

void loop()
{
  readAccelData(accelCount); // Read the x/y/z adc values
  // Now we'll calculate the accleration value into actual g's
  ax = (float)accelCount[0] * aRes; // + accelBias[0];  // get actual g value, this depends on scale being set
  ay = (float)accelCount[1] * aRes; // + accelBias[1];
  az = (float)accelCount[2] * aRes; // + accelBias[2];

  readGyroData(gyroCount); // Read the x/y/z adc values
  // Calculate the gyro value into actual degrees per second
  gx = (float)gyroCount[0] * gRes; // get actual gyro value, this depends on scale being set
  gy = (float)gyroCount[1] * gRes;
  gz = (float)gyroCount[2] * gRes;

  readMagData(magCount); // Read the x/y/z adc values
  // Calculate the magnetometer values in milliGauss
  // Temperature-compensated magnetic field is in 16 LSB/microTesla
  mx = (float)magCount[0] * mRes - magBias[0]; // get actual magnetometer value, this depends on scale being set
  my = (float)magCount[1] * mRes - magBias[1];
  mz = (float)magCount[2] * mRes - magBias[2];

  // Serial print at 0.5 s rate independent of data rates
  delt_t = millis() - count;
  if (delt_t > 500)
  {
    if (SerialDebug)
    {
      // acclerometer (mg)
      
      /* Serial.print((int)1000 * ax, 4);
      Serial.print("\t");
      Serial.print((int)1000 * ay, 4);
      Serial.print("\t");
      Serial.println((int)1000 * az, 4); */
     

      // acclerometer (m/s^2)
      
      Serial.print(G*ax, 4);
      Serial.print("\t");
      Serial.print(G*ay, 4);
      Serial.print("\t");
      Serial.println(G*az, 4);
     


      // gyroscope (deg/s)
      /*
      Serial.print(gx, 2);
      Serial.print("\t");
      Serial.print(gy, 2);
      Serial.print("\t");
      Serial.println(gz, 2);
      */

      // magnetometer (mG)
      /*
      Serial.print((int)mx);
      Serial.print("\t");
      Serial.print((int)my);
      Serial.print("\t");
      Serial.println((int)mz);
      */
    }

    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    count = millis();
  }
}