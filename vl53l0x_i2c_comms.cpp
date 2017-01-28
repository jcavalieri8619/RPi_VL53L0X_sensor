#include "vl53l0x_i2c_platform.h"
#include "vl53l0x_def.h"
#include <cstdio>
//#define I2C_DEBUG

namespace
{
	int I2C_FD = -1;
}

int VL53L0X_i2c_init ( void )
{
	wiringPiSetupSys();

	return VL53L0X_ERROR_NONE;
}

int VL53L0X_write_multi ( uint8_t deviceAddress, uint8_t index, uint8_t *pdata, uint32_t count )
{

	if ( ( I2C_FD == -1 ) && ( ( I2C_FD = wiringPiI2CSetup ( deviceAddress ) ) == -1 ) ) {
		perror ( "i2c_init failed:" );
		return VL53L0X_ERROR_CONTROL_INTERFACE;
	}
#ifdef I2C_DEBUG
	//print debug
#endif

	while ( count-- ) {
		if ( wiringPiI2CWriteReg8 ( I2C_FD, index, ( ( uint8_t ) pdata[0] ) ) == -1 ) {
			perror ( "write_multi:" );
			return VL53L0X_ERROR_CONTROL_INTERFACE;
		}

#ifdef I2C_DEBUG
		//print debug
#endif
		pdata++;
	}
	return VL53L0X_ERROR_NONE;
}

int VL53L0X_read_multi ( uint8_t deviceAddress, uint8_t index, uint8_t *pdata, uint32_t count )
{


	if ( ( I2C_FD == -1 ) && ( ( I2C_FD = wiringPiI2CSetup ( deviceAddress ) ) == -1 ) ) {
		perror ( "i2c_init failed:" );
		return VL53L0X_ERROR_CONTROL_INTERFACE;
	}

#ifdef I2C_DEBUG
	//print debug
#endif

	uint8_t byte_read;
	while ( count-- ) {
		if ( ( byte_read = ( wiringPiI2CReadReg8 ( I2C_FD, index ) ) ) == -1 ) {
			perror ( "multi_read failed: " );
			return VL53L0X_ERROR_CONTROL_INTERFACE;
		}
		pdata[0] = byte_read;

#ifdef I2C_DEBUG
		//print debug
#endif

		pdata++;
	}

	return VL53L0X_ERROR_NONE;
}

int VL53L0X_write_byte ( uint8_t deviceAddress, uint8_t index, uint8_t data )
{
	return VL53L0X_write_multi ( deviceAddress, index, &data, 1 );
}

int VL53L0X_write_word ( uint8_t deviceAddress, uint8_t index, uint16_t data )
{
	uint8_t buff[2];
	buff[1] = data & 0xFF;
	buff[0] = data >> 8;
	return VL53L0X_write_multi ( deviceAddress, index, buff, 2 );
}

int VL53L0X_write_dword ( uint8_t deviceAddress, uint8_t index, uint32_t data )
{
	uint8_t buff[4];

	buff[3] = data & 0xFF;
	buff[2] = data >> 8;
	buff[1] = data >> 16;
	buff[0] = data >> 24;

	return VL53L0X_write_multi ( deviceAddress, index, buff, 4 );
}

int VL53L0X_read_byte ( uint8_t deviceAddress, uint8_t index, uint8_t *data )
{
	return VL53L0X_read_multi ( deviceAddress, index, data, 1 );
}

int VL53L0X_read_word ( uint8_t deviceAddress, uint8_t index, uint16_t *data )
{
	uint8_t buff[2];
	int r = VL53L0X_read_multi ( deviceAddress, index, buff, 2 );

	uint16_t tmp;
	tmp = buff[0];
	tmp <<= 8;
	tmp |= buff[1];
	*data = tmp;

	return r;
}

int VL53L0X_read_dword ( uint8_t deviceAddress, uint8_t index, uint32_t *data )
{
	uint8_t buff[4];
	int r = VL53L0X_read_multi ( deviceAddress, index, buff, 4 );

	uint32_t tmp;
	tmp = buff[0];
	tmp <<= 8;
	tmp |= buff[1];
	tmp <<= 8;
	tmp |= buff[2];
	tmp <<= 8;
	tmp |= buff[3];

	*data = tmp;

	return r;
}
