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
#include "gpio.h"



/* typedef -----------------------------------------------------------*/
/* define ------------------------------------------------------------*/
/* macro -------------------------------------------------------------*/
/* variables ---------------------------------------------------------*/



/* function prototypes -----------------------------------------------*/
/* functions ---------------------------------------------------------*/


/**
  * @brief	GPIO init function
  * @param	void
  *
  * @return	void
  * @remark 软定时器
  */
void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
	GPIO_InitStruct.Pin   = LED0_OUT_PIN;
	HAL_GPIO_Init(LED0_OUT_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin   = LED1_OUT_PIN;
	HAL_GPIO_Init(LED1_OUT_PORT, &GPIO_InitStruct);
	

}

/**
  * @brief	LED0设置
  * @param	state：设置状态 true or false
  *
  * @return	void
  * @remark 
  */
void GPIO_SetLed0(uint8_t state)
{
	HAL_GPIO_WritePin(LED0_OUT_PORT, LED0_OUT_PIN, (state == LED0_OUT_INIT) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
  * @brief	LED1设置
  * @param	state：设置状态 true or false
  *
  * @return	void
  * @remark 
  */
void GPIO_SetLed1(uint8_t state)
{
	HAL_GPIO_WritePin(LED1_OUT_PORT, LED1_OUT_PIN, (state == LED1_OUT_INIT) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


