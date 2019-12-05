/*
 * lsm330.h
 *
 *  Author: Wes Piard
 */

#ifndef LSM330_H_
#define LSM330_H_
/* -------------------------------------------------------------------------- */

/* used to differentiate the accelerometer and gyroscope within the LSM330 */
typedef enum {LSM330_ACCEL, LSM330_GYRO} lsm330_module_t;

/* can be used to contain the separated bytes of data as they are read from
   the LSM330 */
typedef struct lsm330_data_raw
{
  uint8_t accel_x_low, accel_x_high;
  uint8_t accel_y_low, accel_y_high;
  uint8_t accel_z_low, accel_z_high;

  uint8_t gyro_x_low, gyro_x_high;
  uint8_t gyro_y_low, gyro_y_high;
  uint8_t gyro_z_low, gyro_z_high;
}lsm330_data_raw_t;

/* contains the full concatenated signed 16-bit words of data */
typedef struct lsm330_data_full
{
  int16_t accel_x, accel_y, accel_z;
  int16_t gyro_x, gyro_y, gyro_z;
}lsm330_data_full_t;

/* provides the ability to choose how to access the LSM330 data */
typedef union lsm330_data
{
  lsm330_data_full_t  word;
  lsm330_data_raw_t   byte;
}lsm330_data_t;

/* -------------------------------------------------------------------------- */

/* your lsm330 function prototypes here */

//Writes a single byte of data (data) to a specific accelerometer register (reg_addr) within the LSM330.
void accel_write(uint8_t reg_addr, uint8_t data);

//Returns a single byte of data that is read from a specific accelerometer register (reg_addr) within the LSM330.
uint8_t accel_read(uint8_t reg_addr);

//Accelerometer initialize
void accel_init(void);

#endif /* LSM330_H_ */