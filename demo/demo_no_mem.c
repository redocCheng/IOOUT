/*
 * Copyright (c) 2019, redoc
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-14     redoc        the first version
 */

#include "ioout.h"


static struct ioout_struct ioout_beep;
static void gpio_set_beep(uint8_t value);

int beep_init(void)
{
    if(-1 == ioout_init(&ioout_beep,gpio_set_beep))
    {

    }
    
    return 0;
}

static void gpio_set_beep(uint8_t value)
{
    //set_beep
}

void beep_stop(void)
{
    ioout_stop(&ioout_beep);
}

void beep_show_succ(void)
{
    ioout_set(&ioout_beep,0,100,800,false);
}

void beep_show_fail(void)
{
    ioout_set(&ioout_beep,250,250,1500,false);
}

void beep_alarm(void)
{
    ioout_set(&ioout_beep,250,250,0,false);
}


