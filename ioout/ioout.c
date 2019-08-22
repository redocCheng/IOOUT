/*
 * Copyright (c) 2019, redoc
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-14     redoc        the first version
 * 2019-08-22     redoc        add interval first change
 */

#include "ioout.h"

static ioout_t head_handle = NULL;

int ioout_init(ioout_t handle, void(*ioout_cb)(uint8_t))
{
    ioout_t target = head_handle;
    
    if(NULL != ioout_cb)   
    {
#ifdef IOOUT_USE_MEM
       handle = ioout_malloc(sizeof(struct ioout_struct)); 
#endif
    }
    else
    {
        return -1;
    }
    
    memset(handle,0,sizeof(struct ioout_struct));
    
    handle->ioout_cb = ioout_cb;
    handle->ioout_cb(false);  
    
    while(target) 
    {
        if(target == handle) 
        {
            return -1;    
        }
        target = target->next;
    }
    
    handle->next = head_handle;
    head_handle = handle;
    
    return 0;
}

int ioout_kill(ioout_t handle)
{
    if(NULL == handle)
    {
        return -1;
    }
    
    ioout_t* curr;
    
    for(curr = &head_handle; *curr; ) 
    {
        ioout_t entry = *curr;
        if (entry == handle) 
        {
            *curr = entry->next;
        } 
        else
        {
            curr = &entry->next;
        }
    }
    
#ifdef IOOUT_USE_MEM
    ioout_free(handle); 
#endif
    
    return 0;
}

int ioout_start(ioout_t handle)
{
    if(NULL == handle)
    {
        return -1;
    }
    
    handle->enable = true;
    
    return 0;
}

int ioout_stop(ioout_t handle)
{
    if(NULL == handle)
    {
        return -1;
    }
 
    handle->ioout_cb(false);
    handle->interval = 0;
    handle->work_time = 0;
    handle->ctrl_time = 0;
    handle->cur_count = 0;
    handle->sum_count = 0;
    handle->enable = false;
    
    return 0;
}

int ioout_pause(ioout_t handle)
{
    if(NULL == handle)
    {
        return -1;
    }
    
    handle->ioout_cb(false);
    handle->enable = false;
    
    return 0;
}

int ioout_set(ioout_t handle, uint32_t interval, uint32_t work_time, uint32_t ctrl_time, uint8_t interval_first)
{
    if(NULL == handle)
    {
        return -1;
    }
    
    uint32_t base_time = IOOUT_BASE_TIME;

    if(((interval % base_time ) != 0 )
    || ((work_time % base_time ) != 0 )
    || ((ctrl_time % base_time ) != 0 ))
    {
        return -1;
    }
    
    if(0 == work_time)
    {
        return -1;
    }
    
    if(interval != 0)
    {
       handle->interval = interval/base_time; 
    }
    else
    {
        handle->interval = 0;
    }
    
    handle->work_time = work_time/base_time; 

    if(ctrl_time != 0)
    {
       handle->ctrl_time = ctrl_time/base_time; 
    }
    else
    {
        handle->ctrl_time = 0;
    }
    
    handle->sum_count = 0;
    handle->enable = true;
    
    if(interval_first)
    {
        handle->ioout_cb(false);
        handle->io_state = false;
    }
    else
    {
        handle->ioout_cb(true);
        handle->io_state = true;
    }  
    
    return 0;
}

void ioout_loop(void)
{
    ioout_t target;
    
    for(target = head_handle; target; target = target->next) 
    {
        if( !target->enable)
        {
            target->ioout_cb(false);
            target->sum_count = 0;
            continue;
        }
        else
        {
            if(0 == target->interval)
            {
                target->ioout_cb(true);

                if(target->ctrl_time)
                {
                    target->sum_count++;

                    if(target->sum_count >= target->ctrl_time)
                    {
                        target->ioout_cb(false);
                        target->sum_count = 0;
                        target->io_state = false;
                        target->enable = false;
                    }
                }
            }
            else
            {
                target->sum_count++;

                /*  ctrl_time  */
                if(target->ctrl_time)
                {
                    if(target->sum_count >= target->ctrl_time)
                    {
                        target->ioout_cb(false);
                        target->sum_count = 0;
                        target->enable = false;
                        target->io_state = false;
                        target->cur_count = 0;
                        continue;
                    }
                }
                else
                {
                    target->sum_count=0;
                }

                target->cur_count++;
                target->ioout_cb(target->io_state);

                /*  work_time  */
                if(true == target->io_state)
                {
                    if(target->cur_count >= target->work_time)
                    {
                        target->ioout_cb(false);
                        target->io_state = false;
                        target->cur_count = 0;
                    }
                }
                /*  interval  */
                else
                {
                    if(target->cur_count >= target->interval)
                    {
                        target->ioout_cb(true);
                        target->io_state = true;
                        target->cur_count = 0;
                    }
                }
            }
        } 
    }
}




