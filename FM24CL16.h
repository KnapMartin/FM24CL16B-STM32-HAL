/*
 * FM24CL16.h
 *
 *  Created on: Jan 21, 2025
 *      Author: knap-linux
 */

#ifndef FM24CL16_H_
#define FM24CL16_H_

#include "main.h"

#define FM24CL16_TIMEOUT 100 // ms

typedef enum
{
	FM24CL16_OK,
	FM24CL16_ERROR
} FM24CL16State;

typedef struct
{
	I2C_HandleTypeDef *m_hi2c;
	FM24CL16State m_state;
	HAL_StatusTypeDef m_stateHal;
} FM24CL16;

FM24CL16State FM24CL16_init(FM24CL16 *device);
FM24CL16State FM24CL16_write8(FM24CL16 *device, const uint8_t page, const uint8_t address, const uint8_t data);
FM24CL16State FM24CL16_read8(FM24CL16 *device, const uint8_t page, const uint8_t address, uint8_t *data);



#endif /* FM24CL16_H_ */
