/**
  * @file		tim.c
  * @brief		硬件定时器驱动
  * @author 	redoc <619675912@qq.com>
  * @version	v1.0
  * @date		2017-08-08
  *
  * @note
  * [2017-08-08] <redoc> v1.0
  * 初始版本
  *
  * @remark
  */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "tim.h"
#include "ioout.h"


/* typedef -----------------------------------------------------------*/
/* define ------------------------------------------------------------*/
/* macro -------------------------------------------------------------*/
/* variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;


/* function prototypes -----------------------------------------------*/
/* functions ---------------------------------------------------------*/


/**
  * @brief	TIM3 init function
  * @param	void
  *
  * @return	void
  * @remark 软定时器
  */
void MX_TIM3_Init(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;

    //10ms
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 71;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 10000-1;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&htim3);

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

    HAL_TIM_Base_Start_IT(&htim3);
}

/**
  * @brief  Initializes the TIM Base MSP.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();

        HAL_NVIC_SetPriority(TIM3_IRQn,INT_PRO_SOFTTIME, 0);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }
}

/**
  * @brief  DeInitializes TIM Base MSP.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
    if(htim_base->Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_DISABLE();

        HAL_NVIC_DisableIRQ(TIM3_IRQn);
    }
}


/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if(htim == &htim3)
    {
        ioout_CallBackProcRoutine();
    }


}




