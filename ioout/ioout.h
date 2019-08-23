/*
 * Copyright (c) 2019, Anke Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-14     redoc        the first version
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __IOOUT_H
#define __IOOUT_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "ioout_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#if !defined(IOOUT_BASE_TIME)
    #error "Please configure basic time (in ioout_cfg.h)"
#endif
    
/* types ------------------------------------------------------------*/
struct ioout_struct
{
    uint16_t interval;          /**<  间隔时间（占空比低）     */
    uint16_t work_time;         /**<  持续时间（占空比高）     */
    uint16_t ctrl_time;         /**<  总时间                   */
    uint16_t cur_count;         /**<  单周期计数               */
    uint16_t sum_count;         /**<  总时间计数               */
    uint8_t  enable       :1;   /**<  使能                     */
    uint8_t  io_state     :1;   /**<  控制变化标志             */
    uint8_t  reserved     :6;   /**<  保留                     */
    void (*ioout_cb)(uint8_t);  /**<  回调函数 输入true/false  */
    struct ioout_struct* next;
};

typedef struct ioout_struct *ioout_t;

struct ioout_setvalue_struct
{
	uint16_t interval;
	uint16_t work_time;
	uint16_t ctrl_time;
	uint8_t  interval_first;
};

typedef struct ioout_setvalue_struct *ioout_setvalue_t;

/* constants --------------------------------------------------------*/
/* variables --------------------------------------------------------*/
/* macro ------------------------------------------------------------*/
/* functions --------------------------------------------------------*/
// ioout_port.c
int ioout_config(void);

#ifdef IOOUT_USE_MEM
void *ioout_malloc(uint32_t nbytes);
void ioout_free (void *ptr);
#endif

// ioout.c
#ifdef IOOUT_USE_MEM
int ioout_init(ioout_t *handle, void(*ioout_cb)(uint8_t));
int ioout_kill(ioout_t *handle);
#else
int ioout_init(ioout_t handle, void(*ioout_cb)(uint8_t));
int ioout_kill(ioout_t handle);
#endif

int ioout_start(ioout_t handle);
int ioout_stop(ioout_t handle);
int ioout_pause(ioout_t handle);
int ioout_set(ioout_t handle, ioout_setvalue_t value);
void ioout_loop(void);

#ifdef __cplusplus
}
#endif

#endif  /*__IOOUT_H*/

