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

