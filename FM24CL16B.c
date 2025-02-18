/*
 * FM24CL16.c
 *
 *  Created on: Jan 21, 2025
 *      Author: knap-linux
 */

#include "FM24CL16B.h"

#include <string.h>
#if FM24CL16B_PRINT == 1
#include <stdio.h>
#endif

static const uint8_t s_deviceAddressWrite	= 0xA0;
static const uint8_t s_deviceAddressRead	= 0xA1;

/**
 * @brief Initialize device object.
 *
 * @param self Device object pointer.
 * @param hi2c HAL I2C handle.
 * @param address Device I2C address.
 * @return FM24CL16B_State
 */
FM24CL16B_State FM24CL16B_init(struct FM24CL16B *self, I2C_HandleTypeDef *hi2c)
{
	self->m_hi2c = hi2c;
	self->m_init = 1;
	self->m_timeout = FM24CL16B_DEFAULT_TIMEOUT;

	return FM24CL16B_OK;
}

/**
 * @brief Deinitialize device struct.
 *
 * @param self Device object pointer.
 * @return FM24CL16B_State
 */
FM24CL16B_State FM24CL16B_deinit(struct FM24CL16B *self)
{
	self->m_hi2c = NULL;
	self->m_init = 0;

	return FM24CL16B_OK;
}

/**
 * @brief Device write function. To write at selected address pass a buffer pointer and buffer length.
 *
 * @param self Device object pointer.
 * @param address Selected device address.
 * @param data Data to be written.
 * @param len Length of data to be written.
 * @return FM24CL16B_State
 */
FM24CL16B_State FM24CL16B_write(struct FM24CL16B *self, const uint32_t address, const uint8_t *data, const size_t len)
{
	if (!self->m_init) return FM24CL16B_ERROR_INIT;
	if (address + len - 1 > FM24CL16B_MAX_ADDRESS) return FM24CL16B_ERROR_ADDRESS;

#if FM24CL16B_INTERRUPT == 1
	uint32_t timeout = HAL_GetTick() + self->m_timeout;
	while (self->m_hi2c->State != HAL_I2C_STATE_READY)
	{
		if (HAL_GetTick() > timeout) return FM24CL16B_ERROR_TIMEOUT;
	}
#endif

	uint8_t page = (address >> 8) & 0x07;
	uint8_t addressPageOperation = s_deviceAddressWrite;
	addressPageOperation |= (page << 1);

	self->m_data_tx[0] = (uint8_t)(address & 0xFF);

	memcpy(&self->m_data_tx[1], data, len);

#if FM24CL16B_INTERRUPT == 1
	if (HAL_I2C_Master_Transmit_IT(self->m_hi2c, addressPageOperation, self->m_data_tx, len + 1) != HAL_OK)
#else
	if (HAL_I2C_Master_Transmit(self->m_hi2c, addressPageOperation, self->m_data_tx, len + 1, self->m_timeout) != HAL_OK)
#endif
	{
		return FM24CL16B_ERROR_TX;
	}

	return FM24CL16B_OK;
}

/**
 * @brief Device read function. To read from selected address pass a buffer pointer and buffer length.
 *
 * @param self Device object pointer.
 * @param address Selected device address.
 * @param data Outout buffer.
 * @param len Length of data to be read.
 * @return FM24CL16B_State
 */
FM24CL16B_State FM24CL16B_read(struct FM24CL16B *self, const uint32_t address, uint8_t *data, const size_t len)
{
	if (!self->m_init) return FM24CL16B_ERROR_INIT;
	if (address + len - 1 > FM24CL16B_MAX_ADDRESS) return FM24CL16B_ERROR_ADDRESS;

#if FM24CL16B_INTERRUPT == 1
	uint32_t timeout = HAL_GetTick() + self->m_timeout;
	while (self->m_hi2c->State != HAL_I2C_STATE_READY)
	{
		if (HAL_GetTick() > timeout) return FM24CL16B_ERROR_TIMEOUT;
	}
#endif

	uint8_t page = (address >> 8) & 0x07;
	uint8_t addressPageOperation = s_deviceAddressRead;
	addressPageOperation |= (page << 1);

	self->m_data_tx[0] = (uint8_t)(address & 0xFF);

	if (HAL_I2C_Master_Transmit(self->m_hi2c, addressPageOperation, self->m_data_tx, 1, self->m_timeout) != HAL_OK)
	{
		return FM24CL16B_ERROR_TX;
	}

	if (HAL_I2C_Master_Receive(self->m_hi2c, addressPageOperation, self->m_data_rx, len, self->m_timeout) != HAL_OK)
	{
		return FM24CL16B_ERROR_RX;
	}

	memcpy(data, self->m_data_rx, len);

	return FM24CL16B_OK;
}

/**
 * @brief Reset device state to a selected value.
 *
 * @param self Device object pointer.
 * @param value Value to be set.
 * @return FM24CL16B_State
 */
FM24CL16B_State FM24CL16B_reset(struct FM24CL16B *self, const uint8_t value)
{
    if (!self->m_init) return FM24CL16B_ERROR_INIT;

    uint32_t writeLen = FM24CL16B_WRITE_LEN;
    uint32_t pageSize = FM24CL16B_WRITE_LEN;

    memset(&self->m_data_tx[1], value, writeLen);

    for (uint32_t address = 0; address < FM24CL16B_MAX_ADDRESS; address += writeLen)
    {
        uint32_t remainingInPage = pageSize - (address % pageSize);
        uint32_t chunkSize = (writeLen > remainingInPage) ? remainingInPage : writeLen;

        uint8_t page = (address >> 8) & 0x07;
		uint8_t addressPageOperation = s_deviceAddressWrite;
		addressPageOperation |= (page << 1);

		self->m_data_tx[0] = (uint8_t)(address & 0xFF);

        if (HAL_I2C_Master_Transmit(self->m_hi2c, addressPageOperation, self->m_data_tx, chunkSize + 1, self->m_timeout) != HAL_OK)
        {
            return FM24CL16B_ERROR_TX;
        }
    }

    return FM24CL16B_OK;
}


#if FM24CL16B_PRINT == 1
/**
 * @brief Print device state.
 *
 * @param self Device object pointer.
 * @param huart HAL UART handle pointer.
 * @return FM24CL16B_State
 */
FM24CL16B_State FM24CL16B_print(struct FM24CL16B *self, UART_HandleTypeDef *huart)
{
	if (!self->m_init) return FM24CL16B_ERROR_INIT;

	uint32_t readLen = 8;
	uint8_t printBuff[16];

	for (uint32_t address = 0; address < FM24CL16B_MAX_ADDRESS; address += readLen)
	{
		uint8_t page = (address >> 8) & 0x07;
		uint8_t addressPageOperation = s_deviceAddressWrite;
		addressPageOperation |= (page << 1);

		self->m_data_tx[0] = (uint8_t)(address & 0xFF);

		if (HAL_I2C_Master_Transmit(self->m_hi2c, addressPageOperation, self->m_data_tx, 1, self->m_timeout) != HAL_OK)
		{
			return FM24CL16B_ERROR_TX;
		}

		if (HAL_I2C_Master_Receive(self->m_hi2c, addressPageOperation, self->m_data_rx, readLen, self->m_timeout) != HAL_OK)
		{
			return FM24CL16B_ERROR_RX;
		}

		sprintf((char*)printBuff, "%04X:", (uint16_t)address);
		if (HAL_UART_Transmit(huart, printBuff, strlen((char*)printBuff), FM24CL16B_TIMEOUT_UART) != HAL_OK)
		{
			return FM24CL16B_ERROR_UART;
		}

		for (uint32_t byteCtr = 0; byteCtr < readLen; ++byteCtr)
		{
			sprintf((char*)printBuff, " %02X", self->m_data_rx[byteCtr]);
			if (HAL_UART_Transmit(huart, printBuff, strlen((char*)printBuff), FM24CL16B_TIMEOUT_UART) != HAL_OK)
			{
				return FM24CL16B_ERROR_UART;
			}
		}

		sprintf((char*)printBuff, "\r\n");
		if (HAL_UART_Transmit(huart, printBuff, strlen((char*)printBuff), FM24CL16B_TIMEOUT_UART) != HAL_OK)
		{
			return FM24CL16B_ERROR_UART;
		}
	}

	return FM24CL16B_OK;
}
#endif
