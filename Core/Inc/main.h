/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
//void mouse_keyboard_test(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define CS_Pin GPIO_PIN_0
#define CS_GPIO_Port GPIOB
#define CHRG_ON_Pin GPIO_PIN_2
#define CHRG_ON_GPIO_Port GPIOB
#define SWITCH_FEEDBACK_Pin GPIO_PIN_12
#define SWITCH_FEEDBACK_GPIO_Port GPIOB
#define BTN_Pin GPIO_PIN_13
#define BTN_GPIO_Port GPIOB
#define SWITCH_CONTROL_Pin GPIO_PIN_8
#define SWITCH_CONTROL_GPIO_Port GPIOA
#define OTG_HUB_Pin GPIO_PIN_3
#define OTG_HUB_GPIO_Port GPIOB
#define OTG_Pin GPIO_PIN_4
#define OTG_GPIO_Port GPIOB
#define SEL_HUB_Pin GPIO_PIN_5
#define SEL_HUB_GPIO_Port GPIOB
#define SEL_PC_Pin GPIO_PIN_6
#define SEL_PC_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
