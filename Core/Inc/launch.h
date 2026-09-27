/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : launch.h
  * @brief          : Header for launch_check.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
// #ifndef __MAIN_H
// #define __MAIN_H

// #ifdef __cplusplus
// extern "C" {
// #endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

#define ACCEL_THRESHOLD 3.0f // [X] g
#define ACCEL_SUSTAIN_TIME 100u // [X] ms
#define LAUNCH_ALT_RISE_THRESHOLD_M  10.0f   // [X] m   
#define LAUNCH_CONFIRM_TIMEOUT_MS    500U    // [X] ms 
#define PAD_SAMPLE_RATE_HZ           50U     // [X] Hz  
#define PAD_ALT_AVG_WINDOW_MS        2000U   // [X] ms 


/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

