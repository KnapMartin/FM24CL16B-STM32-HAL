/*
 * FM24CL16.c
 *
 *  Created on: Jan 21, 2025
 *      Author: knap-linux
 */

#include "FM24CL16B.h"

#include <stdio.h>
#include <string.h>

static const uint8_t s_device_address_write	= 0xA0;
static const uint8_t s_device_address_read	= 0xA1;
static uint8_t s_init 						= 0;

static inline FM24CL16B_State set_head(FM24CL16B *device, const uint16_t address);

FM24CL16B_State FM24CL16B_init(FM24CL16B *device, I2C_HandleTypeDef *hi2c)
{
	device->m_hi2c = hi2c;
	s_init = FM24CL16B_INIT;

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_deinit(FM24CL16B *device)
{
	device->m_hi2c = NULL;
	s_init = 0;

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_write8(FM24CL16B *device, const uint16_t address, const uint8_t data)
{
	if (s_init != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (address > FM24CL16B_ADDRESS_END) return FM24CL16B_ERROR;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t address_page_operation = s_device_address_write;
	address_page_operation |= (page << 1);

#ifdef FM24CL16B_INTERRUPTS
	uint32_t timeout = HAL_GetTick() + FM24CL16B_TIMEOUT;
	while (device->m_hi2c->State != HAL_I2C_STATE_READY)
	{
		if (HAL_GetTick() > timeout) return FM24CL16B_ERROR_TIMEOUT;
	}
#endif

	device->m_data_tx[0] = row;
	device->m_data_tx[1] = data;

#ifdef FM24CL16B_INTERRUPTS
	if (HAL_I2C_Master_Transmit_IT(device->m_hi2c, address_page_operation, device->m_data_tx, 2) != HAL_OK)
#else
	if (HAL_I2C_Master_Transmit(device->m_hi2c, address_page_operation, device->m_data_tx, 2, FM24CL16B_TIMEOUT) != HAL_OK)
#endif
	{
		return FM24CL16B_ERROR_TX;
	}

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_write16(FM24CL16B *device, const uint16_t address, const uint16_t data)
{
	if (s_init != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (address > FM24CL16B_ADDRESS_END) return FM24CL16B_ERROR;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t address_page_operation = s_device_address_write;
	address_page_operation |= (page << 1);
	uint8_t data0 = (uint8_t)(data >> 8);
	uint8_t data1 = (uint8_t)(data & 0xFF);

#ifdef FM24CL16B_INTERRUPTS
	uint32_t timeout = HAL_GetTick() + FM24CL16B_TIMEOUT;
	while (device->m_hi2c->State != HAL_I2C_STATE_READY)
	{
		if (HAL_GetTick() > timeout) return FM24CL16B_ERROR_TIMEOUT;
	}
#endif

	device->m_data_tx[0] = row;
	device->m_data_tx[1] = data0;
	device->m_data_tx[2] = data1;

#ifdef FM24CL16B_INTERRUPTS
	if (HAL_I2C_Master_Transmit_IT(device->m_hi2c, address_page_operation, device->m_data_tx, 3) != HAL_OK)
#else
	if (HAL_I2C_Master_Transmit(device->m_hi2c, address_page_operation, device->m_data_tx, 3, FM24CL16B_TIMEOUT) != HAL_OK)
#endif
	{
		return FM24CL16B_ERROR_TX;
	}

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_write32(FM24CL16B *device, const uint16_t address, const uint32_t data)
{
	if (s_init != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (address > FM24CL16B_ADDRESS_END) return FM24CL16B_ERROR;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t address_page_operation = s_device_address_write;
	address_page_operation |= (page << 1);
	uint8_t data0 = (uint8_t)(data >> 24);
	uint8_t data1 = (uint8_t)(data >> 16);
	uint8_t data2 = (uint8_t)(data >> 8);
	uint8_t data3 = (uint8_t)(data & 0xFF);

#ifdef FM24CL16B_INTERRUPTS
	uint32_t timeout = HAL_GetTick() + FM24CL16B_TIMEOUT;
	while (device->m_hi2c->State != HAL_I2C_STATE_READY)
	{
		if (HAL_GetTick() > timeout) return FM24CL16B_ERROR_TIMEOUT;
	}
#endif

	device->m_data_tx[0] = row;
	device->m_data_tx[1] = data0;
	device->m_data_tx[2] = data1;
	device->m_data_tx[3] = data2;
	device->m_data_tx[4] = data3;

#ifdef FM24CL16B_INTERRUPTS
	if (HAL_I2C_Master_Transmit_IT(device->m_hi2c, address_page_operation, device->m_data_tx, 5) != HAL_OK)
#else
	if (HAL_I2C_Master_Transmit(device->m_hi2c, address_page_operation, device->m_data_tx, 5, FM24CL16B_TIMEOUT) != HAL_OK)
#endif
	{
		return FM24CL16B_ERROR_TX;
	}

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_read8(FM24CL16B *device, const uint16_t address, uint8_t *data)
{
	if (s_init != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (address > FM24CL16B_ADDRESS_END) return FM24CL16B_ERROR;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t address_page_operation = s_device_address_read;
	address_page_operation |= (page << 1);
	device->m_data_tx[0] = row;

#ifdef FM24CL16B_INTERRUPTS
	uint32_t timeout = HAL_GetTick() + FM24CL16B_TIMEOUT;
	while (device->m_hi2c->State != HAL_I2C_STATE_READY)
	{
		if (HAL_GetTick() > timeout) return FM24CL16B_ERROR_TIMEOUT;
	}
#endif

	if (HAL_I2C_Master_Transmit(device->m_hi2c, address_page_operation, device->m_data_tx, 1, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	if (HAL_I2C_Master_Receive(device->m_hi2c, address_page_operation, data, 1, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_RX;
	}

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_read16(FM24CL16B *device, const uint16_t address, uint16_t *data)
{
	if (s_init != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (address > FM24CL16B_ADDRESS_END) return FM24CL16B_ERROR;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t address_page_operation = s_device_address_read;
	address_page_operation |= (page << 1);
	device->m_data_tx[0] = row;

#ifdef FM24CL16B_INTERRUPTS
	uint32_t timeout = HAL_GetTick() + FM24CL16B_TIMEOUT;
	while (device->m_hi2c->State != HAL_I2C_STATE_READY)
	{
		if (HAL_GetTick() > timeout) return FM24CL16B_ERROR_TIMEOUT;
	}
#endif

	if (HAL_I2C_Master_Transmit(device->m_hi2c, address_page_operation, device->m_data_tx, 1, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	if (HAL_I2C_Master_Receive(device->m_hi2c, address_page_operation, device->m_data_rx, 2, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_RX;
	}

	*data |= (uint16_t)(device->m_data_rx[0] << 8);
	*data |= (uint16_t)(device->m_data_rx[1]);

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_read32(FM24CL16B *device, const uint16_t address, uint32_t *data)
{
	if (s_init != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (address > FM24CL16B_ADDRESS_END) return FM24CL16B_ERROR;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t address_page_operation = s_device_address_read;
	address_page_operation |= (page << 1);
	device->m_data_tx[0] = row;

#ifdef FM24CL16B_INTERRUPTS
	uint32_t timeout = HAL_GetTick() + FM24CL16B_TIMEOUT;
	while (device->m_hi2c->State != HAL_I2C_STATE_READY)
	{
		if (HAL_GetTick() > timeout) return FM24CL16B_ERROR_TIMEOUT;
	}
#endif

	if (HAL_I2C_Master_Transmit(device->m_hi2c, address_page_operation, device->m_data_tx, 1, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	if (HAL_I2C_Master_Receive(device->m_hi2c, address_page_operation, device->m_data_rx, 4, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_RX;
	}

	*data |= (uint32_t)(device->m_data_rx[0] << 24);
	*data |= (uint32_t)(device->m_data_rx[1] << 16);
	*data |= (uint32_t)(device->m_data_rx[2] << 8);
	*data |= (uint32_t)(device->m_data_rx[3]);

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_reset(FM24CL16B *device, const uint8_t value)
{
	if (s_init != FM24CL16B_INIT) return FM24CL16B_ERROR;

	uint32_t mem_value = 0;
	mem_value |= (uint32_t)(value << 24);
	mem_value |= (uint32_t)(value << 16);
	mem_value |= (uint32_t)(value << 8);
	mem_value |= (uint32_t)value;

	for (uint16_t address = 0; address < FM24CL16B_ADDRESS_END + 1; address += 4)
	{
		FM24CL16B_write32(device, address, mem_value);
	}

	return FM24CL16B_OK;
}

FM24CL16B_State FM24CL16B_print(FM24CL16B *device, UART_HandleTypeDef *huart)
{
	if (s_init != FM24CL16B_INIT) return FM24CL16B_ERROR;

	char print_buff[30];

#ifdef FM24CL16B_INTERRUPTS
	uint32_t timeout = HAL_GetTick() + FM24CL16B_TIMEOUT;
	while (device->m_hi2c->State != HAL_I2C_STATE_READY)
	{
		if (HAL_GetTick() > timeout) return FM24CL16B_ERROR_TIMEOUT;
	}
#endif

	if (set_head(device, 0) != FM24CL16B_OK)
	{
		return FM24CL16B_ERROR;
	}

	for (uint16_t address = 0; address < FM24CL16B_ADDRESS_END + 1; address += 8)
	{
		uint8_t page = (address >> 8) & 0x07;
		uint8_t address_page_operation = s_device_address_read;
		address_page_operation |= (page << 1);
		// fast read with address latching
		if (HAL_I2C_Master_Receive(device->m_hi2c, address_page_operation, device->m_data_rx, 8, FM24CL16B_TIMEOUT) != HAL_OK)
		{
			return FM24CL16B_ERROR_RX;
		}

		sprintf(print_buff, "%04X: ", address);
		if (HAL_UART_Transmit(huart, (uint8_t*)print_buff, strlen(print_buff), 100) != HAL_OK)
		{
			return FM24CL16B_ERROR_UART;
		}

		sprintf(print_buff, "%02X %02X %02X %02X %02X %02X %02X %02X\r\n", 	device->m_data_rx[0],
				device->m_data_rx[1],
				device->m_data_rx[2],
				device->m_data_rx[3],
				device->m_data_rx[4],
				device->m_data_rx[5],
				device->m_data_rx[6],
				device->m_data_rx[7]);
		if (HAL_UART_Transmit(huart, (uint8_t*)print_buff, strlen(print_buff), 100) != HAL_OK)
		{
			return FM24CL16B_ERROR_UART;
		}
	}

	return FM24CL16B_OK;
}

#ifdef FM24CL16B_TEST
FM24CL16B_Test FM24CL16B_test8(FM24CL16B *device)
{
	uint8_t data_write = 0x66;
	uint8_t data_read = 0x00;

	for (uint16_t address = 0; address < FM24CL16B_ADDRESS_END + 1; ++address)
	{
		if (FM24CL16B_write8(device, address, data_write) != FM24CL16B_OK)
		{
			return FM24CL16B_TST_FAIL;
		}
	}
	for (uint16_t address = 0; address < FM24CL16B_ADDRESS_END + 1; ++address)
	{
		if (FM24CL16B_read8(device, address, &data_read) != FM24CL16B_OK)
		{
			return FM24CL16B_TST_FAIL;
		}

		if (data_read != data_write)
		{
			return FM24CL16B_TST_FAIL;
		}
		data_read = 0x00;
	}

	FM24CL16B_reset(device, 0x00);

	return FM24CL16B_TST_PASS;
}

FM24CL16B_Test FM24CL16B_test16(FM24CL16B *device)
{
	uint16_t data_write = 0x6666;
	uint16_t data_read = 0x0000;

	for (uint16_t address = 0; address < FM24CL16B_ADDRESS_END + 1; address += 2)
	{
		if (FM24CL16B_write16(device, address, data_write) != FM24CL16B_OK)
		{
			return FM24CL16B_TST_FAIL;
		}
	}
	for (uint16_t address = 0; address < FM24CL16B_ADDRESS_END + 1; address += 2)
	{
		if (FM24CL16B_read16(device, address, &data_read) != FM24CL16B_OK)
		{
			return FM24CL16B_TST_FAIL;
		}

		if (data_read != data_write)
		{
			return FM24CL16B_TST_FAIL;
		}
		data_read = 0x0000;
	}

	FM24CL16B_reset(device, 0x00);

	return FM24CL16B_TST_PASS;
}

FM24CL16B_Test FM24CL16B_test32(FM24CL16B *device)
{
	uint32_t data_write = 0x66666666;
	uint32_t data_read = 0x00000000;

	for (uint16_t address = 0; address < FM24CL16B_ADDRESS_END + 1; address += 4)
	{
		if (FM24CL16B_write32(device, address, data_write) != FM24CL16B_OK)
		{
			return FM24CL16B_TST_FAIL;
		}
	}
	for (uint16_t address = 0; address < FM24CL16B_ADDRESS_END + 1; address += 4)
	{
		if (FM24CL16B_read32(device, address, &data_read) != FM24CL16B_OK)
		{
			return FM24CL16B_TST_FAIL;
		}

		if (data_read != data_write)
		{
			return FM24CL16B_TST_FAIL;
		}
		data_read = 0x00000000;
	}

	FM24CL16B_reset(device, 0x00);

	return FM24CL16B_TST_PASS;
}
#endif

static inline FM24CL16B_State set_head(FM24CL16B *device, const uint16_t address)
{
	if (s_init != FM24CL16B_INIT) return FM24CL16B_ERROR;
	if (address > FM24CL16B_ADDRESS_END) return FM24CL16B_ERROR;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t address_rage_operation = s_device_address_write;
	address_rage_operation |= (page << 1);
	device->m_data_tx[0] = row;
	if (HAL_I2C_Master_Transmit(device->m_hi2c, address_rage_operation, device->m_data_tx, 1, FM24CL16B_TIMEOUT) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	return FM24CL16B_OK;
}

