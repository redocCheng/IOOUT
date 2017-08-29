/**
  * @file       ioout.c
  * @brief      IO输出控制
  * @author     redoc
  * @version    v1.0
  * @date       2016-08-09
  *
  * @note
  * [2016-08-09] <redoc> v1.0
  * 初始版本
  * [2017-08-16] <redoc> v2.0
  * 1.添加IO配置，和直接调用
  * [2017-08-23] <redoc> v3.0
  * 1.更改依赖，依赖c库文件
  * 2.更改部分函数错误处理
  * [2017-08-29] <redoc> v3.1
  * 1.添加函数指针判断
  * @remark
  */

/* Includes ------------------------------------------------------------------*/
#include "ioout.h"
#include "gpio.h"


/* typedef -----------------------------------------------------------*/
/* define ------------------------------------------------------------*/
/* constants ---------------------------------------------------------*/
const uint16_t ioout_Time_Interval = 10;            /**< 查询间隔    */


/* variables ---------------------------------------------------------*/
static ioOut_Typedef m_nIoOutList[IOOUT_MAX];       /**<  IO管理表   */
static uint16_t iooutIdBuf[IOOUT_MAX] ;             /**<  IOID表   */
static uint8_t iooutInitFlag = false;                  /**<  ioout初始化标志   */

/* functions ---------------------------------------------------------*/
static void ioout_ListInit(void);
static iooutErrCode_Typedef ioout_GetIOIndex(uint16_t *sIndex);
static iooutErrCode_Typedef ioout_SetUnlinkIO(uint16_t index);
static iooutErrCode_Typedef ioout_LinkToIO(uint16_t index,IOOUTCALLBACK handle);
static iooutErrCode_Typedef ioout_SetStartIO(uint16_t index);
static iooutErrCode_Typedef ioout_KillIO(uint16_t *sIndex);
static iooutErrCode_Typedef ioout_SetStopIO(uint16_t index);
static iooutErrCode_Typedef ioout_SetPauseIO(uint16_t index);
static iooutErrCode_Typedef ioout_SetParmIO(uint16_t index,uint32_t interval,uint32_t beeptime,uint32_t ctltime);

/**
  * @brief	配置ioout
  * @param	void
  *
  * @return	void
  * @remark
  */
void ioout_Config(void)
{
    ioout_ListInit();

    /*--------------- 移植配置Start */

    if(IOOUT_NO_ERR != ioout_Init(ID_IOOUT_LED0,GPIO_SetLed0))
    {
    	//do something
    }

    if(IOOUT_NO_ERR != ioout_Init(ID_IOOUT_LED1,GPIO_SetLed1))
    {
        //do something
    }

    /*--------------- 移植配置End */
}

/**
  * @brief	IO管理表初始化
  * @param	无
  *
  * @return	无
  * @remark
  */
static void ioout_ListInit(void)
{
    uint16_t i = 0;

    for(i = 0; i < IOOUT_MAX; i++)
    {
        m_nIoOutList[i].interval        = 0;
        m_nIoOutList[i].workTime        = 0;
        m_nIoOutList[i].ctlTime         = 0;
        m_nIoOutList[i].curCount        = 0;
        m_nIoOutList[i].sumCount        = 0;
        m_nIoOutList[i].handle          = NULL;
        m_nIoOutList[i].enable          = false;
        m_nIoOutList[i].valid_enable    = false;
        m_nIoOutList[i].ioCtl           = false;
        m_nIoOutList[i].index           = 0xFFFF;
        iooutIdBuf[i]                   = 0xFFFF;
    }

    iooutInitFlag = true;
}

/**
  * @brief	链接对应序号IO的操纵函数
  * @param	index：IO口序号
  * @param	handle：链接函数
  *
  * @return	设置状态
  * @remark
  */
static iooutErrCode_Typedef ioout_LinkToIO(uint16_t index,IOOUTCALLBACK handle)
{
    if(index > IOOUT_MAX)
    {
        return IOOUT_ERR_MAX;
    }

    if(handle == NULL)
    {
        return IOOUT_ERR_PARAM;
    }

    m_nIoOutList[index].handle = handle;
    m_nIoOutList[index].valid_enable     = true;

    return IOOUT_NO_ERR;
}

/**
  * @brief	清除序号index
  * @param	index：IO口序号
  *
  * @return	设置状态
  * @remark
  */
static iooutErrCode_Typedef ioout_SetUnlinkIO(uint16_t index)
{
    if(index > IOOUT_MAX)
    {
        return IOOUT_ERR_MAX;
    }


    m_nIoOutList[index].handle(false);
    m_nIoOutList[index].interval 		= 0;
    m_nIoOutList[index].workTime        = 0;
    m_nIoOutList[index].ctlTime 		= 0;
    m_nIoOutList[index].curCount    	= 0;
    m_nIoOutList[index].sumCount   	 	= 0;
    m_nIoOutList[index].handle      	= NULL;
    m_nIoOutList[index].index       	= 0xFFFF;
    m_nIoOutList[index].ioCtl         	= false;
    m_nIoOutList[index].enable      	= false;
    m_nIoOutList[index].valid_enable    = false;

    return IOOUT_NO_ERR;
}

/**
  * @brief	获取控制口的序号
  * @param	*mindex：IO口序号(传出)
  *
  * @return	设置状态
  * @remark
  */
static iooutErrCode_Typedef ioout_GetIOIndex(uint16_t *sIndex)
{
    uint16_t i = 0;

    for(i = 0; i < IOOUT_MAX; i++)
    {
        if(false == m_nIoOutList[i].valid_enable)
        {
            *sIndex = i;
            return IOOUT_NO_ERR;
        }
    }

    return IOOUT_ERR_FAIL;
}

/**
  * @brief	启动和恢复对应序号的IO
  * @param	index：IO口序号
  *
  * @return	设置状态
  * @remark
  */
static iooutErrCode_Typedef ioout_SetStartIO(uint16_t index)
{
    if(index == 0xFFFF)
    {
        return IOOUT_ERR_INIT;
    }

    if(false == m_nIoOutList[index].enable)
    {
        m_nIoOutList[index].enable     = true;
    }

    return IOOUT_NO_ERR;
}


/**
  * @brief	删除对应序号的IO
  * @param	index：IO口序号
  *
  * @return	设置状态
  * @remark	不可恢复，释放资源
  */
static iooutErrCode_Typedef ioout_KillIO(uint16_t *sIndex)
{
    if(*sIndex == 0xFFFF)
    {
        return IOOUT_ERR_INIT;
    }

    ioout_SetUnlinkIO(*sIndex);
    *sIndex = 0xffff;

    return IOOUT_NO_ERR;
}

/**
  * @brief	停止对应序号的IO
  * @param	index：IO口序号
  *
  * @return	设置状态
  * @remark	不释放资源，放弃当前任务，不可恢复
  */
static iooutErrCode_Typedef ioout_SetStopIO(uint16_t index)
{
    if(index == 0xFFFF)
    {
        return IOOUT_ERR_INIT;
    }


    m_nIoOutList[index].handle(false);

    m_nIoOutList[index].interval 		= 0;
    m_nIoOutList[index].workTime 	    = 0;
    m_nIoOutList[index].ctlTime 		= 0;
    m_nIoOutList[index].curCount    	= 0;
    m_nIoOutList[index].sumCount   	 	= 0;
    m_nIoOutList[index].enable      	= false;

    return IOOUT_NO_ERR;
}
/**
  * @brief	暂停对应序号的IO
  * @param	index：IO口序号
  *
  * @return	设置状态
  * @remark	暂停当前任务，恢复继续当前任务
  */
static iooutErrCode_Typedef ioout_SetPauseIO(uint16_t index)
{
    if(index == 0xFFFF)
    {
        return IOOUT_ERR_INIT;
    }

    m_nIoOutList[index].handle(false);
    m_nIoOutList[index].enable     = false;

    return IOOUT_NO_ERR;
}

/**
  * @brief	设置对应序号IO参数
  * @param	index：IO口序号
  * @param	interval：间隔时间
  * @param	worktime：持续时间
  * @param	ctltime：总时间
  *
  * @return	iooutErrCode_Typedef
  * @remark ms为单位,参数必须是查询时间的整数倍，此函数不允许worktime为零
  */
static iooutErrCode_Typedef ioout_SetParmIO(uint16_t index,uint32_t interval,uint32_t workTime,uint32_t ctltime)
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

    m_nIoOutList[index].interval 		= interval;
    m_nIoOutList[index].workTime	    = workTime;
    m_nIoOutList[index].ctlTime			= ctltime;
    m_nIoOutList[index].sumCount    	= 0;
    m_nIoOutList[index].enable       	= true;

    return IOOUT_NO_ERR;
}

/**
  * @brief	初始化IOOUT使用接口函数
  * @param	timproc：IO口接口函数
  *
  * @return	IO口号
  * @remark
  */
iooutErrCode_Typedef ioout_Init(iooutId_Typedef iooutId,IOOUTCALLBACK timproc)
{
    if(false == iooutInitFlag)
    {
        return IOOUT_ERR_INIT;
    }

    if(iooutId > IOOUT_MAX)
    {
        return IOOUT_ERR_MAX;
    }

    if(timproc == NULL)
    {
        return IOOUT_ERR_PARAM;
    }

    if(IOOUT_NO_ERR != ioout_GetIOIndex((uint16_t *)(iooutIdBuf + (uint8_t)iooutId)))
    {
        return IOOUT_ERR_FAIL;
    }

    if(IOOUT_NO_ERR != ioout_LinkToIO(iooutIdBuf[iooutId],timproc))
    {
        return IOOUT_ERR_FAIL;
    }

    return IOOUT_NO_ERR;
}

/**
  * @brief	设置IO并启动
  * @param	iooutId:输出口ID
  * @param	interval：间隔时间
  * @param	worktime：持续时间
  * @param	ctltime：总时间
  *
  * @return	iooutErrCode_Typedef
  * @remark 时间以ms为单位,参数必须是查询时间的整数倍，worktime为0停止，ctltime为0一直保持周期
  */
iooutErrCode_Typedef ioout_Set(iooutId_Typedef iooutId,uint32_t interval,uint32_t workTime,uint32_t ctlTime)
{
    iooutErrCode_Typedef status;

    if(iooutId > IOOUT_MAX)
    {
        return IOOUT_ERR_MAX;
    }

    if(workTime != 0)
    {
        status == ioout_SetParmIO(iooutIdBuf[iooutId],interval,workTime,ctlTime);

        if(IOOUT_NO_ERR == status)
        {
            ioout_SetStartIO(iooutIdBuf[iooutId]);
        }
        else
        {
            return status;
        }
    }
    else
    {
        ioout_SetStopIO(iooutIdBuf[iooutId]);
    }

    return IOOUT_NO_ERR;
}

/**
  * @brief	停止,数据清零
  * @param	iooutId:输出口ID
  *
  * @return	iooutErrCode_Typedef
  * @remark
  */
iooutErrCode_Typedef ioout_Stop(iooutId_Typedef iooutId)
{
    if(iooutId > IOOUT_MAX)
    {
        return IOOUT_ERR_MAX;
    }

    return ioout_SetStopIO(iooutIdBuf[iooutId]);
}

/**
  * @brief	启动
  * @param	iooutId:输出口ID
  *
  * @return	iooutErrCode_Typedef
  * @remark
  */
iooutErrCode_Typedef ioout_Start(iooutId_Typedef iooutId)
{
    if(iooutId > IOOUT_MAX)
    {
        return IOOUT_ERR_MAX;
    }

    return ioout_SetStartIO(iooutIdBuf[iooutId]);
}

/**
  * @brief	暂停，数据不清零
  * @param	iooutId:输出口ID
  *
  * @return	iooutErrCode_Typedef
  * @remark
  */
iooutErrCode_Typedef ioout_Pause(iooutId_Typedef iooutId)
{
    if(iooutId > IOOUT_MAX)
    {
        return IOOUT_ERR_MAX;
    }

    return ioout_SetPauseIO(iooutIdBuf[iooutId]);
}

/**
  * @brief	删除
  * @param	iooutId:输出口ID
  *
  * @return	iooutId
  * @remark 不建议使用
  */
iooutErrCode_Typedef ioout_Kill(iooutId_Typedef iooutId)
{
    int16_t id;

    if(iooutId > IOOUT_MAX)
    {
        return IOOUT_ERR_MAX;
    }

    id = iooutId;

    return ioout_KillIO((uint16_t *)(iooutIdBuf + id));
}


/**
  * @brief	IO控制口处理函数
  * @param	无
  *
  * @return	无
  * @remark
  */
void ioout_CallBackProcRoutine(void)
{
    const uint16_t xfer_count = ioout_Time_Interval;
    uint8_t i;

    for(i = 0;i < IOOUT_MAX;i++)
    {
        /*  IO口号未使能  */
        if(m_nIoOutList[i].valid_enable && !m_nIoOutList[i].enable)
        {
            m_nIoOutList[i].handle(false);
            m_nIoOutList[i].sumCount     = 0;
            continue;
        }

        /*  IO口号使能  */
        if(m_nIoOutList[i].valid_enable && m_nIoOutList[i].enable)
        {
            /*  无间隔   */
            if(0 == m_nIoOutList[i].interval)
            {

                m_nIoOutList[i].handle(true);

                /*  计时  */
                if(m_nIoOutList[i].ctlTime)
                {
                    m_nIoOutList[i].sumCount += xfer_count;

                    if(m_nIoOutList[i].sumCount >= m_nIoOutList[i].ctlTime)
                    {
                        m_nIoOutList[i].handle(false);
                        m_nIoOutList[i].sumCount	= 0;
                        m_nIoOutList[i].ioCtl		= false;
                        m_nIoOutList[i].enable      = false;

                    }
                }
            }
            /*  有间隔   */
            else
            {
                m_nIoOutList[i].sumCount += xfer_count;

                /*  总时间  */
                if(m_nIoOutList[i].ctlTime)
                {
                    if(m_nIoOutList[i].sumCount >= m_nIoOutList[i].ctlTime)
                    {
                        m_nIoOutList[i].handle(false);
                        m_nIoOutList[i].sumCount = 0;
                        m_nIoOutList[i].enable = false;
                        m_nIoOutList[i].ioCtl = false;
                        m_nIoOutList[i].curCount = 0;
                        continue;
                    }
                }
                else
                {
                	m_nIoOutList[i].sumCount=0;
                }

                m_nIoOutList[i].curCount += xfer_count;
                m_nIoOutList[i].handle(m_nIoOutList[i].ioCtl);

                /*  持续时间  */
                if(true == m_nIoOutList[i].ioCtl)
                {
                    if(m_nIoOutList[i].curCount >= m_nIoOutList[i].workTime)
                    {
                        m_nIoOutList[i].handle(false);
                        m_nIoOutList[i].ioCtl = false;
                        m_nIoOutList[i].curCount = 0;
                    }
                }
                /*  间隔时间  */
                else
                {
                    if(m_nIoOutList[i].curCount >= m_nIoOutList[i].interval)
                    {
                        m_nIoOutList[i].handle(true);
                        m_nIoOutList[i].ioCtl = true;
                        m_nIoOutList[i].curCount = 0;
                    }
                }

            }

        }
    }
}



