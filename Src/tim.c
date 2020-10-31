/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
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

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
extern uint8_t  TIM5CH1_CAPTURE_STA;	//ÊäÈë²¶»ñ×´Ì¬		    				
extern uint16_t	TIM5CH1_CAPTURE_VAL;	//ÊäÈë²¶»ñÖµ

typedef struct              //???????
{
	uint8_t   ucFinishFlag; //??
	uint8_t   ucreceiveFlag;  //??
	uint16_t  usCtr;
	uint16_t  usPeriod;
}STRUCT_CAPTURE;

uint32_t ulTmrClk, ulTime, ulTimea, ulTimeb;
STRUCT_CAPTURE strCapture = { 0, 0, 0 };
void user_ic_config(uint16_t value)
{
	TIM_IC_InitTypeDef sConfigIC;
	if (value != 0)  //1
	{
		sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;//???
	}
	else//0
	{
		sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING; //???
	}
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1);
}

void user_pwm_setvalue(uint16_t value)
{
	TIM_OC_InitTypeDef sConfigOC;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = value;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}
/* USER CODE END 0 */

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* TIM2 init function */
void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 72;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xFFFF;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

}
/* TIM3 init function */
void MX_TIM3_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 72;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 500;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 50;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_MspPostInit(&htim3);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration    
    PA0-WKUP     ------> TIM2_CH1 
    */
    GPIO_InitStruct.Pin = IR_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(IR_IN_GPIO_Port, &GPIO_InitStruct);

    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspPostInit 0 */

  /* USER CODE END TIM3_MspPostInit 0 */
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM3 GPIO Configuration    
    PA6     ------> TIM3_CH1 
    */
    GPIO_InitStruct.Pin = PWM_OUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(PWM_OUT_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM3_MspPostInit 1 */

  /* USER CODE END TIM3_MspPostInit 1 */
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();
  
    /**TIM2 GPIO Configuration    
    PA0-WKUP     ------> TIM2_CH1 
    */
    HAL_GPIO_DeInit(IR_IN_GPIO_Port, IR_IN_Pin);

    /* TIM2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	TIM_IC_InitTypeDef sConfigIC;
	if (TIM2 == htim->Instance)
	{

		if ((TIM5CH1_CAPTURE_STA & 0x80) == 0)
		{
			if ((TIM5CH1_CAPTURE_STA & 0x40))
			{
				TIM5CH1_CAPTURE_STA |= 0x80;
				TIM5CH1_CAPTURE_VAL = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);//È¡cntÖµ
				/*ÅäÖÃÊäÈë²¶»ñ¼Ä´æÆ÷*/                                      //ÉèÖÃÉÏÉýÑØ²¶»ñ
				user_ic_config(1);
				/*Çå³ýÖÐ¶Ï*/
				__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);
				/*Æô¶¯ÊäÈë²¶»ñ²¢¿ªÆô²¶»ñÖÐ¶Ï*/
				HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
			}
			else
			{
				TIM5CH1_CAPTURE_STA = 0;
				TIM5CH1_CAPTURE_VAL = 0;
				TIM5CH1_CAPTURE_STA |= 0x40;
				__HAL_TIM_SET_COUNTER(htim, 0);//cnt¼Ä´æÆ÷ÖµÇåÁã
				user_ic_config(0);
				__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);
				HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
			}
		}

	}

}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (TIM2 == htim->Instance)
	{

		if ((TIM5CH1_CAPTURE_STA & 0x80) == 0) //»¹Ã»³É¹¦
		{
			if ((TIM5CH1_CAPTURE_STA & 0x40)) //ÒÑ¾­È¡µÄÁË¸ßµçÆ½
			{
				if ((TIM5CH1_CAPTURE_STA & 0x3F) == 0x3F)
				{
					TIM5CH1_CAPTURE_STA |= 0x80;
					TIM5CH1_CAPTURE_VAL = 0XFFFF;
				}
				else TIM5CH1_CAPTURE_STA++;
			}
		}
	}
}

void MX_TIM_INCapture_CallBack(TIM_HandleTypeDef *htim) ///定时器中断回调函数
{
	if ((TIM5CH1_CAPTURE_STA & 0X80) == 0)//还未成功捕获	
	{
		if (__HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE) != RESET)
		{
			if (__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_UPDATE) != RESET)
			{
				__HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);
				if (TIM5CH1_CAPTURE_STA & 0X40)//已经捕获到高电平了
				{
					if ((TIM5CH1_CAPTURE_STA & 0X3F) == 0X3F)//高电平太长了
					{
						TIM5CH1_CAPTURE_STA |= 0X80;//标记成功捕获了一次
						TIM5CH1_CAPTURE_VAL = 0XFFFF;
					}
					else 
						TIM5CH1_CAPTURE_STA++;
				}

			}
		}
		if (__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC1) != RESET)
		{
			if (__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_CC1) != RESET)
			{
				{
					__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);
					htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;

					/* Input capture event */
					if ((htim->Instance->CCMR1 & TIM_CCMR1_CC1S) != 0x00U)
					{
						//HAL_TIM_IC_CaptureCallback(htim);
						if (TIM5CH1_CAPTURE_STA & 0X40)		//捕获到一个下降沿 		
						{
							TIM5CH1_CAPTURE_STA |= 0X80;		//标记成功捕获到一次高电平脉宽
							TIM5CH1_CAPTURE_VAL = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
							//TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
							user_ic_config(1);
							HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
						}
						else  								//还未开始,第一次捕获上升沿
						{
							TIM5CH1_CAPTURE_STA = 0;			//清空
							TIM5CH1_CAPTURE_VAL = 0;
							__HAL_TIM_SET_COUNTER(htim, 0);
							TIM5CH1_CAPTURE_STA |= 0X40;		//标记捕获到了上升沿
							//TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
							user_ic_config(0);
							HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
						}
					}
					/* Output compare event */
					else
					{
						HAL_TIM_OC_DelayElapsedCallback(htim);
						HAL_TIM_PWM_PulseFinishedCallback(htim);
					}
					htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
				}
			}
		}
	}

	__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1 | TIM_IT_UPDATE);
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
