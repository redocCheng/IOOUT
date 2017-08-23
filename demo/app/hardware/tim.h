/**
  * @file		tim.h
  * @brief		硬件定时器驱动头文件
  * @author 	redoc <619675912@qq.com>
  * @version	v1.0
  * @date		2017-08-10
  *
  * @note
  * [x2017-08-10] <redoc> v1.0
  * 初始版本
  *
  * @remark
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H
#define __TIM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* types ------------------------------------------------------------*/
/* define -----------------------------------------------------------*/
/* variables --------------------------------------------------------*/

extern TIM_HandleTypeDef htim3;

/* macro ------------------------------------------------------------*/
/* functions --------------------------------------------------------*/

void MX_TIM3_Init(void);

#ifdef __cplusplus
}
#endif

#endif

