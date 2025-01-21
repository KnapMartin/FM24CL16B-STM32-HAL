/*
 * FM24CL16.h
 *
 *  Created on: Jan 21, 2025
 *      Author: knap-linux
 */

#ifndef FM24CL16B_H_
#define FM24CL16B_H_

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FM24CL16B_TIMEOUT 100 // ms
#define FM24CL16B_ADDRESS_PAGE_END 8
#define FM24CL16B_ADDRESS_ROW_END 256

#define FM24CL16B_INIT 1

#define FM24CL16B_TEST_ENABLE

#ifdef FM24CL16B_TEST_ENABLE
typedef enum
{
	FM24CL16B_PASS,
	FM24CL16B_FAIL
} FM24CL16B_Test;
#endif

typedef enum
{
	FM24CL16B_OK,
	FM24CL16B_ERROR,
	FM24CL16B_ERROR_TX,
	FM24CL16B_ERROR_RX,
	FM24CL16B_ERROR_UART
} FM24CL16B_State;

typedef struct
{
	I2C_HandleTypeDef *m_hi2c;
} FM24CL16B;

FM24CL16B_State FM24CL16B_init(FM24CL16B *device, I2C_HandleTypeDef *hi2c);

FM24CL16B_State FM24CL16B_write8(FM24CL16B *device, const uint8_t page, const uint8_t address, const uint8_t data);
FM24CL16B_State FM24CL16B_write16(FM24CL16B *device, const uint8_t page, const uint8_t address, const uint16_t data);
FM24CL16B_State FM24CL16B_write32(FM24CL16B *device, const uint8_t page, const uint8_t address, const uint32_t data);

FM24CL16B_State FM24CL16B_read8(FM24CL16B *device, const uint8_t page, const uint8_t address, uint8_t *data);
FM24CL16B_State FM24CL16B_read16(FM24CL16B *device, const uint8_t page, const uint8_t address, uint16_t *data);
FM24CL16B_State FM24CL16B_read32(FM24CL16B *device, const uint8_t page, const uint8_t address, uint32_t *data);

FM24CL16B_State FM24CL16B_reset(FM24CL16B *device, const uint8_t value);
FM24CL16B_State FM24CL16B_print(FM24CL16B *device, UART_HandleTypeDef *huart);

#ifdef FM24CL16B_TEST_ENABLE
FM24CL16B_Test FM24CL16B_test8(FM24CL16B *device);
FM24CL16B_Test FM24CL16B_test16(FM24CL16B *device);
FM24CL16B_Test FM24CL16B_test32(FM24CL16B *device);
#endif

#ifdef __cplusplus
}
#endif

#endif /* FM24CL16B_H_ */