/*
 * FM24CL16.c
 *
 *  Created on: Jan 21, 2025
 *      Author: knap-linux
 */

#include "FM24CL16.h"

static uint8_t sDeviceAddressWrite	= 0xA0;
static uint8_t sDeviceAddressRead	= 0xA1;

FM24CL16State FM24CL16_init(FM24CL16 *device)
{
	if (device->m_hi2c == NULL)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}
	device->m_state = FM24CL16_OK;

	return FM24CL16_OK;
}

FM24CL16State FM24CL16_write8(FM24CL16 *device, const uint8_t page, const uint8_t address, const uint8_t data)
{
	if (page >= 8) return FM24CL16_ERROR;

	uint8_t addressPageOperation = sDeviceAddressWrite;
	addressPageOperation |= (page << 1);

	uint8_t bytesToWrite[2] = {address, data};
	HAL_StatusTypeDef state = HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, bytesToWrite, 2, FM24CL16_TIMEOUT);
	device->m_stateHal = state;
	if (state != HAL_OK)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

//	if (HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, bytesToWrite, 2, FM24CL16_TIMEOUT) != HAL_OK)
//	{
//		device->m_state = FM24CL16_ERROR;
//		return FM24CL16_ERROR;
//	}
	return FM24CL16_OK;
}

FM24CL16State FM24CL16_read8(FM24CL16 *device, const uint8_t page, const uint8_t address, uint8_t *data)
{
	return FM24CL16_OK;
}
