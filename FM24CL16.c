/*
 * FM24CL16.c
 *
 *  Created on: Jan 21, 2025
 *      Author: knap-linux
 */

#include "FM24CL16.h"

#include <stdio.h>
#include <string.h>

static const uint8_t sDeviceAddressWrite	= 0xA0;
static const uint8_t sDeviceAddressRead		= 0xA1;

static inline FM24CL16State set_head(FM24CL16 *device, const uint8_t page, const uint8_t address);

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
	if (page >= FM24CL16_ADDRESS_PAGE_END)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	uint8_t addressPageOperation = sDeviceAddressWrite;
	addressPageOperation |= (page << 1);
	uint8_t bytesToWrite[2] = {address, data};
	device->m_stateHal = HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, bytesToWrite, 2, FM24CL16_TIMEOUT);
	if (device->m_stateHal != HAL_OK)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	return FM24CL16_OK;
}

FM24CL16State FM24CL16_write16(FM24CL16 *device, const uint8_t page, const uint8_t address, const uint16_t data)
{
	if (page >= FM24CL16_ADDRESS_PAGE_END)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	uint8_t addressPageOperation = sDeviceAddressWrite;
	addressPageOperation |= (page << 1);
	uint8_t data0 = (uint8_t)(data >> 8);
	uint8_t data1 = (uint8_t)(data & 0xFF);
	uint8_t bytesToWrite[3] = {address, data0, data1};
	device->m_stateHal = HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, bytesToWrite, 3, FM24CL16_TIMEOUT);
	if (device->m_stateHal != HAL_OK)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	return FM24CL16_OK;
}

FM24CL16State FM24CL16_write32(FM24CL16 *device, const uint8_t page, const uint8_t address, const uint32_t data)
{
	if (page >= FM24CL16_ADDRESS_PAGE_END)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	uint8_t addressPageOperation = sDeviceAddressWrite;
	addressPageOperation |= (page << 1);
	uint8_t data0 = (uint8_t)(data >> 24);
	uint8_t data1 = (uint8_t)(data >> 16);
	uint8_t data2 = (uint8_t)(data >> 8);
	uint8_t data3 = (uint8_t)(data & 0xFF);
	uint8_t bytesToWrite[5] = {address, data0, data1, data2, data3};
	device->m_stateHal = HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, bytesToWrite, 5, FM24CL16_TIMEOUT);
	if (device->m_stateHal != HAL_OK)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	return FM24CL16_OK;
}

FM24CL16State FM24CL16_read8(FM24CL16 *device, const uint8_t page, const uint8_t address, uint8_t *data)
{
	if (page >= FM24CL16_ADDRESS_PAGE_END)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	uint8_t addressPageOperation = sDeviceAddressRead;
	addressPageOperation |= (page << 1);

	uint8_t readAddress[1] = {address};
	device->m_stateHal = HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, readAddress, 1, FM24CL16_TIMEOUT);
	if (device->m_stateHal != HAL_OK)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	device->m_stateHal = HAL_I2C_Master_Receive(device->m_hi2c, addressPageOperation, data, 1, FM24CL16_TIMEOUT);
	if (device->m_stateHal != HAL_OK)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	return FM24CL16_OK;
}

FM24CL16State FM24CL16_read16(FM24CL16 *device, const uint8_t page, const uint8_t address, uint16_t *data)
{
	if (page >= FM24CL16_ADDRESS_PAGE_END)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	uint8_t addressPageOperation = sDeviceAddressRead;
	addressPageOperation |= (page << 1);

	uint8_t readAddress[1] = {address};
	device->m_stateHal = HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, readAddress, 1, FM24CL16_TIMEOUT);
	if (device->m_stateHal != HAL_OK)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	uint8_t recData[2];
	device->m_stateHal = HAL_I2C_Master_Receive(device->m_hi2c, addressPageOperation, recData, 2, FM24CL16_TIMEOUT);
	if (device->m_stateHal != HAL_OK)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	*data = recData[0] << 8;
	*data |= recData[1];

	return FM24CL16_OK;
}

FM24CL16State FM24CL16_read32(FM24CL16 *device, const uint8_t page, const uint8_t address, uint32_t *data)
{
	if (page >= FM24CL16_ADDRESS_PAGE_END)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	uint8_t addressPageOperation = sDeviceAddressRead;
	addressPageOperation |= (page << 1);

	uint8_t readAddress[1] = {address};
	device->m_stateHal = HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, readAddress, 1, FM24CL16_TIMEOUT);
	if (device->m_stateHal != HAL_OK)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	uint8_t recData[4];
	device->m_stateHal = HAL_I2C_Master_Receive(device->m_hi2c, addressPageOperation, recData, 4, FM24CL16_TIMEOUT);
	if (device->m_stateHal != HAL_OK)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	*data = (uint32_t)(recData[0] << 24);
	*data = (uint32_t)(recData[1] << 16);
	*data = (uint32_t)(recData[2] << 8);
	*data |= (uint32_t)recData[3];

	return FM24CL16_OK;
}

FM24CL16State FM24CL16_reset(FM24CL16 *device, const uint8_t value)
{
	uint32_t memValue = 0;
	memValue |= (uint32_t)(value << 24);
	memValue |= (uint32_t)(value << 16);
	memValue |= (uint32_t)(value << 8);
	memValue |= (uint32_t)value;
	for (uint8_t page = 0; page < FM24CL16_ADDRESS_PAGE_END; ++page)
	{
		for (uint16_t row = 0; row < FM24CL16_ADDRESS_ROW_END; row += 4)
		{
			if (FM24CL16_write32(device, page, row, memValue) != FM24CL16_OK)
			{
				device->m_state = FM24CL16_ERROR;
				return FM24CL16_ERROR;
			}
		}
	}

	return FM24CL16_OK;
}

FM24CL16State FM24CL16_print(FM24CL16 *device, UART_HandleTypeDef *huart)
{
	char printBuff[30];

	if (set_head(device, 0, 0) != FM24CL16_OK)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	for (uint8_t page = 0; page < FM24CL16_ADDRESS_PAGE_END; ++page)
	{
		for (uint16_t row = 0; row < FM24CL16_ADDRESS_ROW_END; row += 8)
		{
			uint8_t addressPageOperation = sDeviceAddressRead;
			addressPageOperation |= (page << 1);
			uint8_t bytesToRead[8];
			// fast read with address latching
			if (HAL_I2C_Master_Receive(device->m_hi2c, addressPageOperation, bytesToRead, 8, FM24CL16_TIMEOUT) != HAL_OK)
			{
				device->m_state = FM24CL16_ERROR;
				return FM24CL16_ERROR;
			}
			sprintf(printBuff, "p: %01X, a: %02X -> ", page, row);
			HAL_UART_Transmit(huart, (uint8_t*)printBuff, strlen(printBuff), 100);
			sprintf(printBuff, "%02X %02X %02X %02X %02X %02X %02X %02X\r\n", 	bytesToRead[0],
																				bytesToRead[1],
																				bytesToRead[2],
																				bytesToRead[3],
																				bytesToRead[4],
																				bytesToRead[5],
																				bytesToRead[6],
																				bytesToRead[7]);
			HAL_UART_Transmit(huart, (uint8_t*)printBuff, strlen(printBuff), 100);
		}
	}

	return FM24CL16_OK;
}

static inline FM24CL16State set_head(FM24CL16 *device, const uint8_t page, const uint8_t address)
{
	if (page >= FM24CL16_ADDRESS_PAGE_END)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	uint8_t addressPageOperation = sDeviceAddressWrite;
	addressPageOperation |= (page << 1);
	uint8_t bytesToWrite[1] = {address};
	device->m_stateHal = HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, bytesToWrite, 1, FM24CL16_TIMEOUT);
	if (device->m_stateHal != HAL_OK)
	{
		device->m_state = FM24CL16_ERROR;
		return FM24CL16_ERROR;
	}

	return FM24CL16_OK;
}


