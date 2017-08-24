# IOOUT


## 1.介绍
[IOOUT](https://github.com/redocCheng/IOOUT)  是一个 GPIO 输出口控制扩展模块，提供 **启动** 、 **暂停** 、**设置** 和 **停止** 接口，可以根据你所需的周期、占空比和总时间，更加便捷地管理多个IO口时序。


## 2. 移植
### 2.1 端口配置
- **配置控制ID**
```	
/**
  * @brief IO输出控制ID
  */
typedef enum
{
    ID_IOOUT_LED0 = 0,
	ID_IOOUT_LED1,

} iooutId_Typedef;
```

- **配置控制数量**
```	
/**
  * @brief IO输出控制数量类型定义
  */
typedef enum
{
    IOOUT_MAX = (ID_IOOUT_LED1 + 1),

} ioOutIndexNum_Typedef;

```

- **初始化**
初始化 IOOUT 管理结构体，端口操作函数函数需要遵循 **void function(uint8_t)** 格式，
 
```
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
```

- **定时器调用**
需要用户设置一个定时器,并且定时器周期和 ioout_Time_Interval  保持一致
```
const uint16_t ioout_Time_Interval = 10;   /**< 查询间隔    */

void ioout_CallBackProcRoutine(void);	   /**< IO控制口处理函数  */
```

## 3. API


## 4.DEMO



