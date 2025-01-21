/*
 * FM24CL16.c
 *
 *  Created on: Jan 21, 2025
 *      Author: knap-linux
 */

#include "FM24CL16B.h"

#include <stdio.h>
#include <string.h>

static const uint8_t sDeviceAddressWrite	= 0xA0;
static const uint8_t sDeviceAddressRead		= 0xA1;
static uint8_t sInit 						= 0;

static inline FM24CL16B_State set_head(FM24CL16B *device, const uint8_t page, const uint8_t address);

FM24CL16B_State FM24CL16B_init(FM24CL16B *device, I2C_HandleTypeDef *hi2c)
{
	device->m_hi2c = hi2c;
	sInit = FM24CL16B_INIT;

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_write8(FM24CL16B *device, const uint8_t page, const uint8_t address, const uint8_t data)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (page >= FM24CL16B_ADDRESS_PAGE_END) return FM24CL16B_ERROR;

	uint8_t addressPageOperation = sDeviceAddressWrite;
	addressPageOperation |= (page << 1);
	uint8_t bytesToWrite[2] = {address, data};
	if (HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, bytesToWrite, 2, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_write16(FM24CL16B *device, const uint8_t page, const uint8_t address, const uint16_t data)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (page >= FM24CL16B_ADDRESS_PAGE_END) return FM24CL16B_ERROR;

	uint8_t addressPageOperation = sDeviceAddressWrite;
	addressPageOperation |= (page << 1);
	uint8_t data0 = (uint8_t)(data >> 8);
	uint8_t data1 = (uint8_t)(data & 0xFF);
	uint8_t bytesToWrite[3] = {address, data0, data1};
	if (HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, bytesToWrite, 3, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_write32(FM24CL16B *device, const uint8_t page, const uint8_t address, const uint32_t data)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (page >= FM24CL16B_ADDRESS_PAGE_END) return FM24CL16B_ERROR;

	uint8_t addressPageOperation = sDeviceAddressWrite;
	addressPageOperation |= (page << 1);
	uint8_t data0 = (uint8_t)(data >> 24);
	uint8_t data1 = (uint8_t)(data >> 16);
	uint8_t data2 = (uint8_t)(data >> 8);
	uint8_t data3 = (uint8_t)(data & 0xFF);
	uint8_t bytesToWrite[5] = {address, data0, data1, data2, data3};
	if (HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, bytesToWrite, 5, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_read8(FM24CL16B *device, const uint8_t page, const uint8_t address, uint8_t *data)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (page >= FM24CL16B_ADDRESS_PAGE_END) return FM24CL16B_ERROR;

	uint8_t addressPageOperation = sDeviceAddressRead;
	addressPageOperation |= (page << 1);

	uint8_t readAddress[1] = {address};
	if (HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, readAddress, 1, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	if (HAL_I2C_Master_Receive(device->m_hi2c, addressPageOperation, data, 1, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_RX;
	}

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_read16(FM24CL16B *device, const uint8_t page, const uint8_t address, uint16_t *data)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (page >= FM24CL16B_ADDRESS_PAGE_END) return FM24CL16B_ERROR;

	uint8_t addressPageOperation = sDeviceAddressRead;
	addressPageOperation |= (page << 1);

	uint8_t readAddress[1] = {address};
	if (HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, readAddress, 1, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	uint8_t recData[2];
	if (HAL_I2C_Master_Receive(device->m_hi2c, addressPageOperation, recData, 2, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_RX;
	}

	*data = recData[0] << 8;
	*data |= recData[1];

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_read32(FM24CL16B *device, const uint8_t page, const uint8_t address, uint32_t *data)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (page >= FM24CL16B_ADDRESS_PAGE_END) return FM24CL16B_ERROR;

	uint8_t addressPageOperation = sDeviceAddressRead;
	addressPageOperation |= (page << 1);

	uint8_t readAddress[1] = {address};
	if (HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, readAddress, 1, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	uint8_t recData[4];
	if (HAL_I2C_Master_Receive(device->m_hi2c, addressPageOperation, recData, 4, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_RX;
	}


	*data |= (uint32_t)(recData[0] << 24);
	*data |= (uint32_t)(recData[1] << 16);
	*data |= (uint32_t)(recData[2] << 8);
	*data |= (uint32_t)recData[3];

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_reset(FM24CL16B *device, const uint8_t value)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;

	uint32_t memValue = 0;
	memValue |= (uint32_t)(value << 24);
	memValue |= (uint32_t)(value << 16);
	memValue |= (uint32_t)(value << 8);
	memValue |= (uint32_t)value;
	for (uint8_t page = 0; page < FM24CL16B_ADDRESS_PAGE_END; ++page)
	{
		for (uint16_t row = 0; row < FM24CL16B_ADDRESS_ROW_END; row += 4)
		{
			if (FM24CL16B_write32(device, page, row, memValue) != FM24CL16B_OK)
			{
				return FM24CL16B_ERROR;
			}
		}
	}

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_print(FM24CL16B *device, UART_HandleTypeDef *huart)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;

	char printBuff[30];

	if (set_head(device, 0, 0) != FM24CL16B_OK)
	{
		return FM24CL16B_ERROR;
	}

	for (uint8_t page = 0; page < FM24CL16B_ADDRESS_PAGE_END; ++page)
	{
		for (uint16_t row = 0; row < FM24CL16B_ADDRESS_ROW_END; row += 8)
		{
			uint8_t addressPageOperation = sDeviceAddressRead;
			addressPageOperation |= (page << 1);
			uint8_t bytesToRead[8];
			// fast read with address latching
			if (HAL_I2C_Master_Receive(device->m_hi2c, addressPageOperation, bytesToRead, 8, FM24CL16B_TIMEOUT) != HAL_OK)
			{
				return FM24CL16B_ERROR_RX;
			}

			sprintf(printBuff, "p: %01X, a: %02X -> ", page, row);
			if (HAL_UART_Transmit(huart, (uint8_t*)printBuff, strlen(printBuff), 100) != HAL_OK)
			{
				return FM24CL16B_ERROR_UART;
			}

			sprintf(printBuff, "%02X %02X %02X %02X %02X %02X %02X %02X\r\n", 	bytesToRead[0],
																				bytesToRead[1],
																				bytesToRead[2],
																				bytesToRead[3],
																				bytesToRead[4],
																				bytesToRead[5],
																				bytesToRead[6],
																				bytesToRead[7]);
			if (HAL_UART_Transmit(huart, (uint8_t*)printBuff, strlen(printBuff), 100) != HAL_OK)
			{
				return FM24CL16B_ERROR_UART;
			}
		}
	}

	return FM24CL16B_OK;
}

FM24CL16B_Test FM24CL16B_test8(FM24CL16B *device)
{
	uint8_t dataWrite = 0x66;
	if (FM24CL16B_write8(device, 6, 66, dataWrite) != FM24CL16B_OK)
	{
		return FM24CL16B_FAIL;
	}
	uint8_t dataRead = 0;
	if (FM24CL16B_read8(device, 6, 66, &dataRead) != FM24CL16B_OK)
	{
		return FM24CL16B_FAIL;
	}

	if (dataWrite != dataRead) return FM24CL16B_FAIL;


	return FM24CL16B_PASS;
}

FM24CL16B_Test FM24CL16B_test16(FM24CL16B *device)
{
	uint16_t dataWrite = 0x6677;
	if (FM24CL16B_write16(device, 4, 42, dataWrite) != FM24CL16B_OK)
	{
		return FM24CL16B_FAIL;
	}
	uint16_t dataRead = 0;
	if (FM24CL16B_read16(device, 4, 42, &dataRead) != FM24CL16B_OK)
	{
		return FM24CL16B_FAIL;
	}

	if (dataWrite != dataRead) return FM24CL16B_FAIL;


	return FM24CL16B_PASS;
}

FM24CL16B_Test FM24CL16B_test32(FM24CL16B *device)
{
	uint32_t dataWrite = 0x44556677;
	if (FM24CL16B_write32(device, 2, 100, dataWrite) != FM24CL16B_OK)
	{
		return FM24CL16B_FAIL;
	}
	uint32_t dataRead = 0;
	if (FM24CL16B_read32(device, 2, 100, &dataRead) != FM24CL16B_OK)
	{
		return FM24CL16B_FAIL;
	}

	if (dataWrite != dataRead) return FM24CL16B_FAIL;


	return FM24CL16B_PASS;
}

static inline FM24CL16B_State set_head(FM24CL16B *device, const uint8_t page, const uint8_t address)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (page >= FM24CL16B_ADDRESS_PAGE_END) return FM24CL16B_ERROR;

	uint8_t addressPageOperation = sDeviceAddressWrite;
	addressPageOperation |= (page << 1);
	uint8_t bytesToWrite[1] = {address};
	if (HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, bytesToWrite, 1, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	return FM24CL16B_OK;
}


