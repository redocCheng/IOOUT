/**
  * @file		gpio.h
  * @brief		gpio驱动头文件
  * @author 	redoc <619675912@qq.com>
  * @version	v1.0
  * @date		2017-08-26
  *
  * @note
  * [x2017-08-26] <redoc> v1.0
  * 初始版本
  *
  * @remark
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H
#define __GPIO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* types ------------------------------------------------------------*/
/* define -----------------------------------------------------------*/
/* variables --------------------------------------------------------*/
/* macro ------------------------------------------------------------*/
/**
  * @brief  LED0_OUT 
  */
#define LED0_OUT_PIN                       GPIO_PIN_4
#define LED0_OUT_PORT                      GPIOC
#define LED0_OUT_INIT                      GPIO_PIN_RESET	
	
/**
  * @brief  LED1_OUT 
  */
#define LED1_OUT_PIN                       GPIO_PIN_5
#define LED1_OUT_PORT                      GPIOC
#define LED1_OUT_INIT                      GPIO_PIN_RESET	
	
	
/* functions --------------------------------------------------------*/
void MX_GPIO_Init(void);
void GPIO_SetLed0(uint8_t state);
void GPIO_SetLed1(uint8_t state);


#ifdef __cplusplus
}
#endif

#endif

