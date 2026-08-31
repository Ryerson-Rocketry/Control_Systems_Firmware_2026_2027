/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "iwdg.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ALL_TASKS_COMPLETED_FLAGS 0x00000000U  /* Placeholder for tasks to be added later */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osThreadId_t idleTaskHandle;
const osThreadAttr_t idleTaskAttributes = {
  .name = "idleTask", .stack_size = 128 * 4, .priority = osPriorityIdle};

osThreadId_t watchdogTaskHandle;
const osThreadAttr_t watchdogTaskAttributes = {
  .name = "watchdogTask", .stack_size = 128 * 4, .priority = osPriorityLow};

osEventFlagsId_t tasksCompletedEventFlagsHandle;
const osEventFlagsAttr_t tasksCompletedEventFlagsAttributes = {.name = "tasksCompleted"};
/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */
void idleTaskFunction(void *argument);
void watchdogTaskFunction(void *argument);
/* USER CODE END FunctionPrototypes */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
  
  /* USER CODE BEGIN RTOS_THREADS */
  idleTaskHandle = osThreadNew(idleTaskFunction, NULL, &idleTaskAttributes);
  watchdogTaskHandle = osThreadNew(watchdogTaskFunction, NULL, &watchdogTaskAttributes);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  tasksCompletedEventFlagsHandle = osEventFlagsNew(&tasksCompletedEventFlagsAttributes);
  /* USER CODE END RTOS_EVENTS */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/**
  * @brief  Function implementing the watchdogTask thread.
  * @param  argument: Not used
  * @retval None
  */
void watchdogTaskFunction(void *argument)
{
  osEventFlagsWait(tasksCompletedEventFlagsHandle, ALL_TASKS_COMPLETED_FLAGS, (osFlagsWaitAll | osFlagsNoClear), 0);
  HAL_IWDG_Refresh(&hiwdg);
  osEventFlagsClear(tasksCompletedEventFlagsHandle, ALL_TASKS_COMPLETED_FLAGS);
}

/**
  * @brief  Function implementing the idleTask thread.
  * @param  argument: Not used
  * @retval None
  */
void idleTaskFunction(void *argument)
{
  for(;;)
  {
    osDelay(1);
  }
}
/* USER CODE END Application */