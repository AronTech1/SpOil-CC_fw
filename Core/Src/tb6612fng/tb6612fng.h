/*
 * tb6612fng.h
 *
 *  Created on: Oct 11, 2025
 *      Author: moshte
 */

#ifndef TB6612FNG_H
#define TB6612FNG_H

#include "main.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "string.h"
#include "type.h"
#include "log.h"
#include <stdint.h>
#include <stddef.h>

#define VOLTSPERRPM 0.06
#define MAXVOLTAGE 12

extern TIM_HandleTypeDef htim3;

enum gpioLevel{
	HIGH = GPIO_PIN_SET,
	LOW = GPIO_PIN_RESET
};

void tbPmwStart(void);

static void tbSetStndbyPin(enum gpioLevel opt);
static void tbSetAin1Pin(enum gpioLevel opt);
static void tbSetAin2Pin(enum gpioLevel opt);

static void tbSetDutyCycle(float duty);
static void tbResetDutyCycle(void);

void tbMotorCw(byte rpm);
void tbMotorCcw(byte rpm);
void tbMotorShortBrake(void);
void tbMotorStop(void);
void tbMotorStndby(void);

void tbMotorDrive(byte rpm, byte dir);
#endif /*TB6612FNG_H */
