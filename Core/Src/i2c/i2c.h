/*
 * i2c.h
 *
 *  Created on: Oct 4, 2025
 *  Author: moshte
 */

#ifndef I2C_H
#define I2C_H

#include "main.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "string.h"
#include "type.h"
#include "log.h"

#include <stdint.h>
#include <stddef.h>


#define READ_BUFF_LEN 128
#define WRITE_BUFF_LEN 6

#define I2C_TO HAL_MAX_DELAY

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

extern byte readBuf[READ_BUFF_LEN];
extern byte writeBuf[WRITE_BUFF_LEN];

/***************************************************GENERAL FUNCTIONS******************************************************/
/**
 * @brief: Concatenates the command and data to be written to the slave into a payload array
 * @param: cmdOrRegAdrss - Operation specific command or register address
 * @param: data - Array to be written
 * @param: dlen - Length of the data array
 *
 * @return: An array containing the command/register address and data concatenated
 */
extern byte* i2cConcatenate(byte cmdOrRegAdrss, byte* data, byte dlen);

/**
 * @brief: Clear write buffer
 */
extern void clearWriteBuf(void);

/**
 * @brief: Clear read buffer
 */
extern void clearReadBuf(void);

/**********************************AP33772S WRITE AND READ I2C FUNCTIONS*********************************************************/
/**
 * @brief: Writes t0 AP33772S registers via I2c
 * @param: slvAdrss  - Address of the device to be written to
 * @param: cmd - Operation specific command
 * @param: data - Array to be written
 * @param: dlen - Length of the data array
 *
 * @return: HAL status of the transaction. HAL_OK or HAL_ERORR
 */
extern HAL_StatusTypeDef  i2cApWrite(byte slvAdrss, byte cmd, byte* data, byte dlen);

/**
 * @brief: Reads from AP33772S registers via I2c
 * @param: slvAdrss  - Address of the device to be read from
 * @param: cmd - Operation specific command
 * @param: len - Expected length of the read data array
 *
 * @return: HAL status of the transaction. HAL_OK or HAL_ERORR
 */
extern HAL_StatusTypeDef  i2cApRead(byte slvAdrss, byte cmd, dbyte len);

/**********************************BQ25790 WRITE AND READ I2C FUNCTIONS**********************************************************/
/**
 * @brief Write a value to a register via I2C
 *
 * @param: slvAdrss - device address
 * @param: regAdrss -  register address to write to
 * @param: data -  data to be written in @ref reg
 * @param: dlen - length of @ref data
 *
 * @return: HAL status of the transaction. HAL_OK or HAL_ERORR
 */
extern HAL_StatusTypeDef  i2cBqWrite(byte slvAdrss, byte regAdrss, byte* data, byte dlen);

/**
 * @brief Read a register value via I2C
 *
 * @param: slvAdrss - device address
 * @param: regAdrss- register address to read from
 * @param: p - pointer to where the read data is stored. readBuf can be passed here
 * @param: len - Expected length of the read data array
 *
 * @return: HAL status of the transaction. HAL_OK or HAL_ERORR
 */
extern HAL_StatusTypeDef  i2cBqRead(byte slvAdrss, byte regAdrss, byte *p, dbyte len);
#endif /* I2C_H */
