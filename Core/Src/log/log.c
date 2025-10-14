/*
 * log.c
 *
 *  Created on: Oct 4, 2025
 *  Author: moshte
 */

#include "log.h"

HAL_StatusTypeDef logString(char* msg){
	if(HAL_UART_Transmit(&huart2, (byte*)msg, strlen(msg), LOG_TO) == HAL_OK ){
      return HAL_OK;
	}
	else{
      return HAL_ERROR;
	}
}

HAL_StatusTypeDef logInteger(byte num){
	if(HAL_UART_Transmit(&huart2, &num, sizeof(num), LOG_TO) == HAL_OK ){
      return HAL_OK;
	}
	else{
      return HAL_ERROR;
	}
}

HAL_StatusTypeDef logFloat(float num){
	if(HAL_UART_Transmit(&huart2, (byte*)&num, sizeof(num), LOG_TO) == HAL_OK ){
	  return HAL_OK;
	}
	else{
	  return HAL_ERROR;
	}
}
