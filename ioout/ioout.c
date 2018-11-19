/**
  * Copyright (c) 2016-2018, redocCheng, <619675912@qq.com>
  * 
  * @file       ioout.c
  * @brief      IO输出控制文件
  * @author     redoc
  * @version    v1.0
  * @date       2016-08-09
  *
  * @note
  * [2016-08-09] <redoc> v1.0
  * 初始版本
  *
  * @remark
  */

/* Includes ------------------------------------------------------------------*/
#include "ioout.h"



/* typedef -----------------------------------------------------------*/
/* define ------------------------------------------------------------*/
/* constants ---------------------------------------------------------*/
const uint16_t ioout_Time_Interval = IOOUT_TIMER_INTERVAL;            /**< 查询间隔    */


/* variables ---------------------------------------------------------*/
static ioout_t m_ioout_list[IOOUT_MAX];       /**<  IO管理表   */
static uint16_t ioout_id_buf[IOOUT_MAX] ;             /**<  IOID表   */
static uint8_t ioout_list_init_flag = false;                  /**<  ioout表初始化标志   */
static uint8_t ioout_init_flag = false;

/* functions ---------------------------------------------------------*/
static void ioout_list_init(void);
static ioout_err_t ioout_get_index_io(uint16_t *sIndex);
static ioout_err_t ioout_unlink_io(uint16_t index);
static ioout_err_t ioout_link_io(uint16_t index,IOOUTCALLBACK handle);
static ioout_err_t ioout_start_io(uint16_t index);
static ioout_err_t ioout_kill_io(uint16_t *sIndex);
static ioout_err_t ioout_stop_io(uint16_t index);
static ioout_err_t ioout_pause_io(uint16_t index);
static ioout_err_t ioout_set_param_io(uint16_t index,uint32_t interval,uint32_t beeptime,uint32_t ctltime);

/**
  * @brief  配置ioout
  * @param  void
  *
  * @return void
  * @remark
  */
void ioout_config(void)
{
    ioout_list_init();

    ioout_port_init();
    
    ioout_init_flag = true;
}

/**
  * @brief  IO管理表初始化
  * @param  无
  *
  * @return 无
  * @remark
  */
static void ioout_list_init(void)
{
    uint16_t i = 0;

    for(i = 0; i < IOOUT_MAX; i++)
    {
        m_ioout_list[i].interval        = 0;
        m_ioout_list[i].workTime        = 0;
        m_ioout_list[i].ctlTime         = 0;
        m_ioout_list[i].curCount        = 0;
        m_ioout_list[i].sumCount        = 0;
        m_ioout_list[i].handle          = NULL;
        m_ioout_list[i].enable          = false;
        m_ioout_list[i].valid_enable    = false;
        m_ioout_list[i].ioCtl           = false;
        m_ioout_list[i].index           = 0xFFFF;
        ioout_id_buf[i]                   = 0xFFFF;
    }

    ioout_list_init_flag = true;
}

/**
  * @brief  链接对应序号IO的操纵函数
  * @param  index：IO口序号
  * @param  handle：链接函数
  *
  * @return 设置状态
  * @remark
  */
static ioout_err_t ioout_link_io(uint16_t index,IOOUTCALLBACK handle)
{
    if(index > IOOUT_MAX)
    {
        return IOOUT_ERR_MAX;
    }

    if(handle == NULL)
    {
        return IOOUT_ERR_PARAM;
    }

    m_ioout_list[index].handle = handle;
    m_ioout_list[index].valid_enable     = true;

    return IOOUT_NO_ERR;
}

/**
  * @brief  清除序号index
  * @param  index：IO口序号
  *
  * @return 设置状态
  * @remark
  */
static ioout_err_t ioout_unlink_io(uint16_t index)
{
    if(index > IOOUT_MAX)
    {
        return IOOUT_ERR_MAX;
    }


    m_ioout_list[index].handle(false);
    m_ioout_list[index].interval    = 0;
    m_ioout_list[index].workTime        = 0;
    m_ioout_list[index].ctlTime     = 0;
    m_ioout_list[index].curCount      = 0;
    m_ioout_list[index].sumCount      = 0;
    m_ioout_list[index].handle        = NULL;
    m_ioout_list[index].index         = 0xFFFF;
    m_ioout_list[index].ioCtl           = false;
    m_ioout_list[index].enable        = false;
    m_ioout_list[index].valid_enable    = false;

    return IOOUT_NO_ERR;
}

/**
  * @brief  获取控制口的序号
  * @param  *mindex：IO口序号(传出)
  *
  * @return 设置状态
  * @remark
  */
static ioout_err_t ioout_get_index_io(uint16_t *sIndex)
{
    uint16_t i = 0;

    for(i = 0; i < IOOUT_MAX; i++)
    {
        if(false == m_ioout_list[i].valid_enable)
        {
            *sIndex = i;
            return IOOUT_NO_ERR;
        }
    }

    return IOOUT_ERR_FAIL;
}

/**
  * @brief  启动和恢复对应序号的IO
  * @param  index：IO口序号
  *
  * @return 设置状态
  * @remark
  */
static ioout_err_t ioout_start_io(uint16_t index)
{
    if(index == 0xFFFF)
    {
        return IOOUT_ERR_INIT;
    }

    if(false == m_ioout_list[index].enable)
    {
        m_ioout_list[index].enable     = true;
    }

    return IOOUT_NO_ERR;
}


/**
  * @brief  删除对应序号的IO
  * @param  index：IO口序号
  *
  * @return 设置状态
  * @remark 不可恢复，释放资源
  */
static ioout_err_t ioout_kill_io(uint16_t *sIndex)
{
    if(*sIndex == 0xFFFF)
    {
        return IOOUT_ERR_INIT;
    }

    ioout_unlink_io(*sIndex);
    *sIndex = 0xffff;

    return IOOUT_NO_ERR;
}

/**
  * @brief  停止对应序号的IO
  * @param  index：IO口序号
  *
  * @return 设置状态
  * @remark 不释放资源，放弃当前任务，不可恢复
  */
static ioout_err_t ioout_stop_io(uint16_t index)
{
    if(index == 0xFFFF)
    {
        return IOOUT_ERR_INIT;
    }


    m_ioout_list[index].handle(false);

    m_ioout_list[index].interval    = 0;
    m_ioout_list[index].workTime      = 0;
    m_ioout_list[index].ctlTime     = 0;
    m_ioout_list[index].curCount      = 0;
    m_ioout_list[index].sumCount      = 0;
    m_ioout_list[index].enable        = false;

    return IOOUT_NO_ERR;
}
/**
  * @brief  暂停对应序号的IO
  * @param  index：IO口序号
  *
  * @return 设置状态
  * @remark 暂停当前任务，恢复继续当前任务
  */
static ioout_err_t ioout_pause_io(uint16_t index)
{
    if(index == 0xFFFF)
    {
        return IOOUT_ERR_INIT;
    }

    m_ioout_list[index].handle(false);
    m_ioout_list[index].enable     = false;

    return IOOUT_NO_ERR;
}

/**
  * @brief  设置对应序号IO参数
  * @param  index：IO口序号
  * @param  interval：间隔时间
  * @param  worktime：持续时间
  * @param  ctltime：总时间
  *
  * @return ioout_err_t
  * @remark ms为单位,参数必须是查询时间的整数倍，此函数不允许worktime为零
  */
static ioout_err_t ioout_set_param_io(uint16_t index,uint32_t interval,uint32_t workTime,uint32_t ctltime)
{
    if(index == 0xFFFF)
    {
        return IOOUT_ERR_INIT;
    }

    if(((interval % ioout_Time_Interval ) != 0 )
    || ((interval % ioout_Time_Interval ) != 0 )
    || ((interval % ioout_Time_Interval ) != 0 ))
    {
        return IOOUT_ERR_PARAM;
    }

    if(0 == workTime)
    {
        return IOOUT_ERR_PARAM;
    }

    m_ioout_list[index].interval    = interval;
    m_ioout_list[index].workTime      = workTime;
    m_ioout_list[index].ctlTime     = ctltime;
    m_ioout_list[index].sumCount      = 0;
    m_ioout_list[index].enable        = true;

    return IOOUT_NO_ERR;
}

/**
  * @brief  初始化IOOUT使用接口函数
  * @param  timproc：IO口接口函数
  *
  * @return IO口号
  * @remark
  */
ioout_err_t ioout_init(ioout_id_t ioout_id,IOOUTCALLBACK timproc)
{
    if(false == ioout_list_init_flag)
    {
        return IOOUT_ERR_INIT;
    }

    if(ioout_id > IOOUT_MAX)
    {
        return IOOUT_ERR_MAX;
    }

    if(timproc == NULL)
    {
        return IOOUT_ERR_PARAM;
    }

    if(IOOUT_NO_ERR != ioout_get_index_io((uint16_t *)(ioout_id_buf + (uint8_t)ioout_id)))
    {
        return IOOUT_ERR_FAIL;
    }

    if(IOOUT_NO_ERR != ioout_link_io(ioout_id_buf[ioout_id],timproc))
    {
        return IOOUT_ERR_FAIL;
    }

    return IOOUT_NO_ERR;
}

/**
  * @brief  设置IO并启动
  * @param  ioout_id:输出口ID
  * @param  interval：间隔时间
  * @param  worktime：持续时间
  * @param  ctltime：总时间
  *
  * @return ioout_err_t
  * @remark 时间以ms为单位,参数必须是查询时间的整数倍，worktime为0停止，ctltime为0一直保持周期
  */
ioout_err_t ioout_set(ioout_id_t ioout_id,uint32_t interval,uint32_t workTime,uint32_t ctlTime)
{
    ioout_err_t status;

  if(false == ioout_init_flag)
  {
    ioout_config();
  }
  
    if(ioout_id > IOOUT_MAX)
    {
        return IOOUT_ERR_MAX;
    }

    if(workTime != 0)
    {
        status == ioout_set_param_io(ioout_id_buf[ioout_id],interval,workTime,ctlTime);

        if(IOOUT_NO_ERR == status)
        {
            ioout_start_io(ioout_id_buf[ioout_id]);
        }
        else
        {
            return status;
        }
    }
    else
    {
        ioout_stop_io(ioout_id_buf[ioout_id]);
    }

    return IOOUT_NO_ERR;
}

/**
  * @brief  停止,数据清零
  * @param  ioout_id:输出口ID
  *
  * @return ioout_err_t
  * @remark
  */
ioout_err_t ioout_stop(ioout_id_t ioout_id)
{
  if(false == ioout_init_flag)
  {
    ioout_config();
  }
  
    if(ioout_id > IOOUT_MAX)
    {
        return IOOUT_ERR_MAX;
    }

    return ioout_stop_io(ioout_id_buf[ioout_id]);
}

/**
  * @brief  启动
  * @param  ioout_id:输出口ID
  *
  * @return ioout_err_t
  * @remark
  */
ioout_err_t ioout_start(ioout_id_t ioout_id)
{
  if(false == ioout_init_flag)
  {
    ioout_config();
  }
  
    if(ioout_id > IOOUT_MAX)
    {
        return IOOUT_ERR_MAX;
    }

    return ioout_start_io(ioout_id_buf[ioout_id]);
}

/**
  * @brief  暂停，数据不清零
  * @param  ioout_id:输出口ID
  *
  * @return ioout_err_t
  * @remark
  */
ioout_err_t ioout_pause(ioout_id_t ioout_id)
{
  if(false == ioout_init_flag)
  {
    ioout_config();
  }
  
    if(ioout_id > IOOUT_MAX)
    {
        return IOOUT_ERR_MAX;
    }

    return ioout_pause_io(ioout_id_buf[ioout_id]);
}

/**
  * @brief  删除
  * @param  ioout_id:输出口ID
  *
  * @return ioout_id
  * @remark 不建议使用
  */
ioout_err_t ioout_kill(ioout_id_t ioout_id)
{
  if(false == ioout_init_flag)
  {
    ioout_config();
  }
  
    int16_t id;

    if(ioout_id > IOOUT_MAX)
    {
        return IOOUT_ERR_MAX;
    }

    id = ioout_id;

    return ioout_kill_io((uint16_t *)(ioout_id_buf + id));
}


/**
  * @brief  IO控制口处理函数
  * @param  无
  *
  * @return 无
  * @remark
  */
void ioout_callback_process(void)
{
    const uint16_t xfer_count = ioout_Time_Interval;
    uint8_t i;

    for(i = 0;i < IOOUT_MAX;i++)
    {
        /*  IO口号未使能  */
        if(m_ioout_list[i].valid_enable && !m_ioout_list[i].enable)
        {
            m_ioout_list[i].handle(false);
            m_ioout_list[i].sumCount     = 0;
            continue;
        }

        /*  IO口号使能  */
        if(m_ioout_list[i].valid_enable && m_ioout_list[i].enable)
        {
            /*  无间隔   */
            if(0 == m_ioout_list[i].interval)
            {

                m_ioout_list[i].handle(true);

                /*  计时  */
                if(m_ioout_list[i].ctlTime)
                {
                    m_ioout_list[i].sumCount += xfer_count;

                    if(m_ioout_list[i].sumCount >= m_ioout_list[i].ctlTime)
                    {
                        m_ioout_list[i].handle(false);
                        m_ioout_list[i].sumCount  = 0;
                        m_ioout_list[i].ioCtl   = false;
                        m_ioout_list[i].enable      = false;

                    }
                }
            }
            /*  有间隔   */
            else
            {
                m_ioout_list[i].sumCount += xfer_count;

                /*  总时间  */
                if(m_ioout_list[i].ctlTime)
                {
                    if(m_ioout_list[i].sumCount >= m_ioout_list[i].ctlTime)
                    {
                        m_ioout_list[i].handle(false);
                        m_ioout_list[i].sumCount = 0;
                        m_ioout_list[i].enable = false;
                        m_ioout_list[i].ioCtl = false;
                        m_ioout_list[i].curCount = 0;
                        continue;
                    }
                }
                else
                {
                  m_ioout_list[i].sumCount=0;
                }

                m_ioout_list[i].curCount += xfer_count;
                m_ioout_list[i].handle(m_ioout_list[i].ioCtl);

                /*  持续时间  */
                if(true == m_ioout_list[i].ioCtl)
                {
                    if(m_ioout_list[i].curCount >= m_ioout_list[i].workTime)
                    {
                        m_ioout_list[i].handle(false);
                        m_ioout_list[i].ioCtl = false;
                        m_ioout_list[i].curCount = 0;
                    }
                }
                /*  间隔时间  */
                else
                {
                    if(m_ioout_list[i].curCount >= m_ioout_list[i].interval)
                    {
                        m_ioout_list[i].handle(true);
                        m_ioout_list[i].ioCtl = true;
                        m_ioout_list[i].curCount = 0;
                    }
                }

            }

        }
    }
}

