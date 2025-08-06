/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2021-08-25     AisinoChip        First Version
 */

#include <board.h>
#include <rtthread.h>
#include <rtdevice.h>


#ifdef RT_USING_WDT
#include "board.h"

struct acm32_wdt_obj
{
    union
    {
        WDT_HandleTypeDef       wdt;    //窗口看门狗句柄
        IWDT_HandleTypeDef      iwdt;   //独立看门狗句柄
    } handle;
    rt_uint16_t             is_start;   //看门狗是否启动标志
    rt_uint16_t             type;       //看门狗类型（窗口或独立）
    rt_watchdog_t           watchdog;   //RT_Thread看门狗设备结构，位置：component/drivers
};

#define TYPE_WDT         0
#define TYPE_IWDT        1
#define IWDT_FREQ        (32000)

#ifdef BSP_USING_WDT
    #define WDT_NAME        "wdt"
    static struct acm32_wdt_obj acm32_wdt;
#endif

#ifdef BSP_USING_IWDT
    #define IWDT_NAME       "iwdt"
    static struct acm32_wdt_obj acm32_iwdt;
#endif

static struct rt_watchdog_ops ops;
//通过适当选择分频系数，使得看门狗定时器能够在不溢出的情况下实现期望的超时时间
rt_inline rt_base_t calc_wdt_divisor_load(rt_uint32_t freq, rt_uint32_t sec, rt_uint32_t *divisor, rt_uint32_t *load)
{
    rt_uint32_t freqMaxSec = 0;
    rt_uint32_t minFreqDiv = WDT_DIVISOR_NONE;

    freqMaxSec = RT_UINT32_MAX / freq;

    while (minFreqDiv <= WDT_DIVISOR_128)
    {
        if (sec < freqMaxSec)
        {
            break;
        }
        minFreqDiv ++;
        freqMaxSec = RT_UINT32_MAX / freq * (1 << minFreqDiv);
    }

    if (minFreqDiv > WDT_DIVISOR_128)
    {
        return -1;
    }

    *divisor = minFreqDiv;
    *load = sec * (freq >> minFreqDiv);
    return 0;
}

rt_inline rt_base_t calc_iwdt_divisor_load(rt_uint32_t freq, rt_uint32_t sec, rt_uint32_t *divisor, rt_uint32_t *load)
{
    rt_uint32_t minFreqDiv = IWDT_CLOCK_PRESCALER_4;
    rt_uint32_t freqMaxMs = 0;

    freqMaxMs = IWDT_RELOAD_MAX_VALUE * 1000 / (freq >> (2 + minFreqDiv));

    while (minFreqDiv <= IWDT_CLOCK_PRESCALER_256)
    {
        if (sec * 1000 < freqMaxMs)
        {
            break;
        }
        minFreqDiv ++;
        freqMaxMs = IWDT_RELOAD_MAX_VALUE * 1000 / (freq >> (2 + minFreqDiv));
    }

    if (minFreqDiv > IWDT_CLOCK_PRESCALER_256)
    {
        return -1;
    }

    *divisor = minFreqDiv;
    if (sec < 1000)
    {
        *load = (sec * 1000) * IWDT_RELOAD_MAX_VALUE / freqMaxMs;
    }
    else
    {
        *load = (sec) * IWDT_RELOAD_MAX_VALUE / freqMaxMs / 1000;
    }
    return 0;
}

rt_inline rt_uint32_t calc_wdt_timeout(rt_uint32_t freq, rt_uint32_t divisor, rt_uint32_t count)
{
    /* 1 / ( freq / (1<<divisor) ) * (count) */
    return (rt_uint32_t)(((rt_uint64_t)count) * (1 << divisor) / (freq));
}

rt_inline rt_uint32_t calc_iwdt_timeout(rt_uint32_t freq, rt_uint32_t divisor, rt_uint32_t count)
{
    /* (freq >> (2+divisor)) / IWDT_RELOAD_MAX_VALUE * count */
    return count / (freq >> (2 + divisor));
}

static rt_err_t wdt_init(rt_watchdog_t *wdt)
{
    return RT_EOK;
}

static rt_err_t wdt_control(rt_watchdog_t *wdt, int cmd, void *arg)
{
    struct acm32_wdt_obj *wdtObj = NULL;
    rt_uint32_t timer_clk_hz;
    rt_uint32_t divisor, load;

    RT_ASSERT(wdt != RT_NULL);

    wdtObj = rt_container_of(wdt, struct acm32_wdt_obj, watchdog);//获取看门狗对象
    timer_clk_hz = System_Get_APBClock();//获取系统时钟频率
    /* acm_wdg funciton branch*/
    switch (cmd)
    {
    /* feed the watchdog */
    case RT_DEVICE_CTRL_WDT_KEEPALIVE://"喂狗"操作，重置看门狗计数器，防止系统复位
        if (TYPE_WDT == wdtObj->type)
        {
            HAL_WDT_Feed(&wdtObj->handle.wdt);
        }
        else
        {
            HAL_IWDT_Kick_Watchdog_Wait_For_Done(&wdtObj->handle.iwdt);
        }
        break;
    /* set watchdog timeout, seconds */
    case RT_DEVICE_CTRL_WDT_SET_TIMEOUT:
        if (TYPE_WDT == wdtObj->type)
        {
            if (calc_wdt_divisor_load(timer_clk_hz, (*((rt_uint32_t *)arg)), &divisor, &load))
            {
                return -RT_ERROR;
            }
            wdtObj->handle.wdt.Init.WDTDivisor = (WDT_DIVISOR)divisor;
            wdtObj->handle.wdt.Init.WDTLoad = load;
            HAL_WDT_Init(&wdtObj->handle.wdt);
        }
        else
        {
            if (calc_iwdt_divisor_load(IWDT_FREQ, (*((rt_uint32_t *)arg)), &divisor, &load))
            {
                return -RT_ERROR;
            }
            wdtObj->handle.iwdt.Instance = IWDT;
            wdtObj->handle.iwdt.Init.Prescaler = divisor;
            wdtObj->handle.iwdt.Init.Reload = load;
        }

        if (wdtObj->is_start)
        {
            if (TYPE_WDT == wdtObj->type)
            {
                HAL_WDT_Init(&wdtObj->handle.wdt);
            }
            else
            {
                HAL_IWDT_Init(&wdtObj->handle.iwdt);
            }
        }
        break;
    case RT_DEVICE_CTRL_WDT_GET_TIMELEFT:
        if (TYPE_WDT == wdtObj->type)
        {
            (*((rt_uint32_t *)arg)) = calc_wdt_timeout(timer_clk_hz,
                                      wdtObj->handle.wdt.Init.WDTDivisor,
                                      wdtObj->handle.wdt.Instance->COUNT);
        }
        else
        {
            return -RT_EINVAL;
        }
        break;
    case RT_DEVICE_CTRL_WDT_GET_TIMEOUT:
        if (TYPE_WDT == wdtObj->type)
        {
            (*((rt_uint32_t *)arg)) = calc_wdt_timeout(timer_clk_hz,
                                      wdtObj->handle.wdt.Init.WDTDivisor,
                                      wdtObj->handle.wdt.Init.WDTLoad);
        }
        else
        {
            (*((rt_uint32_t *)arg)) = calc_iwdt_timeout(IWDT_FREQ,
                                      wdtObj->handle.iwdt.Init.Prescaler,
                                      wdtObj->handle.iwdt.Init.Reload);
        }
        break;
    case RT_DEVICE_CTRL_WDT_START://启动开门狗
        if (TYPE_WDT == wdtObj->type)
        {
            wdtObj->handle.wdt.Instance = WDT;
            wdtObj->handle.wdt.Init.WDTMode = WDT_MODE_RST;
            wdtObj->handle.wdt.Init.WDTINTCLRTIME = 0xffff;
            HAL_WDT_Init(&wdtObj->handle.wdt);
            HAL_WDT_Start(&wdtObj->handle.wdt);
        }
        else
        {
            wdtObj->handle.iwdt.Instance->CMDR = IWDT_ENABLE_COMMAND;
            wdtObj->handle.iwdt.Init.Window = IWDT_RELOAD_MAX_VALUE;  /* window function disabled when window >= reload */
            wdtObj->handle.iwdt.Init.Wakeup = IWDT_RELOAD_MAX_VALUE;  /* wakeup function disabled when wakeup >= reload */
            HAL_IWDT_Init(&wdtObj->handle.iwdt);
        }
        wdtObj->is_start = 1;
        break;
    case RT_DEVICE_CTRL_WDT_STOP:
        if (TYPE_WDT == wdtObj->type)
        {
            HAL_WDT_Stop(&wdtObj->handle.wdt);
        }
        else
        {
            wdtObj->handle.iwdt.Instance->CMDR = IWDT_DISABLE_COMMAND;
        }
        wdtObj->is_start = 0;
        break;
    default:
        return -RT_ERROR;
    }
    return RT_EOK;
}

int rt_wdt_init(void)
{   //MOTE: 注册fops
    ops.init = &wdt_init;
    ops.control = &wdt_control;
    //设备注册
#ifdef BSP_USING_WDT
    acm32_wdt.type = TYPE_WDT;
    acm32_wdt.is_start = 0;
    acm32_wdt.watchdog.ops = &ops;
    if (rt_hw_watchdog_register(&acm32_wdt.watchdog, WDT_NAME, RT_DEVICE_FLAG_DEACTIVATE, RT_NULL) != RT_EOK)
    {
        return -RT_ERROR;
    }
#endif
#ifdef BSP_USING_IWDT
    acm32_iwdt.type = TYPE_IWDT;
    acm32_iwdt.is_start = 0;
    acm32_iwdt.watchdog.ops = &ops;
    if (rt_hw_watchdog_register(&acm32_iwdt.watchdog, IWDT_NAME, RT_DEVICE_FLAG_DEACTIVATE, RT_NULL) != RT_EOK)
    {
        return -RT_ERROR;
    }
#endif

    return RT_EOK;
}
INIT_BOARD_EXPORT(rt_wdt_init);//作用是将 rt_wdt_init 函数注册为板级初始化函数，使其在系统启动时自动被调用。

#endif /* RT_USING_WDT */

/*
    @Breif: 解释 RT设备驱动框架
#define INIT_BOARD_EXPORT(fn)  INIT_EXPORT(fn, "1")

级别"0"：系统最开始初始化
级别"1"：板级初始化（BOARD_EXPORT）
级别"2"：设备初始化（DEVICE_EXPORT）
级别"3"：组件初始化（COMPONENT_EXPORT）
级别"4"：文件系统初始化（FS_EXPORT）
级别"5"：环境变量初始化（ENV_EXPORT）
级别"6"：应用初始化（APP_EXPORT）

实际实现： 根据编译器和配置不同，INIT_EXPORT 宏会将函数指针放置在特定的段（section）中，例如：

rt_used const init_fn_t __rt_init_##fn rt_section(".rti_fn." level) = fn
这样在链接时，所有标记为同一级别的函数会被放在连续的内存区域中。

自动调用： 系统启动时，在 rt_components_board_init() 函数中会遍历级别为"1"的所有函数并依次调用它们。


*/
