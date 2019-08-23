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

#ifdef IOOUT_USE_MEM
int ioout_init(ioout_t *handle, void(*ioout_cb)(uint8_t))
#else
int ioout_init(ioout_t handle, void(*ioout_cb)(uint8_t))
#endif
{
    ioout_t target = head_handle;
    ioout_t handle_malloc = NULL;
    
    if(NULL != ioout_cb)   
    {
#ifdef IOOUT_USE_MEM
       handle_malloc = ioout_malloc(sizeof(struct ioout_struct)); 
       *handle = handle_malloc;
#else
       handle_malloc =  handle;
#endif
    }
    else
    {
        return -1;
    }
    
    memset(handle_malloc,0,sizeof(struct ioout_struct));
    
    handle_malloc->ioout_cb = ioout_cb;
    handle_malloc->ioout_cb(false);  
    
    while(target) 
    {
        if(target == handle_malloc) 
        {
            return -1;    
        }
        target = target->next;
    }
    
    handle_malloc->next = head_handle;
    head_handle = handle_malloc;
    
    return 0;
}

#ifdef IOOUT_USE_MEM
int ioout_kill(ioout_t* handle)
#else
int ioout_kill(ioout_t handle)
#endif
{   
#ifdef IOOUT_USE_MEM
    ioout_t handle_malloc = *handle;
#else
    ioout_t handle_malloc = handle;
#endif    
    
    if(NULL == handle_malloc)
    {
        return -1;
    }
    
    ioout_t* curr;
    
    for(curr = &head_handle; *curr; ) 
    {
        ioout_t entry = *curr;
        if (entry == handle_malloc) 
        {
            *curr = entry->next;
        } 
        else
        {
            curr = &entry->next;
        }
    }
    
#ifdef IOOUT_USE_MEM
    ioout_free(handle_malloc); 
    *handle = NULL;
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

int ioout_set(ioout_t handle, ioout_setvalue_t setvalue)
{
    if(NULL == handle)
    {
        return -1;
    }
    
    uint32_t base_time = IOOUT_BASE_TIME;

    if(((setvalue->interval % base_time ) != 0 )
    || ((setvalue->work_time % base_time ) != 0 )
    || ((setvalue->ctrl_time % base_time ) != 0 ))
    {
        return -1;
    }
    
    if(0 == setvalue->work_time)
    {
        return -1;
    }
    
    if(setvalue->interval != 0)
    {
       handle->interval = setvalue->interval/base_time; 
    }
    else
    {
        handle->interval = 0;
    }
    
    handle->work_time = setvalue->work_time/base_time; 

    if(setvalue->ctrl_time != 0)
    {
       handle->ctrl_time = setvalue->ctrl_time/base_time; 
    }
    else
    {
        handle->ctrl_time = 0;
    }
    
    handle->sum_count = 0;
    handle->enable = true;
    
    if(setvalue->interval_first)
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




