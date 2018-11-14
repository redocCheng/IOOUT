/**
  * @file       ioout.h
  * @brief      IO输出控制头文件
  * @author     redoc
  * @version    v1.0
  * @date       2016-08-09
  *
  * @note
  * [2016-08-09] <redoc> v1.0
  * 初始版本
  * @remark
  */


/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __IOOUT_H
#define __IOOUT_H

/* Includes ------------------------------------------------------------------*/
#include "ioout_config.h"

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif


/* types ------------------------------------------------------------*/

/**
  * @brief IO输出控制回调函数类型定义
  */
typedef void (*IOOUTCALLBACK)(uint8_t);

/**
  * @brief IO输出error code
  */
typedef enum
{
    IOOUT_ERR_INIT = -4,        /**<  初始化错误  */
    IOOUT_ERR_PARAM = -3,       /**<  参数输入错误  */
    IOOUT_ERR_FAIL = -2,        /**<  申请失败  */
    IOOUT_ERR_MAX = -1,         /**<  超过最大值  */
    IOOUT_NO_ERR = 0,           /**<  无错误  */

}ioout_err_t;

/**
  * @brief IO输出控制计时表达结构
  */
typedef struct
{
    uint32_t interval;          /**<  间隔时间（占空比低）  */
    uint32_t workTime;          /**<  持续时间（占空比高）  */
    uint32_t ctlTime;           /**<  总时间  */
    uint32_t curCount;          /**<  单周期计数  */
    uint32_t sumCount;          /**<  总时间计数  */
    uint8_t enable;             /**<  使能  */
    uint8_t valid_enable;       /**<  是否有效，true表示有效，正在使用，false表示无效，可以分配  */
    uint8_t ioCtl;              /**<  控制变化标志  */
    uint16_t index;             /**<  序号  */
    IOOUTCALLBACK handle;       /**<  函数指针  */
} ioout_t;


/* constants --------------------------------------------------------*/
/* variables --------------------------------------------------------*/

/* macro ------------------------------------------------------------*/
/* functions --------------------------------------------------------*/
void ioout_config(void);
ioout_err_t ioout_init(ioout_id_t ioout_id,IOOUTCALLBACK timproc);
ioout_err_t ioout_set(ioout_id_t ioout_id,uint32_t interval,uint32_t workTime,uint32_t ctltime);
ioout_err_t ioout_stop(ioout_id_t ioout_id);
ioout_err_t ioout_start(ioout_id_t ioout_id);
ioout_err_t ioout_pause(ioout_id_t ioout_id);
ioout_err_t ioout_kill(ioout_id_t ioout_id);
void ioout_callback_process(void);

void ioout_port_init(void);

#ifdef __cplusplus
}
#endif

#endif  /*__IOOUT_H*/

