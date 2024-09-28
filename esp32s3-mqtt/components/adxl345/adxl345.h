#ifndef __ADXL345_H__
#define __ADXL345_H__
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "esp_log.h"

#define I2C_MASTER_SCL_IO           9      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           8      /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define ADXL345_SENSOR_ADDR                 0x53        /*!< Slave address of the ADXL345 sensor */
#define ADXL345_WHO_AM_I_REG_ADDR           0x00        /*!< Register addresses of the "who am I" register */

#define ADXL345_PWR_MGMT_1_REG_ADDR         0x6B        /*!< Register addresses of the power managment register */
#define ADXL345_RESET_BIT                   7

void i2c_master_init(void);
int16_t twosCompToDec(uint16_t two_compliment_val);
void adxl345_init(void );
void adxl345_get_accel(float *result);
void adxl345_register_write_byte(uint8_t reg_addr, uint8_t data);
void adxl345_register_read(uint8_t reg_addr, uint8_t *data, size_t len);

#endif