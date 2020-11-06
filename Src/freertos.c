﻿/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "WTN6.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "button.h"
#include "gpio.h"
#include "BspSound.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint8_t  TIM5CH1_CAPTURE_STA = 0;	//ÊäÈë²¶»ñ×´Ì¬		    				
uint16_t	TIM5CH1_CAPTURE_VAL;	//ÊäÈë²¶»ñÖµ
uint32_t temp = 0;
//按键变量
uint16_t KeyValue_t = 0xffff; uint16_t lastvalue_t = 0xffff;
Key_Message keys[2] = { 0 };
uint8_t flag = 0;
float res;
uint8_t led_renum = 0;
//线程句柄
osThreadId SensorDriveHandle;//传感器驱动线程
osThreadId ListenerTaskHandle;    //监听线程
/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
//按键注册和事件回调函数
void  Key_CallBack(Key_Message index);
void SensorDrive_CallBack(void const *argument);
void Listener_CallBack(void const *argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);

/* USER CODE BEGIN 2 */
__weak void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
}
/* USER CODE END 2 */

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

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	
	taskENTER_CRITICAL(); //进入临界区

	osThreadDef(SensorDrive, SensorDrive_CallBack, 4, 0, 128);
	SensorDriveHandle = osThreadCreate(osThread(SensorDrive), NULL);

	osThreadDef(Listener,Listener_CallBack,2,0,128);
	ListenerTaskHandle = osThreadCreate(osThread(Listener), NULL);

	vTaskDelete(defaultTaskHandle); //删除任务
	taskEXIT_CRITICAL();//推出临界区
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void SensorDrive_CallBack(void const *argument)
{
	WTN6_PlayOneByte(0xe1);//调节音量
	osDelay(100);
	for (;;)
	{
		ScanKeys(&KeyValue_t, &lastvalue_t, keys, Key_CallBack);
		//printf1("in while \r\n");
		osDelay(20);
	}
}
void Listener_CallBack(void const *argument)
{
	for (;;)
	{
		//printf1("task listener************************** \r\n");
		HAL_GPIO_TogglePin(LED_LEFT_PORT, LED_LEFT_PIN);
		osDelay(500);
	}
}
void  Key_regist(void)
{
	keys[0].GPIOx = GPIOA;
	keys[0].GPIO_Pin = KEY_START_Pin;
	keys[0].Key_count = 2;

	keys[1].GPIOx = GPIOA;
	keys[1].GPIO_Pin = KEY_USER_Pin;
	keys[1].Key_count = 2;
}
//按键回调函数
void  Key_CallBack(Key_Message index)
{
	//printf1("The KEY_USER is passed!\r\n");
	if (index.GPIO_Pin == KEY_START_Pin)
	{
		printf1("The KEY_START is passed_pin8!\r\n");
	}
	if (index.GPIO_Pin == KEY_USER_Pin)
	{
		printf1("The KEY_USER is passed_pin11!\r\n");
		flag = 1;
		TIM5CH1_CAPTURE_STA = 0;
		BeginSound();
		while (flag)
		{
			
			printf1("in while \r\n");
			osDelay(10);
			if (led_renum++ > 50)
			{
				led_renum = 0;
				HAL_GPIO_TogglePin(LED_LEFT_PORT, LED_RIGHT_PIN);//跳起后此led闪耀
			}
				
			if (TIM5CH1_CAPTURE_STA & 0X80)
			{
				temp = TIM5CH1_CAPTURE_STA & 0X3F;
				temp *= 65536;//溢出时间总和
				temp += TIM5CH1_CAPTURE_VAL;////得到总的高电平时间
				printf("HIGH:%d us\r\n", temp);//打印总的高点平时间
				TIM5CH1_CAPTURE_STA = 0;//开始下一次捕获
				osDelay(1000); //延时一会
				res = temp / 1000.00 / 1000.00;
				//res = (res*10);
				printf("HIGH/1000====%dmm\r\n", (uint32_t)(res*res*1250));////输出毫米
				ProcessHeight((double)(res*res * 1250 / 10.00));
				flag = 0;
			}
			
		}
		
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
