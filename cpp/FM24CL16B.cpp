/*
 * FM24CL16B.cpp
 *
 *  Created on: Jan 23, 2025
 *      Author: knap-linux
 */

#include "FM24CL16B.h"

#include <stdio.h>
#include <string.h>

constexpr uint8_t DEVICE_ADDRESS_WRITE 	= 0xA0;
constexpr uint8_t DEVICE_ADDRESS_READ	= 0xA1;
constexpr uint32_t I2C_TIMEOUT			= 100; // ms
constexpr uint16_t DEVICE_ADDRESS_END	= 2047;

FM24CL16B::FM24CL16B(I2C_HandleTypeDef *hi2c)
	: m_hi2c{hi2c}
	, m_bufferRx{}
	, m_bufferTx{}
{
}

FM24CL16B::~FM24CL16B()
{

}

FM24CL16B::State FM24CL16B::write(const uint16_t address, const uint8_t data)
{
	if (m_hi2c == nullptr) return State::Error;
	if (address > DEVICE_ADDRESS_END) return State::Error;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;
	uint8_t addressPageOperation = DEVICE_ADDRESS_WRITE;

	addressPageOperation |= (page << 1);

#ifdef FM24CL16B_INTERRUPTS
	uint32_t timeout = HAL_GetTick() + I2C_TIMEOUT;
	while (this->m_hi2c->State != HAL_I2C_STATE_READY)
	{
		if (HAL_GetTick() > timeout) return State::ErrorTimeout;
	}
#endif

	m_bufferTx[0] = row;
	m_bufferTx[1] = data;

#ifdef FM24CL16B_INTERRUPTS
	if (HAL_I2C_Master_Transmit_IT(m_hi2c, addressPageOperation, m_bufferTx, 2) != HAL_OK)
#else
	if (HAL_I2C_Master_Transmit(m_hi2c, addressPageOperation, m_bufferTx, 2, I2C_TIMEOUT) != HAL_OK)
#endif
	{
		return State::ErrorTx;
	}

	return State::Ok;
}

FM24CL16B::State FM24CL16B::write(const uint16_t address, const uint16_t data)
{
	if (m_hi2c == nullptr) return State::Error;
	if (address > DEVICE_ADDRESS_END) return State::Error;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t addressPageOperation = DEVICE_ADDRESS_WRITE;
	addressPageOperation |= (page << 1);
	uint8_t data0 = (uint8_t)(data >> 8);
	uint8_t data1 = (uint8_t)(data & 0xFF);

#ifdef FM24CL16B_INTERRUPTS
	uint32_t timeout = HAL_GetTick() + I2C_TIMEOUT;
	while (this->m_hi2c->State != HAL_I2C_STATE_READY)
	{
		if (HAL_GetTick() > timeout) return State::ErrorTimeout;
	}
#endif

	m_bufferTx[0] = row;
	m_bufferTx[1] = data0;
	m_bufferTx[2] = data1;

#ifdef FM24CL16B_INTERRUPTS
	if (HAL_I2C_Master_Transmit_IT(m_hi2c, addressPageOperation, m_bufferTx, 3) != HAL_OK)
#else
	if (HAL_I2C_Master_Transmit(m_hi2c, addressPageOperation, m_bufferTx, 3, I2C_TIMEOUT) != HAL_OK)
#endif
	{
		return State::ErrorTx;
	}

	return State::Ok;
}

FM24CL16B::State FM24CL16B::write(const uint16_t address, const uint32_t data)
{
	if (m_hi2c == nullptr) return State::Error;
	if (address > DEVICE_ADDRESS_END) return State::Error;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t addressPageOperation = DEVICE_ADDRESS_WRITE;
	addressPageOperation |= (page << 1);
	uint8_t data0 = (uint8_t)(data >> 24);
	uint8_t data1 = (uint8_t)(data >> 16);
	uint8_t data2 = (uint8_t)(data >> 8);
	uint8_t data3 = (uint8_t)(data & 0xFF);

#ifdef FM24CL16B_INTERRUPTS
	uint32_t timeout = HAL_GetTick() + I2C_TIMEOUT;
	while (this->m_hi2c->State != HAL_I2C_STATE_READY)
	{
		if (HAL_GetTick() > timeout) return State::ErrorTimeout;
	}
#endif

	m_bufferTx[0] = row;
	m_bufferTx[1] = data0;
	m_bufferTx[2] = data1;
	m_bufferTx[3] = data2;
	m_bufferTx[4] = data3;

#ifdef FM24CL16B_INTERRUPTS
	if (HAL_I2C_Master_Transmit_IT(m_hi2c, addressPageOperation, m_bufferTx, 5) != HAL_OK)
#else
	if (HAL_I2C_Master_Transmit(m_hi2c, addressPageOperation, m_bufferTx, 5, I2C_TIMEOUT) != HAL_OK)
#endif
	{
		return State::ErrorTx;
	}

	return State::Ok;
}

FM24CL16B::State FM24CL16B::read(const uint16_t address, uint8_t *data)
{
	if (m_hi2c == nullptr) return State::Error;
	if (address > DEVICE_ADDRESS_END) return State::Error;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t addressPageOperation = DEVICE_ADDRESS_READ;
	addressPageOperation |= (page << 1);
	m_bufferTx[0] = row;

#ifdef FM24CL16B_INTERRUPTS
	uint32_t timeout = HAL_GetTick() + I2C_TIMEOUT;
	while (this->m_hi2c->State != HAL_I2C_STATE_READY)
	{
		if (HAL_GetTick() > timeout) return State::ErrorTimeout;
	}
#endif

	if (HAL_I2C_Master_Transmit(m_hi2c, addressPageOperation, m_bufferTx, 1, I2C_TIMEOUT) != HAL_OK)
	{
		return State::ErrorTx;
	}

	if (HAL_I2C_Master_Receive(m_hi2c, addressPageOperation, m_bufferRx, 1, I2C_TIMEOUT) != HAL_OK)
	{
		return State::ErrorRx;
	}

	*data = this->m_bufferRx[0];

	return State::Ok;
}

FM24CL16B::State FM24CL16B::read(const uint16_t address, uint16_t *data)
{
	if (m_hi2c == nullptr) return State::Error;
	if (address > DEVICE_ADDRESS_END) return State::Error;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t addressPageOperation = DEVICE_ADDRESS_READ;
	addressPageOperation |= (page << 1);
	m_bufferTx[0] = row;

#ifdef FM24CL16B_INTERRUPTS
	uint32_t timeout = HAL_GetTick() + I2C_TIMEOUT;
	while (this->m_hi2c->State != HAL_I2C_STATE_READY)
	{
		if (HAL_GetTick() > timeout) return State::ErrorTimeout;
	}
#endif

	if (HAL_I2C_Master_Transmit(m_hi2c, addressPageOperation, m_bufferTx, 1, I2C_TIMEOUT) != HAL_OK)
	{
		return State::ErrorTx;
	}

	if (HAL_I2C_Master_Receive(m_hi2c, addressPageOperation, m_bufferRx, 2, I2C_TIMEOUT) != HAL_OK)
	{
		return State::ErrorRx;
	}

	uint16_t result = 0;
	result |= (uint16_t)(this->m_bufferRx[0] << 8);
	result |= (uint16_t)(this->m_bufferRx[1]);
	*data = result;

	return State::Ok;
}

FM24CL16B::State FM24CL16B::read(const uint16_t address, uint32_t *data)
{
	if (m_hi2c == nullptr) return State::Error;
	if (address > DEVICE_ADDRESS_END) return State::Error;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t addressPageOperation = DEVICE_ADDRESS_READ;
	addressPageOperation |= (page << 1);
	m_bufferTx[0] = row;

#ifdef FM24CL16B_INTERRUPTS
	uint32_t timeout = HAL_GetTick() + I2C_TIMEOUT;
	while (this->m_hi2c->State != HAL_I2C_STATE_READY)
	{
		if (HAL_GetTick() > timeout) return State::ErrorTimeout;
	}
#endif

	if (HAL_I2C_Master_Transmit(m_hi2c, addressPageOperation, m_bufferTx, 1, I2C_TIMEOUT) != HAL_OK)
	{
		return State::ErrorTx;
	}

	if (HAL_I2C_Master_Receive(m_hi2c, addressPageOperation, m_bufferRx, 4, I2C_TIMEOUT) != HAL_OK)
	{
		return State::ErrorRx;
	}

	uint32_t result = 0;
	result |= (uint32_t)(this->m_bufferRx[0] << 24);
	result |= (uint32_t)(this->m_bufferRx[1] << 16);
	result |= (uint32_t)(this->m_bufferRx[2] << 8);
	result |= (uint32_t)(this->m_bufferRx[3]);
	*data = result;

	return State::Ok;
}

#ifdef FM24CL16B_TEST
FM24CL16B::Test FM24CL16B::test8()
{
	if (m_hi2c == nullptr) return Test::Fail;

	uint8_t dataWrite = 0x66;
	uint8_t dataRead = 0x00;

	for (uint16_t address = 0; address < DEVICE_ADDRESS_END + 1; ++address)
	{
		if (this->write(address, dataWrite) != State::Ok)
		{
			return Test::Fail;
		}
	}
	for (uint16_t address = 0; address < DEVICE_ADDRESS_END + 1; ++address)
	{
		if (this->read(address, &dataRead) != State::Ok)
		{
			return Test::Fail;
		}

		if (dataRead != dataWrite)
		{
			return Test::Fail;
		}
		dataRead = 0;
	}

	this->reset(0x00);

	return Test::Pass;
}

FM24CL16B::Test FM24CL16B::test16()
{
	if (m_hi2c == nullptr) return Test::Fail;

	uint16_t dataWrite = 0x6666;
	uint16_t dataRead = 0x0000;

	for (uint16_t address = 0; address < DEVICE_ADDRESS_END + 1; ++address)
	{
		if (this->write(address, dataWrite) != State::Ok)
		{
			return Test::Fail;
		}
	}
	for (uint16_t address = 0; address < DEVICE_ADDRESS_END + 1; ++address)
	{
		if (this->read(address, &dataRead) != State::Ok)
		{
			return Test::Fail;
		}

		if (dataRead != dataWrite)
		{
			return Test::Fail;
		}
		dataRead = 0;
	}

	this->reset(0x00);

	return Test::Pass;
}

FM24CL16B::Test FM24CL16B::test32()
{
	if (m_hi2c == nullptr) return Test::Fail;

	uint32_t dataWrite = 0x66666666;
	uint32_t dataRead = 0x00000000;

	for (uint16_t address = 0; address < DEVICE_ADDRESS_END + 1; ++address)
	{
		if (this->write(address, dataWrite) != State::Ok)
		{
			return Test::Fail;
		}
	}
	for (uint16_t address = 0; address < DEVICE_ADDRESS_END + 1; ++address)
	{
		if (this->read(address, &dataRead) != State::Ok)
		{
			return Test::Fail;
		}

		if (dataRead != dataWrite)
		{
			return Test::Fail;
		}
		dataRead = 0;
	}

	this->reset(0x00);

	return Test::Pass;
}
#endif

FM24CL16B::State FM24CL16B::reset(const uint32_t data)
{
	if (m_hi2c == nullptr) return State::Error;

	uint32_t memValue = 0;
	memValue |= (uint32_t)(data << 24);
	memValue |= (uint32_t)(data << 16);
	memValue |= (uint32_t)(data << 8);
	memValue |= (uint32_t)data;

	for (uint16_t address = 0; address < DEVICE_ADDRESS_END + 1; address += 4)
	{
		 if (this->write(address, memValue) != State::Ok)
		 {
			 return State::Error;
		 }
	}

	return State::Ok;
}

FM24CL16B::State FM24CL16B::print(UART_HandleTypeDef *huart)
{
	if (m_hi2c == nullptr) return State::Error;

	char printBuff[30];

	if (this->setHead(0) != State::Ok)
	{
		return State::Error;
	}

	for (uint16_t address = 0; address < DEVICE_ADDRESS_END + 1; address += 8)
	{
		uint8_t page = (address >> 8) & 0x07;
		uint8_t addressPageOperation = DEVICE_ADDRESS_READ;
		addressPageOperation |= (page << 1);
		// fast read with address latching
		if (HAL_I2C_Master_Receive(m_hi2c, addressPageOperation, m_bufferRx, 8, I2C_TIMEOUT) != HAL_OK)
		{
			return State::ErrorRx;
		}

		sprintf(printBuff, "%04X: ", address);
		if (HAL_UART_Transmit(huart, (uint8_t*)printBuff, strlen(printBuff), 100) != HAL_OK)
		{
			return State::ErrorUart;
		}

		sprintf(printBuff, "%02X %02X %02X %02X %02X %02X %02X %02X\r\n", 	m_bufferRx[0],
				m_bufferRx[1],
				m_bufferRx[2],
				m_bufferRx[3],
				m_bufferRx[4],
				m_bufferRx[5],
				m_bufferRx[6],
				m_bufferRx[7]);
		if (HAL_UART_Transmit(huart, (uint8_t*)printBuff, strlen(printBuff), 100) != HAL_OK)
		{
			return State::ErrorUart;
		}
	}

	return State::Ok;
}

inline FM24CL16B::State FM24CL16B::setHead(const uint16_t address)
{
	if (m_hi2c == nullptr) return State::Error;
	if (address > DEVICE_ADDRESS_END) return State::Error;

	uint8_t page = (address >> 8) & 0x07;
	uint8_t row = address & 0xFF;

	uint8_t addressPageOperation = DEVICE_ADDRESS_WRITE;
	addressPageOperation |= (page << 1);
	m_bufferTx[0] = row;

#ifdef FM24CL16B_INTERRUPTS
	uint32_t timeout = HAL_GetTick() + I2C_TIMEOUT;
	while (this->m_hi2c->State != HAL_I2C_STATE_READY)
	{
		if (HAL_GetTick() > timeout) return State::ErrorTimeout;
	}
#endif

	if (HAL_I2C_Master_Transmit(m_hi2c, addressPageOperation, m_bufferTx, 1, I2C_TIMEOUT) != HAL_OK)
	{
		return State::ErrorTx;
	}

	return State::Ok;
}
