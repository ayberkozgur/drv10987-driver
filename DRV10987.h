#pragma once

#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_i2c.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef void* DRV10987_c_ptr;

	DRV10987_c_ptr DRV10987_instance(I2C_HandleTypeDef i2c);
	void DRV10987_delete(DRV10987_c_ptr dev);


	void DRV10987_printEEPROMConfig(DRV10987_c_ptr dev);
	void DRV10987_writeDefaultEEPROMConfig(DRV10987_c_ptr dev);
	void DRV10987_testOpenLoop(DRV10987_c_ptr dev);

#ifdef __cplusplus
}
#endif
