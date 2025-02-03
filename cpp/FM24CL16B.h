/*
 * FM24CL16B.h
 *
 *  Created on: Jan 23, 2025
 *      Author: knap-linux
 */

#ifndef FM24CL16B_H_
#define FM24CL16B_H_

#include "main.h"

#define FM24CL16B_TEST // disable compilation of tests

class FM24CL16B
{
public:
	FM24CL16B(I2C_HandleTypeDef *hi2c = nullptr);
	virtual ~FM24CL16B();

	enum class State
	{
		Ok,
		Error,
		ErrorTx,
		ErrorRx,
		ErrorUart,
	};

#ifdef FM24CL16B_TEST
	enum class Test
	{
		Pass,
		Fail
	};
#endif

	State write(const uint16_t address, const uint8_t data);
	State write(const uint16_t address, const uint16_t data);
	State write(const uint16_t address, const uint32_t data);

	State read(const uint16_t address, uint8_t *data);
	State read(const uint16_t address, uint16_t *data);
	State read(const uint16_t address, uint32_t *data):

	State reset(const uint32_t data);
	State print(UART_HandleTypeDef *huart);

#ifdef FM24CL16B_TEST
	Test test8();
	Test test16();
	Test test32();
#endif

private:
	inline State setHead(const uint16_t address);

	I2C_HandleTypeDef *m_hi2c;
	uint8_t m_bufferRx[8];
	uint8_t m_bufferTx[8];

};

#endif /* FM24CL16B_H_ */
