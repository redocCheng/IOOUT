/*
 * Copyright (c) 2019, Anke Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-22     redoc        the first version
 */
 
#include "ioout_rtt.h"

//#define DRV_DEBUG
#define LOG_TAG             "drv.ioout"
#include <drv_log.h>

#ifdef IOOUT_USE_RT_DEVICE
struct rt_ioout_device
{
    struct rt_device parent;
    ioout_t ioout;
    void(*ioout_cb)(uint8_t);
};

static rt_err_t rt_ioout_open(rt_device_t dev, rt_uint16_t oflag)
{
	rt_err_t result = RT_EOK;
    struct rt_ioout_device *ioout_device;
	
	RT_ASSERT(dev != RT_NULL);
	RT_ASSERT(dev->user_data != RT_NULL);
	
	ioout_device = (struct rt_ioout_device *)dev->user_data;
	
	result = ioout_init(&ioout_device->ioout,ioout_device->ioout_cb);
    
    LOG_D("%s init", dev->parent.name);
	
	return result;
}

static rt_err_t rt_ioout_close(rt_device_t dev)
{
	rt_err_t result = RT_EOK;
    struct rt_ioout_device *ioout_device;
	
	RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);
	
	ioout_device = (struct rt_ioout_device *)dev->user_data;
	
	result = ioout_kill(&ioout_device->ioout);
    
    LOG_D("%s close", dev->parent.name);
	
	return result;
}

static rt_size_t rt_ioout_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
	rt_err_t result = RT_EOK;
    struct rt_ioout_device *ioout_device;
	struct ioout_setvalue_struct *ioout_setvalue;
	
	RT_ASSERT(dev != RT_NULL);
	RT_ASSERT(buffer != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);
	
	ioout_device = (struct rt_ioout_device *)dev->user_data;
	ioout_setvalue = (struct ioout_setvalue_struct *)buffer;
	
	result = ioout_set( ioout_device->ioout, 
                        ioout_setvalue->interval, 
                        ioout_setvalue->work_time, 
                        ioout_setvalue->ctrl_time, 
                        ioout_setvalue->interval_first);
								
    LOG_D(  "%s write data :%d %d %d %d", 
            dev->parent.name, 
            ioout_setvalue->interval, 
            ioout_setvalue->work_time, 
            ioout_setvalue->ctrl_time, 
            ioout_setvalue->interval_first);
    
	return result;
}

static rt_err_t rt_ioout_ctrl(rt_device_t dev, int cmd, void *args)
{
	rt_err_t result = RT_EOK;
    struct rt_ioout_device *ioout_device;
	
	RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(dev->user_data != RT_NULL);
	
	ioout_device = (struct rt_ioout_device *)dev->user_data;
	
	switch (cmd)
    {
	case RT_IOOUT_CTRL_START  :
		result = ioout_start(ioout_device->ioout);
		break;
		
	case RT_IOOUT_CTRL_STOP   :
		result = ioout_stop(ioout_device->ioout);
		break;
		
	case RT_IOOUT_CTRL_PAUSE  :
		result = ioout_pause(ioout_device->ioout);
		break;
		
	case RT_IOOUT_CTRL_SET    :
		if (args)
		{
			struct ioout_setvalue_struct *ioout_setvalue;
			ioout_setvalue = (struct ioout_setvalue_struct *)args;
            
			result = ioout_set( ioout_device->ioout, 
								ioout_setvalue->interval, 
								ioout_setvalue->work_time, 
								ioout_setvalue->ctrl_time, 
								ioout_setvalue->interval_first);
		}
		break;
	default:
		break;
	}
	
	return result;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops ioout_ops =
    {
        RT_NULL,
        rt_ioout_open,
        rt_ioout_close,
        RT_NULL,
        rt_ioout_write,
        rt_ioout_ctrl};
#endif

static rt_err_t rt_hw_ioout_register(  struct rt_ioout_device *ioout_device,
                                       const char              *name,
                                       rt_uint32_t              flag,
                                       void                    *data)
{
    rt_err_t ret;
    struct rt_device *device;
    RT_ASSERT(ioout_device != RT_NULL);

    device = &(ioout_device->parent);

    device->type        = RT_Device_Class_Char;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->ops         = &ioout_ops;
#else
    device->init        = RT_NULL;
    device->open        = rt_ioout_open;
    device->close       = rt_ioout_close;
    device->read        = RT_NULL;
    device->write       = rt_ioout_write;
    device->control     = rt_ioout_ctrl;
#endif
    device->user_data   = data;

    /* register a character device */
    ret = rt_device_register(device, name, flag);

    return ret;
}

rt_err_t rt_hw_ioout_device_init(const char *device_name, void(*ioout_cb)(uint8_t))
{
    rt_err_t result = RT_EOK;
    struct rt_ioout_device *ioout_device;
    
    ioout_device = (struct rt_ioout_device *)rt_malloc(sizeof(struct rt_ioout_device));
    RT_ASSERT(ioout_device != RT_NULL);
    RT_ASSERT(ioout_cb != RT_NULL);
    
    ioout_device->ioout_cb = ioout_cb;
    ioout_device->ioout = RT_NULL;
    
    result = rt_hw_ioout_register(ioout_device, device_name, RT_NULL, ioout_device);
    
    if (result != RT_EOK)
    {
        LOG_E("%s register faild, %d\n", device_name, result);
    }

    RT_ASSERT(result == RT_EOK);

    LOG_D("%s register done", device_name);
    
    return result;
}

#endif
