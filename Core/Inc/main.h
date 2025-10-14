/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CS_Pin GPIO_PIN_1
#define CS_GPIO_Port GPIOA
#define LOG_TX_Pin GPIO_PIN_2
#define LOG_TX_GPIO_Port GPIOA
#define LOG_RX_Pin GPIO_PIN_3
#define LOG_RX_GPIO_Port GPIOA
#define AIN1_Pin GPIO_PIN_5
#define AIN1_GPIO_Port GPIOA
#define TIM3_CH2_TB_Pin GPIO_PIN_7
#define TIM3_CH2_TB_GPIO_Port GPIOA
#define STBY_Pin GPIO_PIN_0
#define STBY_GPIO_Port GPIOB
#define TIM1_CH3N_VN_Pin GPIO_PIN_1
#define TIM1_CH3N_VN_GPIO_Port GPIOB
#define SCL_BQ_Pin GPIO_PIN_10
#define SCL_BQ_GPIO_Port GPIOB
#define SDA_BQ_Pin GPIO_PIN_11
#define SDA_BQ_GPIO_Port GPIOB
#define BQ_INT_Pin GPIO_PIN_12
#define BQ_INT_GPIO_Port GPIOB
#define CS_DIS_Pin GPIO_PIN_14
#define CS_DIS_GPIO_Port GPIOB
#define ENDB_ST_Pin GPIO_PIN_15
#define ENDB_ST_GPIO_Port GPIOB
#define INB_Pin GPIO_PIN_8
#define INB_GPIO_Port GPIOA
#define ENDA_ST_Pin GPIO_PIN_9
#define ENDA_ST_GPIO_Port GPIOA
#define INA_Pin GPIO_PIN_10
#define INA_GPIO_Port GPIOA
#define IO_Pin GPIO_PIN_13
#define IO_GPIO_Port GPIOA
#define CLK_Pin GPIO_PIN_14
#define CLK_GPIO_Port GPIOA
#define MTR_EXTI_Pin GPIO_PIN_3
#define MTR_EXTI_GPIO_Port GPIOB
#define INT_PD_Pin GPIO_PIN_5
#define INT_PD_GPIO_Port GPIOB
#define SCL_PD_Pin GPIO_PIN_6
#define SCL_PD_GPIO_Port GPIOB
#define SDA_PD_Pin GPIO_PIN_7
#define SDA_PD_GPIO_Port GPIOB
#define LED_ST_Pin GPIO_PIN_8
#define LED_ST_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
