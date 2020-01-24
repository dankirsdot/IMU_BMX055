#include <Arduino.h>
#include <i2c_t3.h>

// BMX055 data sheet http://ae-bst.resource.bosch.com/media/products/dokumente/bmx055/BST-BMX055-DS000-01v2.pdf
// The BMX055 is a conglomeration of three separate motion sensors packaged together but
// addressed and communicated with separately by design
// Accelerometer registers
#define BMX055_ACC_WHOAMI 0x00 // should return 0xFA
//#define BMX055_ACC_Reserved    0x01
#define BMX055_ACC_D_X_LSB 0x02
#define BMX055_ACC_D_X_MSB 0x03
#define BMX055_ACC_D_Y_LSB 0x04
#define BMX055_ACC_D_Y_MSB 0x05
#define BMX055_ACC_D_Z_LSB 0x06
#define BMX055_ACC_D_Z_MSB 0x07
#define BMX055_ACC_D_TEMP 0x08
#define BMX055_ACC_INT_STATUS_0 0x09
#define BMX055_ACC_INT_STATUS_1 0x0A
#define BMX055_ACC_INT_STATUS_2 0x0B
#define BMX055_ACC_INT_STATUS_3 0x0C
//#define BMX055_ACC_Reserved    0x0D
#define BMX055_ACC_FIFO_STATUS 0x0E
#define BMX055_ACC_PMU_RANGE 0x0F
#define BMX055_ACC_PMU_BW 0x10
#define BMX055_ACC_PMU_LPW 0x11
#define BMX055_ACC_PMU_LOW_POWER 0x12
#define BMX055_ACC_D_HBW 0x13
#define BMX055_ACC_BGW_SOFTRESET 0x14
//#define BMX055_ACC_Reserved    0x15
#define BMX055_ACC_INT_EN_0 0x16
#define BMX055_ACC_INT_EN_1 0x17
#define BMX055_ACC_INT_EN_2 0x18
#define BMX055_ACC_INT_MAP_0 0x19
#define BMX055_ACC_INT_MAP_1 0x1A
#define BMX055_ACC_INT_MAP_2 0x1B
//#define BMX055_ACC_Reserved    0x1C
//#define BMX055_ACC_Reserved    0x1D
#define BMX055_ACC_INT_SRC 0x1E
//#define BMX055_ACC_Reserved    0x1F
#define BMX055_ACC_INT_OUT_CTRL 0x20
#define BMX055_ACC_INT_RST_LATCH 0x21
#define BMX055_ACC_INT_0 0x22
#define BMX055_ACC_INT_1 0x23
#define BMX055_ACC_INT_2 0x24
#define BMX055_ACC_INT_3 0x25
#define BMX055_ACC_INT_4 0x26
#define BMX055_ACC_INT_5 0x27
#define BMX055_ACC_INT_6 0x28
#define BMX055_ACC_INT_7 0x29
#define BMX055_ACC_INT_8 0x2A
#define BMX055_ACC_INT_9 0x2B
#define BMX055_ACC_INT_A 0x2C
#define BMX055_ACC_INT_B 0x2D
#define BMX055_ACC_INT_C 0x2E
#define BMX055_ACC_INT_D 0x2F
#define BMX055_ACC_FIFO_CONFIG_0 0x30
//#define BMX055_ACC_Reserved    0x31
#define BMX055_ACC_PMU_SELF_TEST 0x32
#define BMX055_ACC_TRIM_NVM_CTRL 0x33
#define BMX055_ACC_BGW_SPI3_WDT 0x34
//#define BMX055_ACC_Reserved    0x35
#define BMX055_ACC_OFC_CTRL 0x36
#define BMX055_ACC_OFC_SETTING 0x37
#define BMX055_ACC_OFC_OFFSET_X 0x38
#define BMX055_ACC_OFC_OFFSET_Y 0x39
#define BMX055_ACC_OFC_OFFSET_Z 0x3A
#define BMX055_ACC_TRIM_GPO 0x3B
#define BMX055_ACC_TRIM_GP1 0x3C
//#define BMX055_ACC_Reserved    0x3D
#define BMX055_ACC_FIFO_CONFIG_1 0x3E
#define BMX055_ACC_FIFO_DATA 0x3F

// BMX055 Gyroscope Registers
#define BMX055_GYRO_WHOAMI 0x00 // should return 0x0F
//#define BMX055_GYRO_Reserved       0x01
#define BMX055_GYRO_RATE_X_LSB 0x02
#define BMX055_GYRO_RATE_X_MSB 0x03
#define BMX055_GYRO_RATE_Y_LSB 0x04
#define BMX055_GYRO_RATE_Y_MSB 0x05
#define BMX055_GYRO_RATE_Z_LSB 0x06
#define BMX055_GYRO_RATE_Z_MSB 0x07
//#define BMX055_GYRO_Reserved       0x08
#define BMX055_GYRO_INT_STATUS_0 0x09
#define BMX055_GYRO_INT_STATUS_1 0x0A
#define BMX055_GYRO_INT_STATUS_2 0x0B
#define BMX055_GYRO_INT_STATUS_3 0x0C
//#define BMX055_GYRO_Reserved    0x0D
#define BMX055_GYRO_FIFO_STATUS 0x0E
#define BMX055_GYRO_RANGE 0x0F
#define BMX055_GYRO_BW 0x10
#define BMX055_GYRO_LPM1 0x11
#define BMX055_GYRO_LPM2 0x12
#define BMX055_GYRO_RATE_HBW 0x13
#define BMX055_GYRO_BGW_SOFTRESET 0x14
#define BMX055_GYRO_INT_EN_0 0x15
#define BMX055_GYRO_INT_EN_1 0x16
#define BMX055_GYRO_INT_MAP_0 0x17
#define BMX055_GYRO_INT_MAP_1 0x18
#define BMX055_GYRO_INT_MAP_2 0x19
#define BMX055_GYRO_INT_SRC_1 0x1A
#define BMX055_GYRO_INT_SRC_2 0x1B
#define BMX055_GYRO_INT_SRC_3 0x1C
//#define BMX055_GYRO_Reserved    0x1D
#define BMX055_GYRO_FIFO_EN 0x1E
//#define BMX055_GYRO_Reserved    0x1F
//#define BMX055_GYRO_Reserved    0x20
#define BMX055_GYRO_INT_RST_LATCH 0x21
#define BMX055_GYRO_HIGH_TH_X 0x22
#define BMX055_GYRO_HIGH_DUR_X 0x23
#define BMX055_GYRO_HIGH_TH_Y 0x24
#define BMX055_GYRO_HIGH_DUR_Y 0x25
#define BMX055_GYRO_HIGH_TH_Z 0x26
#define BMX055_GYRO_HIGH_DUR_Z 0x27
//#define BMX055_GYRO_Reserved    0x28
//#define BMX055_GYRO_Reserved    0x29
//#define BMX055_GYRO_Reserved    0x2A
#define BMX055_GYRO_SOC 0x31
#define BMX055_GYRO_A_FOC 0x32
#define BMX055_GYRO_TRIM_NVM_CTRL 0x33
#define BMX055_GYRO_BGW_SPI3_WDT 0x34
//#define BMX055_GYRO_Reserved    0x35
#define BMX055_GYRO_OFC1 0x36
#define BMX055_GYRO_OFC2 0x37
#define BMX055_GYRO_OFC3 0x38
#define BMX055_GYRO_OFC4 0x39
#define BMX055_GYRO_TRIM_GP0 0x3A
#define BMX055_GYRO_TRIM_GP1 0x3B
#define BMX055_GYRO_BIST 0x3C
#define BMX055_GYRO_FIFO_CONFIG_0 0x3D
#define BMX055_GYRO_FIFO_CONFIG_1 0x3E

// BMX055 magnetometer registers
#define BMX055_MAG_WHOAMI 0x40 // should return 0x32
#define BMX055_MAG_Reserved 0x41
#define BMX055_MAG_XOUT_LSB 0x42
#define BMX055_MAG_XOUT_MSB 0x43
#define BMX055_MAG_YOUT_LSB 0x44
#define BMX055_MAG_YOUT_MSB 0x45
#define BMX055_MAG_ZOUT_LSB 0x46
#define BMX055_MAG_ZOUT_MSB 0x47
#define BMX055_MAG_ROUT_LSB 0x48
#define BMX055_MAG_ROUT_MSB 0x49
#define BMX055_MAG_INT_STATUS 0x4A
#define BMX055_MAG_PWR_CNTL1 0x4B
#define BMX055_MAG_PWR_CNTL2 0x4C
#define BMX055_MAG_INT_EN_1 0x4D
#define BMX055_MAG_INT_EN_2 0x4E
#define BMX055_MAG_LOW_THS 0x4F
#define BMX055_MAG_HIGH_THS 0x50
#define BMX055_MAG_REP_XY 0x51
#define BMX055_MAG_REP_Z 0x52
/* Trim Extended Registers */
#define BMM050_DIG_X1 0x5D // needed for magnetic field calculation
#define BMM050_DIG_Y1 0x5E
#define BMM050_DIG_Z4_LSB 0x62
#define BMM050_DIG_Z4_MSB 0x63
#define BMM050_DIG_X2 0x64
#define BMM050_DIG_Y2 0x65
#define BMM050_DIG_Z2_LSB 0x68
#define BMM050_DIG_Z2_MSB 0x69
#define BMM050_DIG_Z1_LSB 0x6A
#define BMM050_DIG_Z1_MSB 0x6B
#define BMM050_DIG_XYZ1_LSB 0x6C
#define BMM050_DIG_XYZ1_MSB 0x6D
#define BMM050_DIG_Z3_LSB 0x6E
#define BMM050_DIG_Z3_MSB 0x6F
#define BMM050_DIG_XY2 0x70
#define BMM050_DIG_XY1 0x71

// SDO1 = SDO2 = CSB3 = GND as designed
// Seven-bit device addresses are ACC = 0x18, GYRO = 0x68, MAG = 0x10
#define BMX055_ACC_ADDRESS 0x18  // Address of BMX055 accelerometer
#define BMX055_GYRO_ADDRESS 0x68 // Address of BMX055 gyroscope
#define BMX055_MAG_ADDRESS 0x10  // Address of BMX055 magnetometer

#define SerialDebug true // set to true to get Serial output for debugging

// Set initial input parameters
// define BMX055 ACC full scale options
#define AFS_2G 0x03
#define AFS_4G 0x05
#define AFS_8G 0x08
#define AFS_16G 0x0C

enum ACCBW
{            // define BMX055 accelerometer bandwidths
  ABW_8Hz,   // 7.81 Hz,  64 ms update time
  ABW_16Hz,  // 15.63 Hz, 32 ms update time
  ABW_31Hz,  // 31.25 Hz, 16 ms update time
  ABW_63Hz,  // 62.5  Hz,  8 ms update time
  ABW_125Hz, // 125   Hz,  4 ms update time
  ABW_250Hz, // 250   Hz,  2 ms update time
  ABW_500Hz, // 500   Hz,  1 ms update time
  ABW_100Hz  // 1000  Hz,  0.5 ms update time
};

enum Gscale
{
  GFS_2000DPS = 0,
  GFS_1000DPS,
  GFS_500DPS,
  GFS_250DPS,
  GFS_125DPS
};

enum GODRBW
{
  G_2000Hz523Hz = 0, // 2000 Hz ODR and unfiltered (bandwidth 523Hz)
  G_2000Hz230Hz,
  G_1000Hz116Hz,
  G_400Hz47Hz,
  G_200Hz23Hz,
  G_100Hz12Hz,
  G_200Hz64Hz,
  G_100Hz32Hz // 100 Hz ODR and 32 Hz bandwidth
};

enum MODR
{
  MODR_10Hz = 0, // 10 Hz ODR
  MODR_2Hz,      // 2 Hz ODR
  MODR_6Hz,      // 6 Hz ODR
  MODR_8Hz,      // 8 Hz ODR
  MODR_15Hz,     // 15 Hz ODR
  MODR_20Hz,     // 20 Hz ODR
  MODR_25Hz,     // 25 Hz ODR
  MODR_30Hz      // 30 Hz ODR
};

enum Mmode
{
  lowPower = 0,    // rms noise ~1.0 microTesla, 0.17 mA power
  Regular,         // rms noise ~0.6 microTesla, 0.5 mA power
  enhancedRegular, // rms noise ~0.5 microTesla, 0.8 mA power
  highAccuracy     // rms noise ~0.3 microTesla, 4.9 mA power
};

// Specify sensor full scale
uint8_t Gscale = GFS_125DPS;     // set gyro full scale
uint8_t GODRBW = G_200Hz23Hz;    // set gyro ODR and bandwidth
uint8_t Ascale = AFS_2G;         // set accel full scale
uint8_t ACCBW = 0x08 | ABW_16Hz; // Choose bandwidth for accelerometer, need bit 3 = 1 to enable bandwidth choice in enum
uint8_t Mmode = Regular;         // Choose magnetometer operation mode
uint8_t MODR = MODR_10Hz;        // set magnetometer data rate
float aRes, gRes, mRes;          // scale resolutions per LSB for the sensors

// Parameters to hold BMX055 trim values
signed char dig_x1;
signed char dig_y1;
signed char dig_x2;
signed char dig_y2;
uint16_t dig_z1;
int16_t dig_z2;
int16_t dig_z3;
int16_t dig_z4;
unsigned char dig_xy1;
signed char dig_xy2;
uint16_t dig_xyz1;

//===================================================================================================================
//====== Set of useful function to access acceleration. gyroscope, magnetometer, and temperature data
//===================================================================================================================

// I2C read/write functions for the MPU9250 and AK8963 sensors

void writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
  Wire.beginTransmission(address); // Initialize the Tx buffer
  Wire.write(subAddress);          // Put slave register address in Tx buffer
  Wire.write(data);                // Put data in Tx buffer
  Wire.endTransmission();          // Send the Tx buffer
}

uint8_t readByte(uint8_t address, uint8_t subAddress)
{
  uint8_t data;                         // `data` will store the register data
  Wire.beginTransmission(address);      // Initialize the Tx buffer
  Wire.write(subAddress);               // Put slave register address in Tx buffer
  Wire.endTransmission(I2C_NOSTOP);     // Send the Tx buffer, but send a restart to keep connection alive
                                        //	Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
                                        //	Wire.requestFrom(address, 1);  // Read one byte from slave register address
  Wire.requestFrom(address, (size_t)1); // Read one byte from slave register address
  data = Wire.read();                   // Fill Rx buffer with result
  return data;                          // Return data read from slave register
}

void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t *dest)
{
  Wire.beginTransmission(address);  // Initialize the Tx buffer
  Wire.write(subAddress);           // Put slave register address in Tx buffer
  Wire.endTransmission(I2C_NOSTOP); // Send the Tx buffer, but send a restart to keep connection alive
                                    //	Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
  uint8_t i = 0;
  //        Wire.requestFrom(address, count);  // Read bytes from slave register address
  Wire.requestFrom(address, (size_t)count); // Read bytes from slave register address
  while (Wire.available())
  {
    dest[i++] = Wire.read();
  } // Put read results in the Rx buffer
}

void getGres()
{
  switch (Gscale)
  {
    // Possible gyro scales (and their register bit settings) are:
    // 125 DPS (100), 250 DPS (011), 500 DPS (010), 1000 DPS (001), and 2000 DPS (000).
  case GFS_125DPS:
    gRes = 124.87 / 32768.0; // per data sheet, not exactly 125!?
    break;
  case GFS_250DPS:
    gRes = 249.75 / 32768.0;
    break;
  case GFS_500DPS:
    gRes = 499.5 / 32768.0;
    break;
  case GFS_1000DPS:
    gRes = 999.0 / 32768.0;
    break;
  case GFS_2000DPS:
    gRes = 1998.0 / 32768.0;
    break;
  }
}

void getAres()
{
  switch (Ascale)
  {
    // Possible accelerometer scales (and their register bit settings) are:
    // 2 Gs (0011), 4 Gs (0101), 8 Gs (1000), and 16 Gs  (1100).
    // BMX055 ACC data is signed 12 bit
  case AFS_2G:
    aRes = 2.0 / 2048.0;
    break;
  case AFS_4G:
    aRes = 4.0 / 2048.0;
    break;
  case AFS_8G:
    aRes = 8.0 / 2048.0;
    break;
  case AFS_16G:
    aRes = 16.0 / 2048.0;
    break;
  }
}

void readAccelData(int16_t *destination)
{
  uint8_t rawData[6];                                                // x/y/z accel register data stored here
  readBytes(BMX055_ACC_ADDRESS, BMX055_ACC_D_X_LSB, 6, &rawData[0]); // Read the six raw data registers into data array
  if ((rawData[0] & 0x01) && (rawData[2] & 0x01) && (rawData[4] & 0x01))
  {                                                                           // Check that all 3 axes have new data
    destination[0] = (int16_t)(((int16_t)rawData[1] << 8) | rawData[0]) >> 4; // Turn the MSB and LSB into a signed 12-bit value
    destination[1] = (int16_t)(((int16_t)rawData[3] << 8) | rawData[2]) >> 4;
    destination[2] = (int16_t)(((int16_t)rawData[5] << 8) | rawData[4]) >> 4;
  }
}

void readGyroData(int16_t *destination)
{
  uint8_t rawData[6];                                                     // x/y/z gyro register data stored here
  readBytes(BMX055_GYRO_ADDRESS, BMX055_GYRO_RATE_X_LSB, 6, &rawData[0]); // Read the six raw data registers sequentially into data array
  destination[0] = (int16_t)(((int16_t)rawData[1] << 8) | rawData[0]);    // Turn the MSB and LSB into a signed 16-bit value
  destination[1] = (int16_t)(((int16_t)rawData[3] << 8) | rawData[2]);
  destination[2] = (int16_t)(((int16_t)rawData[5] << 8) | rawData[4]);
}

void readMagData(int16_t *magData)
{
  int16_t mdata_x = 0, mdata_y = 0, mdata_z = 0, temp = 0;
  uint16_t data_r = 0;
  uint8_t rawData[8];                                                 // x/y/z hall magnetic field data, and Hall resistance data
  readBytes(BMX055_MAG_ADDRESS, BMX055_MAG_XOUT_LSB, 8, &rawData[0]); // Read the eight raw data registers sequentially into data array
  if (rawData[6] & 0x01)
  {                                                                     // Check if data ready status bit is set
    mdata_x = (int16_t)(((int16_t)rawData[1] << 8) | rawData[0]) >> 3;  // 13-bit signed integer for x-axis field
    mdata_y = (int16_t)(((int16_t)rawData[3] << 8) | rawData[2]) >> 3;  // 13-bit signed integer for y-axis field
    mdata_z = (int16_t)(((int16_t)rawData[5] << 8) | rawData[4]) >> 1;  // 15-bit signed integer for z-axis field
    data_r = (uint16_t)(((uint16_t)rawData[7] << 8) | rawData[6]) >> 2; // 14-bit unsigned integer for Hall resistance

    // calculate temperature compensated 16-bit magnetic fields
    temp = ((int16_t)(((uint16_t)((((int32_t)dig_xyz1) << 14) / (data_r != 0 ? data_r : dig_xyz1))) - ((uint16_t)0x4000)));
    magData[0] = ((int16_t)((((int32_t)mdata_x) *
                             ((((((((int32_t)dig_xy2) * ((((int32_t)temp) * ((int32_t)temp)) >> 7)) +
                                  (((int32_t)temp) * ((int32_t)(((int16_t)dig_xy1) << 7)))) >>
                                 9) +
                                ((int32_t)0x100000)) *
                               ((int32_t)(((int16_t)dig_x2) + ((int16_t)0xA0)))) >>
                              12)) >>
                            13)) +
                 (((int16_t)dig_x1) << 3);

    temp = ((int16_t)(((uint16_t)((((int32_t)dig_xyz1) << 14) / (data_r != 0 ? data_r : dig_xyz1))) - ((uint16_t)0x4000)));
    magData[1] = ((int16_t)((((int32_t)mdata_y) *
                             ((((((((int32_t)dig_xy2) * ((((int32_t)temp) * ((int32_t)temp)) >> 7)) +
                                  (((int32_t)temp) * ((int32_t)(((int16_t)dig_xy1) << 7)))) >>
                                 9) +
                                ((int32_t)0x100000)) *
                               ((int32_t)(((int16_t)dig_y2) + ((int16_t)0xA0)))) >>
                              12)) >>
                            13)) +
                 (((int16_t)dig_y1) << 3);
    magData[2] = (((((int32_t)(mdata_z - dig_z4)) << 15) - ((((int32_t)dig_z3) * ((int32_t)(((int16_t)data_r) -
                                                                                            ((int16_t)dig_xyz1)))) >>
                                                            2)) /
                  (dig_z2 + ((int16_t)(((((int32_t)dig_z1) * ((((int16_t)data_r) << 1))) + (1 << 15)) >> 16))));
  }
}

void trimBMX055() // get trim values for magnetometer sensitivity
{
  uint8_t rawData[2]; //placeholder for 2-byte trim data
  dig_x1 = readByte(BMX055_ACC_ADDRESS, BMM050_DIG_X1);
  dig_x2 = readByte(BMX055_ACC_ADDRESS, BMM050_DIG_X2);
  dig_y1 = readByte(BMX055_ACC_ADDRESS, BMM050_DIG_Y1);
  dig_y2 = readByte(BMX055_ACC_ADDRESS, BMM050_DIG_Y2);
  dig_xy1 = readByte(BMX055_ACC_ADDRESS, BMM050_DIG_XY1);
  dig_xy2 = readByte(BMX055_ACC_ADDRESS, BMM050_DIG_XY2);
  readBytes(BMX055_MAG_ADDRESS, BMM050_DIG_Z1_LSB, 2, &rawData[0]);
  dig_z1 = (uint16_t)(((uint16_t)rawData[1] << 8) | rawData[0]);
  readBytes(BMX055_MAG_ADDRESS, BMM050_DIG_Z2_LSB, 2, &rawData[0]);
  dig_z2 = (int16_t)(((int16_t)rawData[1] << 8) | rawData[0]);
  readBytes(BMX055_MAG_ADDRESS, BMM050_DIG_Z3_LSB, 2, &rawData[0]);
  dig_z3 = (int16_t)(((int16_t)rawData[1] << 8) | rawData[0]);
  readBytes(BMX055_MAG_ADDRESS, BMM050_DIG_Z4_LSB, 2, &rawData[0]);
  dig_z4 = (int16_t)(((int16_t)rawData[1] << 8) | rawData[0]);
  readBytes(BMX055_MAG_ADDRESS, BMM050_DIG_XYZ1_LSB, 2, &rawData[0]);
  dig_xyz1 = (uint16_t)(((uint16_t)rawData[1] << 8) | rawData[0]);
}

void initBMX055()
{
  // start with all sensors in default mode with all registers reset
  writeByte(BMX055_ACC_ADDRESS, BMX055_ACC_BGW_SOFTRESET, 0xB6); // reset accelerometer
  delay(1000);                                                   // Wait for all registers to reset

  // Configure accelerometer
  writeByte(BMX055_ACC_ADDRESS, BMX055_ACC_PMU_RANGE, Ascale & 0x0F); // Set accelerometer full range
  writeByte(BMX055_ACC_ADDRESS, BMX055_ACC_PMU_BW, ACCBW & 0x0F);     // Set accelerometer bandwidth
  writeByte(BMX055_ACC_ADDRESS, BMX055_ACC_D_HBW, 0x00);              // Use filtered data

  // Configure Gyro
  writeByte(BMX055_GYRO_ADDRESS, BMX055_GYRO_RANGE, Gscale); // set GYRO FS range
  writeByte(BMX055_GYRO_ADDRESS, BMX055_GYRO_BW, GODRBW);    // set GYRO ODR and Bandwidth

  // Configure magnetometer
  writeByte(BMX055_MAG_ADDRESS, BMX055_MAG_PWR_CNTL1, 0x82); // Softreset magnetometer, ends up in sleep mode
  delay(100);
  writeByte(BMX055_MAG_ADDRESS, BMX055_MAG_PWR_CNTL1, 0x01); // Wake up magnetometer
  delay(100);

  writeByte(BMX055_MAG_ADDRESS, BMX055_MAG_PWR_CNTL2, MODR << 3); // Normal mode

  // Set up four standard configurations for the magnetometer
  switch (Mmode)
  {
  case lowPower:
    // Low-power
    writeByte(BMX055_MAG_ADDRESS, BMX055_MAG_REP_XY, 0x01); // 3 repetitions (oversampling)
    writeByte(BMX055_MAG_ADDRESS, BMX055_MAG_REP_Z, 0x02);  // 3 repetitions (oversampling)
    break;
  case Regular:
    // Regular
    writeByte(BMX055_MAG_ADDRESS, BMX055_MAG_REP_XY, 0x04); //  9 repetitions (oversampling)
    writeByte(BMX055_MAG_ADDRESS, BMX055_MAG_REP_Z, 0x16);  // 15 repetitions (oversampling)
    break;
  case enhancedRegular:
    // Enhanced Regular
    writeByte(BMX055_MAG_ADDRESS, BMX055_MAG_REP_XY, 0x07); // 15 repetitions (oversampling)
    writeByte(BMX055_MAG_ADDRESS, BMX055_MAG_REP_Z, 0x22);  // 27 repetitions (oversampling)
    break;
  case highAccuracy:
    // High Accuracy
    writeByte(BMX055_MAG_ADDRESS, BMX055_MAG_REP_XY, 0x17); // 47 repetitions (oversampling)
    writeByte(BMX055_MAG_ADDRESS, BMX055_MAG_REP_Z, 0x51);  // 83 repetitions (oversampling)
    break;
  }
}

void fastcompaccelBMX055(float *dest1)
{
  writeByte(BMX055_ACC_ADDRESS, BMX055_ACC_OFC_CTRL, 0x80);    // set all accel offset compensation registers to zero
  writeByte(BMX055_ACC_ADDRESS, BMX055_ACC_OFC_SETTING, 0x20); // set offset targets to 0, 0, and +1 g for x, y, z axes
  writeByte(BMX055_ACC_ADDRESS, BMX055_ACC_OFC_CTRL, 0x20);    // calculate x-axis offset

  byte c = readByte(BMX055_ACC_ADDRESS, BMX055_ACC_OFC_CTRL);
  while (!(c & 0x10))
  { // check if fast calibration complete
    c = readByte(BMX055_ACC_ADDRESS, BMX055_ACC_OFC_CTRL);
    delay(10);
  }
  writeByte(BMX055_ACC_ADDRESS, BMX055_ACC_OFC_CTRL, 0x40); // calculate y-axis offset

  c = readByte(BMX055_ACC_ADDRESS, BMX055_ACC_OFC_CTRL);
  while (!(c & 0x10))
  { // check if fast calibration complete
    c = readByte(BMX055_ACC_ADDRESS, BMX055_ACC_OFC_CTRL);
    delay(10);
  }
  writeByte(BMX055_ACC_ADDRESS, BMX055_ACC_OFC_CTRL, 0x60); // calculate z-axis offset

  c = readByte(BMX055_ACC_ADDRESS, BMX055_ACC_OFC_CTRL);
  while (!(c & 0x10))
  { // check if fast calibration complete
    c = readByte(BMX055_ACC_ADDRESS, BMX055_ACC_OFC_CTRL);
    delay(10);
  }

  int8_t compx = readByte(BMX055_ACC_ADDRESS, BMX055_ACC_OFC_OFFSET_X);
  int8_t compy = readByte(BMX055_ACC_ADDRESS, BMX055_ACC_OFC_OFFSET_Y);
  int8_t compz = readByte(BMX055_ACC_ADDRESS, BMX055_ACC_OFC_OFFSET_Z);

  dest1[0] = (float)compx / 128.; // accleration bias in g
  dest1[1] = (float)compy / 128.; // accleration bias in g
  dest1[2] = (float)compz / 128.; // accleration bias in g
}

void magcalBMX055(float *dest1)
{
  uint16_t ii = 0, sample_count = 0;
  int32_t mag_bias[3] = {0, 0, 0};
  int16_t mag_max[3] = {0, 0, 0}, mag_min[3] = {0, 0, 0};

  Serial.println("Mag Calibration: Wave device in a figure eight until done!");
  delay(4000);

  sample_count = 128;
  for (ii = 0; ii < sample_count; ii++)
  {
    int16_t mag_temp[3] = {0, 0, 0};
    readMagData(mag_temp);
    for (int jj = 0; jj < 3; jj++)
    {
      if (mag_temp[jj] > mag_max[jj])
        mag_max[jj] = mag_temp[jj];
      if (mag_temp[jj] < mag_min[jj])
        mag_min[jj] = mag_temp[jj];
    }
    delay(105); // at 10 Hz ODR, new mag data is available every 100 ms
  }

  mag_bias[0] = (mag_max[0] + mag_min[0]) / 2; // get average x mag bias in counts
  mag_bias[1] = (mag_max[1] + mag_min[1]) / 2; // get average y mag bias in counts
  mag_bias[2] = (mag_max[2] + mag_min[2]) / 2; // get average z mag bias in counts

  dest1[0] = (float)mag_bias[0] * mRes; // save mag biases in G for main program
  dest1[1] = (float)mag_bias[1] * mRes;
  dest1[2] = (float)mag_bias[2] * mRes;

  Serial.println("Mag Calibration done!");
}

//===================================================================================================================
//===================================================================================================================
//===================================================================================================================