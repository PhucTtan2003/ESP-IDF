#include <adxl345.h>
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"

static const char *TAG = "i2c-simple-example";
const float kRatio  = (float) (4 * 2) / 1024.0f;

/**
 * @brief Read a sequence of bytes from a ADXL345 sensor registers
 */
void adxl345_register_read(uint8_t reg_addr, uint8_t *data, size_t len)
{
    i2c_master_write_read_device(I2C_MASTER_NUM, ADXL345_SENSOR_ADDR, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

/**
 * @brief Write a byte to a ADXL345 sensor register
 */
void adxl345_register_write_byte(uint8_t reg_addr, uint8_t data)
{
    uint8_t write_buf[2] = {reg_addr, data};

	i2c_master_write_to_device(I2C_MASTER_NUM, ADXL345_SENSOR_ADDR, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

void adxl345_init(void )
{
	adxl345_register_write_byte(0x2D, 1 << 3);
	adxl345_register_write_byte(0x31, 0x01); // 0x01 4g
	adxl345_register_write_byte(0x2C, 0x09); // Data rate and Low Power Mode. 0x09: Low Power Off, Rate 50Hz
}

void adxl345_get_accel(float *result) {
	uint8_t regAddress = 0x32;
	uint8_t buff[6];
	adxl345_register_read(regAddress, buff, 6);
	int16_t raw[3];

	raw[0] = twosCompToDec((uint16_t) (((int) buff[1]) << 8) | buff[0]);
	raw[1] = twosCompToDec((uint16_t) (((int) buff[3]) << 8) | buff[2]);
	raw[2] = twosCompToDec((uint16_t) (((int) buff[5]) << 8) | buff[4]);
	
	result[0] = kRatio * (float) raw[0];
	result[1] = kRatio * (float) raw[1];
	result[2] = kRatio * (float) raw[2];
}

int16_t twosCompToDec(uint16_t two_compliment_val)
{
    // [0x0000; 0x7FFF] corresponds to [0; 32,767]
    // [0x8000; 0xFFFF] corresponds to [-32,768; -1]
    // int16_t has the range [-32,768; 32,767]

    uint16_t sign_mask = 0x8000;

    // if positive
    if ( (two_compliment_val & sign_mask) == 0 ) {
        return two_compliment_val;
    //  if negative
    } else {
        // invert all bits, add one, and make negative
        return -(~two_compliment_val + 1);
    }
}

/**
 * @brief i2c master initialization
 */
void i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}
