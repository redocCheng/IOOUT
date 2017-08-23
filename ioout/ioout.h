/**
  * @file		ioout.h
  * @brief		IO输出控制头文件
  * @author 	redoc
  * @version	v1.0
  * @date		2016-08-09
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

/*------------ 移植配置Start */	
	
/**
  * @brief IO输出控制ID
  */
typedef enum
{
    ID_IOOUT_LED0 = 0,
	ID_IOOUT_LED1,

} iooutId_Typedef;

/**
  * @brief IO输出控制数量类型定义
  */
typedef enum
{
    IOOUT_MAX = (ID_IOOUT_LED1 + 1),

} ioOutIndexNum_Typedef;

/*------------- 移植配置End */	


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

}iooutErrCode_Typedef;

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
} ioOut_Typedef;


/* constants --------------------------------------------------------*/
/* variables --------------------------------------------------------*/
/* macro ------------------------------------------------------------*/
/* functions --------------------------------------------------------*/
void ioout_Config(void);
iooutErrCode_Typedef ioout_Init(iooutId_Typedef iooutId,IOOUTCALLBACK timproc);
iooutErrCode_Typedef ioout_Set(iooutId_Typedef iooutId,uint32_t interval,uint32_t workTime,uint32_t ctltime);
iooutErrCode_Typedef ioout_Stop(iooutId_Typedef iooutId);
iooutErrCode_Typedef ioout_Start(iooutId_Typedef iooutId);
iooutErrCode_Typedef ioout_Pause(iooutId_Typedef iooutId);
iooutErrCode_Typedef ioout_Kill(iooutId_Typedef iooutId);
void ioout_CallBackProcRoutine(void);

#ifdef __cplusplus
}
#endif

#endif  /*__IOOUT_H*/

