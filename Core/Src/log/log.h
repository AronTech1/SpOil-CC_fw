/*
 * log.h
 *
 *  Created on: Oct 4, 2025
 *  Author: moshte
 */

#ifndef LOG_H
#define LOG_H

#include "main.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "string.h"
#include "type.h"

#define LOG_TO HAL_MAX_DELAY //Log timeout
#define LOG_EN // Enable or disable logging

extern UART_HandleTypeDef huart2;

HAL_StatusTypeDef logString(char* msg);
HAL_StatusTypeDef logInteger(byte num);
HAL_StatusTypeDef logFloat(float num);

#endif /* LOG_H */
