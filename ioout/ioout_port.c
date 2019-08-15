/*
 * Copyright (c) 2019, Anke Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-14     redoc        the first version
 */
 
#include "ioout.h"
#include "rtthread.h"


#ifdef IOOUT_USE_MEM

void *ioout_malloc(uint32_t nbytes)
{
    return rt_malloc(nbytes);
}

void ioout_free (void *ptr)
{
    rt_free(ptr);
}

#endif

static void timeout_ioout_cb(void *parameter)
{
    ioout_loop();
}

int ioout_config(void)
{
    rt_timer_t timer_ioout = rt_timer_create("timer_ioout", 
                                             timeout_ioout_cb,
                                             RT_NULL, 
                                             IOOUT_BASE_TIME,
                                             RT_TIMER_FLAG_PERIODIC);
    
    if (timer_ioout != RT_NULL)
    {        
        rt_timer_start(timer_ioout);
    }
    else
    {
        return -1;
    }
    
    return 0;
}
INIT_PREV_EXPORT(ioout_config);



