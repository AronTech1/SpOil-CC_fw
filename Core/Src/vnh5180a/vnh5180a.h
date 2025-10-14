/*
 * vnh5180a.h
 *
 *  Created on: Oct 14, 2025
 *      Author: moshte
 */

#ifndef VNH5180A_H
#define VNH5180A_H

#include "main.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "string.h"
#include "type.h"
#include "log.h"
#include <stdint.h>
#include <stddef.h>

#define VOLTSPERRPM 0.06 // Might need adjustment according to motor
#define MAXVOLTAGE 12

extern TIM_HandleTypeDef htim1;

enum gpioLevel{
	HIGH = GPIO_PIN_SET,
	LOW = GPIO_PIN_RESET
};

void vnSetInAPin(enum gpioLevel opt);
void vnSetInBPin(enum gpioLevel opt);

GPIO_PinState vnReadDiagAPin(void);
GPIO_PinState vnReadDiagBPin(void);

byte vnCheckMotorStatus(void);

void vnPmwStart(void);

static void vnSetDutyCycle(float duty);
static void vnResetDutyCycle(void);

void vnMotorCw(qbyte rpm);
void vnMotorCcw(qbyte rpm);

void tbMotorBrakeToGnd(void);
void tbMotorBrakeToVcc(void);

void vnMotorDrive(byte rpm, byte dir);

void csEnOrDis(byte enOrDis);

#endif /* VNH5180A_H */
