/**
  * Copyright (c) 2016-2018, redocCheng, <619675912@qq.com>
  * 
  * @file       ioout_port.c
  * @brief      ioout_port文件
  * @author     redoc
  * @version    v1.0
  * @date       2018-11-09
  *
  * @note

  *
  * @remark
  */
#include "ioout.h"

void gpio_set_led1(uint8_t value);
void gpio_set_led2(uint8_t value);

/**
  * @brief  配置ioout
  * @param  void
  *
  * @return void
  * @remark
  */
void ioout_port_init(void)
{
  
    if(IOOUT_NO_ERR != ioout_init(ID_IOOUT_LED1,gpio_set_led1))
    {

    }

    if(IOOUT_NO_ERR != ioout_init(ID_IOOUT_LED2,gpio_set_led2))
    {

    }
	
	
	
}


static void gpio_set_led1(uint8_t value)
{
	//set led1
}

static void gpio_set_led2(uint8_t value)
{
	//set led2
}

