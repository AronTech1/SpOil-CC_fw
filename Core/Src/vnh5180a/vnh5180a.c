/*
 * vnh5180a.c
 *
 *  Created on: Oct 14, 2025
 *      Author: moshte
 */
#include "vnh5180a.h"

void vnSetInAPin(enum gpioLevel opt){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, opt);
}

void vnSetInBPin(enum gpioLevel opt){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, opt);
}

GPIO_PinState vnReadDiagAPin(void){
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)){
		return HIGH;
	}
	else{
		return LOW;
	}
}

GPIO_PinState vnReadDiagBPin(void){
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)){
		return HIGH;
	}
	else{
		return LOW;
	}
}

void vnPmwStart(void){
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
}

static void vnSetDutyCycle(float duty){
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (qbyte)duty);
}

static void vnResetDutyCycle(void){
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
}

byte vnCheckMotorStatus(void){
	GPIO_PinState pinAStatus = 0;
	GPIO_PinState pinBStatus = 0;

	pinAStatus = vnReadDiagAPin();
	pinBStatus = vnReadDiagBPin();

	if(pinAStatus && pinBStatus){
        logString("VNH5180A: Normal Condition");
        return 1;
	}
	else{
		logString("VNH5180A: Fault Condition");
		return 0;
	}
}

void vnMotorCw(qbyte rpm){
	byte sts = 0;
	sts = vnCheckMotorStatus();

	if(rpm > 22000){
	   rpm = 22000;
    }

	float voltsMtr =  rpm * VOLTSPERRPM;
	float duty = ((voltsMtr/MAXVOLTAGE) * 100);

	if(sts){
		vnSetInAPin(HIGH);
		vnSetInBPin(LOW);

		vnSetDutyCycle(duty);

		logString("VNH5180A: Clockwise mode/r/n");
	}
	else{
		logString("VNH5180A: High Imp state/r/n");
		return;
	}
	return;
}

void vnMotorCcw(qbyte rpm){
	byte sts = 0;
	sts = vnCheckMotorStatus();

	if(rpm > 22000){
	   rpm = 22000;
    }

	float voltsMtr =  rpm * VOLTSPERRPM;
	float duty = ((voltsMtr/MAXVOLTAGE) * 100);

	if(sts){
		vnSetInAPin(LOW);
		vnSetInBPin(HIGH);

		vnSetDutyCycle(duty);

		logString("VNH5180A: Counter clockwise mode/r/n");
	}
	else{
		logString("VNH5180A: High Imp state or IoutK/r/n");
		return;
	}
	return;
}

void tbMotorBrakeToGnd(void){
	byte sts = 0;
	sts = vnCheckMotorStatus();

	if(sts){
		vnSetInAPin(LOW);
		vnSetInBPin(LOW);

		vnResetDutyCycle();

		logString("VNH5180A: Brake to ground/r/n");
	}
	else{
		logString("VNH5180A: High Imp state/r/n");
		return;
	}
	return;
}

void tbMotorBrakeToVcc(void){
	byte sts = 0;
	sts = vnCheckMotorStatus();

	if(sts){
		vnSetInAPin(HIGH);
		vnSetInBPin(HIGH);

		vnResetDutyCycle();

		logString("VNH5180A: Brake to VCC/r/n");
	}
	else{
		logString("VNH5180A: High Imp stater/r/n");
		return;
	}
	return;
}


void vnMotorDrive(byte rpm, byte dir){

	if(dir != 0 || dir != 1){
		logString("Incorrect direction parameter!/r/n");
		return;
	}

	if(dir == 0){
		vnMotorCw(rpm);
	}
	else if(dir == 1){
		vnMotorCcw(rpm);
	}
}

void csEnOrDis(byte enOrDis){
	if(enOrDis != 0 || enOrDis != 1){
		logString("VNH5180A: Incorrect CS enable or disable parameter!/r/n");
		return;
	}
	if(enOrDis){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, LOW);
	}
	else{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, HIGH);
	}
}
