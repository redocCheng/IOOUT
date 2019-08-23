/*
 * Copyright (c) 2019, Anke Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-22     redoc        the first version
 */

#ifndef __IOOUT_RTT_H
#define __IOOUT_RTT_H
 
#include <rtthread.h>
#include "ioout.h"

#ifdef IOOUT_USE_RT_DEVICE

#define RT_IOOUT_CTRL_START  1
#define RT_IOOUT_CTRL_STOP   2
#define RT_IOOUT_CTRL_PAUSE  3
#define RT_IOOUT_CTRL_SET    4

struct ioout_setvalue_struct
{
	uint16_t interval;
	uint16_t work_time;
	uint16_t ctrl_time;
	uint8_t  interval_first;
};

rt_err_t rt_hw_ioout_device_init(const char *device_name, void(*ioout_cb)(uint8_t));
#endif

#endif



