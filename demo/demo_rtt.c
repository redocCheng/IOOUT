/*
 * Copyright (c) 2019, Anke Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-14     redoc        the first version
 */

#include "drv_gpio.h"
#include "ioout_rtt.h"
#include "beep.h"

#define PIN_BEEP 	GET_PIN(E,1)

#define VALUE_BEEP_SHOW_SUCC   {0,100,800,false}
#define VALUE_BEEP_SHOW_FAIL   {250,250,1500,false}
#define VALUE_BEEP_SHOW_ALARM  {250,250,0,false}

static void gpio_set_beep(uint8_t value);
rt_device_t beep;

int beep_init(void)
{
    rt_pin_mode(PIN_BEEP,PIN_MODE_OUTPUT);

    rt_hw_ioout_device_init("beep", gpio_set_beep);

    beep = rt_device_find("beep");

    return rt_device_open(beep,RT_DEVICE_FLAG_WRONLY);
}
INIT_COMPONENT_EXPORT(beep_init);

static void gpio_set_beep(uint8_t value)
{
    if(value)
    {
        rt_pin_write(PIN_BEEP, PIN_HIGH);
    }
    else
    {
        rt_pin_write(PIN_BEEP, PIN_LOW); 
    }
}

int beep_stop(void)
{
    return rt_device_control(beep, RT_IOOUT_CTRL_STOP, RT_NULL);
}

int beep_show_succ(void)
{
    struct ioout_setvalue_struct show_succ = VALUE_BEEP_SHOW_SUCC;
    return rt_device_write(beep, 0, (uint8_t *)&show_succ, sizeof(struct ioout_setvalue_struct));
}

int beep_show_fail(void)
{
    struct ioout_setvalue_struct show_fail = VALUE_BEEP_SHOW_FAIL;
    return rt_device_write(beep, 0, (uint8_t *)&show_fail, sizeof(struct ioout_setvalue_struct));
}

int beep_show_fail(void)
{
    struct ioout_setvalue_struct show_alarm = VALUE_BEEP_SHOW_ALARM;
    return rt_device_write(beep, 0, (uint8_t *)&show_alarm, sizeof(struct ioout_setvalue_struct));
}
