# IOOUT
[![Badge](https://img.shields.io/badge/link-996.icu-red.svg)](https://996.icu/#/zh_CN)

## 1.介绍
[IOOUT](https://github.com/redocCheng/IOOUT)  是一个 GPIO 输出口控制扩展模块，提供 **启动** 、 **暂停** 、**设置** 和 **停止** 接口，可以根据你所需的周期、占空比和总时间，更加便捷地管理多个IO口时序。

## 1.1 文件结构

| 源文件               |描述|
| :--------           |:--------  |
| \ioout              | IOOUT操作接口和实现源码 | 
| \demo               | 使用范例 | 

## 1.2 资源占用

**单片机资源**： 定时一个

**RAM**: 视IO口多少

## 2. 移植


### 2.1 定时调用
需要用户设置 IOOUT_BASE_TIME，初始化一个定时器，并且定时周期和 IOOUT_BASE_TIME  保持一致

#define IOOUT_BASE_TIME  10


在定时器中断中，调用接口函数
```
ioout_loop();	   

```

### 2.2 配置内存管理

选择是否使用内存管理，并配置内存管理接口函数

```	
#define IOOUT_USE_MEM
```	

```
#ifdef IOOUT_USE_MEM

void *ioout_malloc(uint32_t nbytes)
{
    //add malloc
}

void ioout_free (void *ptr)
{
    //add free
}

#endif
```


### 2.3 配置优先输出的电平

默认优先输出work_time,定义后即可先输出interval

```	
#define IOOUT_LOW_PULSE_PRIORITY    

```


## 3. API
API接口在 [*/ioout/ioout.h*](https://github.com/redocCheng/IOOUT/tree/master/ioout/ioout.h) 声明，下方内容可以使用 CTRL+F 搜索。



### 3.1 端口函数初始化
查找空闲端口，并初始化该端口，初始化成返回 0 ，其中，IO口接口函数需要遵循 *void function(uint8_t)* 格式。

使用IOOUT_USE_MEM会申请内存，只需创建一个句柄指针即可，反之，需要创建句柄实体。
>int ioout_init(ioout_t handle, void(*ioout_cb)(uint8_t));


| 参数                |描述|
| :--------           |:--------  |
| handle              | 句柄 | 
| ioout_cb            | IO口接口函数 | 

示例：
```
if(-1 == ioout_init(ioout_beep,gpio_set_beep))
{

}
```

### 3.2 删除
将端口删除，删除成功返回 0 。
使用IOOUT_USE_MEM会释放内存。
>int ioout_kill(ioout_t handle)

| 参数                |描述|
| :--------           |:--------  |
| iooutId             | 句柄 | 


### 3.3 时间参数设置
设置端口时间并启动，时间参数以 ms 为单位，参数必须是查询 IOOUT_BASE_TIME  时间的整数倍，ctrl_time 为 0 一直保持周期，设置成功返回 0 。

>int ioout_set(ioout_t handle, uint32_t interval, uint32_t work_time, uint32_t ctrl_time);

| 参数                |描述|
| :--------           |:--------  |
| handle             | 句柄 | 
| interval            | 间隔时间 | 
| work_time            | 持续时间 | 
| ctrl_time             | 总时间 | 

示例：

见demo



## 4. DEMO

[DEMO](https://github.com/redocCheng/IOOUT/tree/master/demo) 。

## 5. 其它
If you have any question,Please contact  redoc/619675912@qq.com
