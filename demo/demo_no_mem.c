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

#define VALUE_BEEP_SHOW_SUCC   {0,100,800,false}
#define VALUE_BEEP_SHOW_FAIL   {250,250,1500,false}
#define VALUE_BEEP_SHOW_ALARM  {250,250,0,false}

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

int beep_stop(void)
{
    return ioout_stop(&iioout_beep);
}

int beep_show_succ(void)
{
	struct ioout_setvalue_struct show_succ = VALUE_BEEP_SHOW_SUCC;
    return ioout_set(&ioout_beep, &show_succ);
}

int beep_show_fail(void)
{
	struct ioout_setvalue_struct show_fail = VALUE_BEEP_SHOW_FAIL;
    return ioout_set(&iioout_beep, &show_fail);
}

int beep_alarm(void)
{
	struct ioout_setvalue_struct show_alarm = VALUE_BEEP_SHOW_ALARM;
    return ioout_set(&iioout_beep, &show_alarm);
}

