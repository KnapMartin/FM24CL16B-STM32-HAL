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

static inline FM24CL16B_State set_head(FM24CL16B *device, const uint16_t address);

FM24CL16B_State FM24CL16B_init(FM24CL16B *device, I2C_HandleTypeDef *hi2c)
{
	device->m_hi2c = hi2c;
	sInit = FM24CL16B_INIT;

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_write8(FM24CL16B *device, const uint16_t address, const uint8_t data)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (address > FM24CL16B_ADDRESS_END) return FM24CL16B_ERROR;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t addressPageOperation = sDeviceAddressWrite;
	addressPageOperation |= (page << 1);
	device->m_bufferTx[0] = row;
	device->m_bufferTx[1] = data;
	if (HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, device->m_bufferTx, 2, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_write16(FM24CL16B *device, const uint16_t address, const uint16_t data)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (address > FM24CL16B_ADDRESS_END) return FM24CL16B_ERROR;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t addressPageOperation = sDeviceAddressWrite;
	addressPageOperation |= (page << 1);
	uint8_t data0 = (uint8_t)(data >> 8);
	uint8_t data1 = (uint8_t)(data & 0xFF);
	device->m_bufferTx[0] = row;
	device->m_bufferTx[1] = data0;
	device->m_bufferTx[2] = data1;
	if (HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, device->m_bufferTx, 3, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_write32(FM24CL16B *device, const uint16_t address, const uint32_t data)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (address > FM24CL16B_ADDRESS_END) return FM24CL16B_ERROR;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t addressPageOperation = sDeviceAddressWrite;
	addressPageOperation |= (page << 1);
	uint8_t data0 = (uint8_t)(data >> 24);
	uint8_t data1 = (uint8_t)(data >> 16);
	uint8_t data2 = (uint8_t)(data >> 8);
	uint8_t data3 = (uint8_t)(data & 0xFF);
	device->m_bufferTx[0] = row;
	device->m_bufferTx[1] = data0;
	device->m_bufferTx[2] = data1;
	device->m_bufferTx[3] = data2;
	device->m_bufferTx[4] = data3;
	if (HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, device->m_bufferTx, 5, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_read8(FM24CL16B *device, const uint16_t address, uint8_t *data)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (address > FM24CL16B_ADDRESS_END) return FM24CL16B_ERROR;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t addressPageOperation = sDeviceAddressRead;
	addressPageOperation |= (page << 1);
	device->m_bufferTx[0] = row;
	if (HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, device->m_bufferTx, 1, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	if (HAL_I2C_Master_Receive(device->m_hi2c, addressPageOperation, data, 1, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_RX;
	}

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_read16(FM24CL16B *device, const uint16_t address, uint16_t *data)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (address > FM24CL16B_ADDRESS_END) return FM24CL16B_ERROR;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t addressPageOperation = sDeviceAddressRead;
	addressPageOperation |= (page << 1);
	device->m_bufferTx[0] = row;
	if (HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, device->m_bufferTx, 1, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	if (HAL_I2C_Master_Receive(device->m_hi2c, addressPageOperation, device->m_bufferRx, 2, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_RX;
	}

	*data |= (uint16_t)(device->m_bufferRx[0] << 8);
	*data |= (uint16_t)(device->m_bufferRx[1]);

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_read32(FM24CL16B *device, const uint16_t address, uint32_t *data)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (address > FM24CL16B_ADDRESS_END) return FM24CL16B_ERROR;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t addressPageOperation = sDeviceAddressRead;
	addressPageOperation |= (page << 1);
	device->m_bufferTx[0] = row;
	if (HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, device->m_bufferTx, 1, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	if (HAL_I2C_Master_Receive(device->m_hi2c, addressPageOperation, device->m_bufferRx, 4, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_RX;
	}

	*data |= (uint32_t)(device->m_bufferRx[0] << 24);
	*data |= (uint32_t)(device->m_bufferRx[1] << 16);
	*data |= (uint32_t)(device->m_bufferRx[2] << 8);
	*data |= (uint32_t)(device->m_bufferRx[3]);

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

	for (uint16_t address = 0; address < FM24CL16B_ADDRESS_END + 1; address += 4)
	{
		FM24CL16B_write32(device, address, memValue);
	}

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_print(FM24CL16B *device, UART_HandleTypeDef *huart)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;

	char printBuff[30];

	if (set_head(device, 0) != FM24CL16B_OK)
	{
		return FM24CL16B_ERROR;
	}

	for (uint16_t address = 0; address < FM24CL16B_ADDRESS_END + 1; address += 8)
	{
		uint8_t page = (address >> 8) & 0x07;
		uint8_t addressPageOperation = sDeviceAddressRead;
		addressPageOperation |= (page << 1);
		// fast read with address latching
		if (HAL_I2C_Master_Receive(device->m_hi2c, addressPageOperation, device->m_bufferRx, 8, FM24CL16B_TIMEOUT) != HAL_OK)
		{
			return FM24CL16B_ERROR_RX;
		}

		sprintf(printBuff, "%04X: ", address);
		if (HAL_UART_Transmit(huart, (uint8_t*)printBuff, strlen(printBuff), 100) != HAL_OK)
		{
			return FM24CL16B_ERROR_UART;
		}

		sprintf(printBuff, "%02X %02X %02X %02X %02X %02X %02X %02X\r\n", 	device->m_bufferRx[0],
				device->m_bufferRx[1],
				device->m_bufferRx[2],
				device->m_bufferRx[3],
				device->m_bufferRx[4],
				device->m_bufferRx[5],
				device->m_bufferRx[6],
				device->m_bufferRx[7]);
		if (HAL_UART_Transmit(huart, (uint8_t*)printBuff, strlen(printBuff), 100) != HAL_OK)
		{
			return FM24CL16B_ERROR_UART;
		}
	}

	return FM24CL16B_OK;
}

#ifdef FM24CL16B_TEST_ENABLE
FM24CL16B_Test FM24CL16B_test8(FM24CL16B *device)
{
	uint8_t dataWrite = 0x66;
	uint8_t dataRead = 0x00;

	for (uint16_t address = 0; address < FM24CL16B_ADDRESS_END + 1; ++address)
	{
		if (FM24CL16B_write8(device, address, dataWrite) != FM24CL16B_OK)
		{
			return FM24CL16B_FAIL;
		}
	}
	for (uint16_t address = 0; address < FM24CL16B_ADDRESS_END + 1; ++address)
	{
		if (FM24CL16B_read8(device, address, &dataRead) != FM24CL16B_OK)
		{
			return FM24CL16B_FAIL;
		}

		if (dataRead != dataWrite)
		{
			return FM24CL16B_FAIL;
		}
		dataRead = 0x00;
	}

	FM24CL16B_reset(device, 0x00);

	return FM24CL16B_PASS;
}

FM24CL16B_Test FM24CL16B_test16(FM24CL16B *device)
{
	uint16_t dataWrite = 0x6666;
	uint16_t dataRead = 0x0000;

	for (uint16_t address = 0; address < FM24CL16B_ADDRESS_END + 1; address += 2)
	{
		if (FM24CL16B_write16(device, address, dataWrite) != FM24CL16B_OK)
		{
			return FM24CL16B_FAIL;
		}
	}
	for (uint16_t address = 0; address < FM24CL16B_ADDRESS_END + 1; address += 2)
	{
		if (FM24CL16B_read16(device, address, &dataRead) != FM24CL16B_OK)
		{
			return FM24CL16B_FAIL;
		}

		if (dataRead != dataWrite)
		{
			return FM24CL16B_FAIL;
		}
		dataRead = 0x0000;
	}

	FM24CL16B_reset(device, 0x00);

	return FM24CL16B_PASS;
}

FM24CL16B_Test FM24CL16B_test32(FM24CL16B *device)
{
	uint32_t dataWrite = 0x66666666;
	uint32_t dataRead = 0x00000000;

	for (uint16_t address = 0; address < FM24CL16B_ADDRESS_END + 1; address += 4)
	{
		if (FM24CL16B_write32(device, address, dataWrite) != FM24CL16B_OK)
		{
			return FM24CL16B_FAIL;
		}
	}
	for (uint16_t address = 0; address < FM24CL16B_ADDRESS_END + 1; address += 4)
	{
		if (FM24CL16B_read32(device, address, &dataRead) != FM24CL16B_OK)
		{
			return FM24CL16B_FAIL;
		}

		if (dataRead != dataWrite)
		{
			return FM24CL16B_FAIL;
		}
		dataRead = 0x00000000;
	}

	FM24CL16B_reset(device, 0x00);

	return FM24CL16B_PASS;
}
#endif

static inline FM24CL16B_State set_head(FM24CL16B *device, const uint16_t address)
{
	if (sInit != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (address > FM24CL16B_ADDRESS_END) return FM24CL16B_ERROR;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t addressPageOperation = sDeviceAddressWrite;
	addressPageOperation |= (page << 1);
	device->m_bufferTx[0] = row;
	if (HAL_I2C_Master_Transmit(device->m_hi2c, addressPageOperation, device->m_bufferTx, 1, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	return FM24CL16B_OK;
}


