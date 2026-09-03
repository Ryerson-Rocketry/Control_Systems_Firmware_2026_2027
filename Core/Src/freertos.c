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
#define ALL_TASKS_COMPLETE_FLAGS 0x00000000U  /* Placeholder for tasks to be added later */
#define ALL_TASKS_CONTINUE_FLAGS ALL_TASKS_COMPLETE_FLAGS
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
  .name = "watchdogTask", .stack_size = 128 * 4, .priority = osPriorityHigh};
  
osEventFlagsId_t tasksCompleteEventFlagsHandle;
const osEventFlagsAttr_t tasksCompleteEventFlagsAttributes = {.name = "tasksComplete"};
  
osEventFlagsId_t tasksContinueEventFlagsHandle;
const osEventFlagsAttr_t tasksContinueEventFlagsAttributes = {.name = "tasksContinue"};
/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */
void IdleTaskFunction(void *argument);
void WatchdogTaskFunction(void *argument);
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
  idleTaskHandle = osThreadNew(IdleTaskFunction, NULL, &idleTaskAttributes);
  watchdogTaskHandle = osThreadNew(WatchdogTaskFunction, NULL, &watchdogTaskAttributes);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  tasksCompleteEventFlagsHandle = osEventFlagsNew(&tasksCompleteEventFlagsAttributes);
  tasksContinueEventFlagsHandle = osEventFlagsNew(&tasksContinueEventFlagsAttributes);
  /* USER CODE END RTOS_EVENTS */

  /* Reset IWDG on RTOS kernel start. */
  osEventFlagsSet(tasksCompleteEventFlagsHandle, ALL_TASKS_COMPLETE_FLAGS);
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/**
  * @brief  Function implementing the watchdogTask thread.
  * @param  argument: Not used
  * @retval None
  */
void WatchdogTaskFunction(void *argument)
{
  for(;;)
  {
    uint32_t flags = osEventFlagsWait(tasksCompleteEventFlagsHandle, ALL_TASKS_COMPLETE_FLAGS, osFlagsWaitAll, osWaitForever);
    if (flags == osFlagsErrorUnknown || flags == osFlagsErrorTimeout || flags == osFlagsErrorResource || flags == osFlagsErrorParameter)
    {
      /* IWDG should eventually expire if osEventFlagsWait() continues to fail. */
      osDelay(1);
      continue;
    }
    
    HAL_IWDG_Refresh(&hiwdg);
    osEventFlagsSet(tasksContinueEventFlagsHandle, ALL_TASKS_CONTINUE_FLAGS);
  }
}

/**
  * @brief  Function implementing the idleTask thread.
  * @param  argument: Not used
  * @retval None
  */
void IdleTaskFunction(void *argument)
{
  for(;;)
  {
    osDelay(1);
  }
}
/* USER CODE END Application */