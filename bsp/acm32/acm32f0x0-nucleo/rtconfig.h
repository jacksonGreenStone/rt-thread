#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

/* RT-Thread Kernel */

/* klibc options */

/* rt_vsnprintf options */

/* end of rt_vsnprintf options */

/* rt_vsscanf options */

/* end of rt_vsscanf options */

/* rt_memset options */

/* end of rt_memset options */

/* rt_memcpy options */

/* end of rt_memcpy options */

/* rt_memmove options */

/* end of rt_memmove options */

/* rt_memcmp options */

/* end of rt_memcmp options */

/* rt_strstr options */

/* end of rt_strstr options */

/* rt_strcasecmp options */

/* end of rt_strcasecmp options */

/* rt_strncpy options */

/* end of rt_strncpy options */

/* rt_strcpy options */

/* end of rt_strcpy options */

/* rt_strncmp options */

/* end of rt_strncmp options */

/* rt_strcmp options */

/* end of rt_strcmp options */

/* rt_strlen options */

/* end of rt_strlen options */

/* rt_strnlen options */

/* end of rt_strnlen options */
/* end of klibc options */
//RT_Thread： RT系统设置
/*
定义内核对象名称的最大长度为8个字符
RT-Thread中的线程、信号量、消息队列等内核对象都有名称标识
限制名称长度有助于节省内存
*/
#define RT_NAME_MAX 8
/*
义系统中CPU核心的数量为1
表示这是一个单核系统
对于多核系统，该值会大于1，并启用相应的多核支持代码
*/
#define RT_CPUS_NR 1
/*
定义内存对齐大小为8字节
确保数据结构在内存中的对齐，提高访问效率
对于32位系统通常为4字节，64位系统通常为8字节
*/
#define RT_ALIGN_SIZE 8
/*
定义系统支持32级线程优先级
这是一个标识性定义，表明系统配置为32级优先级
*/
#define RT_THREAD_PRIORITY_32
/*
定义线程优先级的最大级别数为32
优先级数值从0到31，数值越小优先级越高
0为最高优先级，31为最低优先级
*/
#define RT_THREAD_PRIORITY_MAX 32
/*
定义系统每秒的时钟节拍数为100
即系统时钟节拍间隔为10ms（1/100秒）---- 一般我们设置为 1000； 即 1tick = 1ms
这个值影响系统的定时精度和任务调度频率
*/
#define RT_TICK_PER_SECOND 100
/*
启用堆栈溢出检查功能
系统会检测线程堆栈是否发生溢出
有助于发现和调试堆栈相关问题
*/
#define RT_USING_OVERFLOW_CHECK
/*
使用函数指针方式实现钩子函数
通过函数指针调用用户定义的钩子函数--- 钩子函数/回调函数的作用，事件处理，以及统计处理
*/
#define RT_USING_HOOK
/*
启用系统钩子函数功能
允许用户注册回调函数来监控系统关键事件
*/
#define RT_HOOK_USING_FUNC_PTR
/*
启用空闲任务钩子函数功能
允许用户在空闲任务中执行自定义代码
*/
#define RT_USING_IDLE_HOOK
/*
定义空闲任务钩子函数列表的大小为4
最多可以注册4个空闲任务钩子函数
*/
#define RT_IDLE_HOOK_LIST_SIZE 4
/*
设置空闲线程的堆栈大小为256字节
空闲线程是系统最低优先级的线程，在没有其他任务运行时执行
*/
#define IDLE_THREAD_STACK_SIZE 256
/*
启用软件定时器功能 ---- 软件定时器：用于处理一些 软件逻辑时序、及机制处理，用于实时性的设计
软件定时器在专门的线程中处理，不会在中断上下文中执行
*/
#define RT_USING_TIMER_SOFT
/*
设置软件定时器线程的优先级为4 ---- 高优先级，保证了实时性的准确性
相对较高的优先级，确保定时器能及时处理
*/
#define RT_TIMER_THREAD_PRIO 4
/*
设置软件定时器线程的堆栈大小为512字节
为定时器回调函数的执行提供足够的堆栈空间
*/
#define RT_TIMER_THREAD_STACK_SIZE 512

/* kservice options */

/* end of kservice options */
#define RT_USING_DEBUG
#define RT_DEBUGING_ASSERT
#define RT_DEBUGING_COLOR
#define RT_DEBUGING_CONTEXT

/* Inter-Thread communication */
//RT_Thread: 配置RT的内部通信管理机制
/*
启用信号量（Semaphore）功能
信号量是一种用于控制多个线程对共享资源访问的同步机制
主要用于实现资源的计数和线程间的同步
典型应用场景：控制同时访问某个资源的线程数量----- 如何设计以及使用
*/
#define RT_USING_SEMAPHORE
/*
启用互斥量（Mutex）功能
互斥量是一种特殊的二值信号量，用于保护临界区
提供互斥访问机制，确保同一时间只有一个线程能访问共享资源
支持优先级继承机制，防止优先级反转问题
*/
#define RT_USING_MUTEX
/*
启用事件组（Event）功能
事件组允许线程等待一个或多个事件的发生
通过事件标志位的组合实现复杂的同步条件
支持"与"和"或"逻辑操作，可以等待多个事件的组合
*/
#define RT_USING_EVENT
/*
启用邮箱（Mailbox）功能
邮箱是一种轻量级的线程间通信机制
用于在线程间传递固定大小的消息（通常是4字节的指针或整数）
消息传递采用FIFO（先进先出）方式
*/
#define RT_USING_MAILBOX
/*
启用消息队列（Message Queue）功能
消息队列是一种更灵活的线程间通信机制
支持传递可变长度的消息
可以存储多个消息，具有缓冲能力
消息传递同样采用FIFO方式
*/
#define RT_USING_MESSAGEQUEUE
/* end of Inter-Thread communication */

/* Memory Management */
//RT_Thread: 配置RT的内存管理机制
#define RT_USING_MEMPOOL
#define RT_USING_SMALL_MEM
#define RT_USING_SMALL_MEM_AS_HEAP
#define RT_USING_HEAP
/* end of Memory Management */
#define RT_USING_DEVICE
#define RT_USING_DEVICE_OPS
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 128
#define RT_CONSOLE_DEVICE_NAME "uart1"
#define RT_VER_NUM 0x50201
#define RT_BACKTRACE_LEVEL_MAX_NR 32
/* end of RT-Thread Kernel */
#define ARCH_ARM
#define ARCH_ARM_CORTEX_M
#define ARCH_ARM_CORTEX_M0

/* RT-Thread Components */
//RT_Thread： 系统组件配置
/*
*启用RT-Thread组件自动初始化功能
*允许系统在启动时自动初始化各个组件模块
*通过RTI（RunTime Instrumentation）机制实现组件的有序初始化
*/
#define RT_USING_COMPONENTS_INIT
/*
启用用户主函数功能
允许用户定义main函数作为应用程序入口点
系统会在RT-Thread内核初始化完成后调用用户定义的main函数
*/
#define RT_USING_USER_MAIN
/*
设置主线程（main thread）的堆栈大小为2048字节
主线程是运行用户main函数的线程
堆栈大小需要根据main函数中的局部变量使用情况来合理设置
*/
#define RT_MAIN_THREAD_STACK_SIZE 2048
/*
设置主线程的优先级为10
RT-Thread中线程优先级数值越小优先级越高
优先级10属于中等优先级，确保主线程能及时运行但不会抢占关键的高优先级任务
*/
#define RT_MAIN_THREAD_PRIORITY 10
/*
启用MSH（Mini Shell）命令行功能
MSH是RT-Thread提供的增强型命令行接口，支持类似Linux的命令语法
*/
#define RT_USING_MSH
/*
启用FinSH（Fin Shell）命令行功能
FinSH是RT-Thread传统的命令行接口
通常与MSH配合使用，MSH提供了更多功能
*/
#define RT_USING_FINSH
/*
在FinSH中启用MSH功能
使FinSH可以使用MSH提供的命令和功能
*/
#define FINSH_USING_MSH
#define FINSH_THREAD_NAME "tshell"
#define FINSH_THREAD_PRIORITY 20
#define FINSH_THREAD_STACK_SIZE 4096
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES 5
#define FINSH_USING_SYMTAB
#define FINSH_CMD_SIZE 80
#define MSH_USING_BUILT_IN_COMMANDS
#define FINSH_USING_DESCRIPTION
#define FINSH_ARG_MAX 10
#define FINSH_USING_OPTION_COMPLETION

/* DFS: device virtual file system */
//RT_Thread：是否需要配置支持 DFS
/* end of DFS: device virtual file system */

/* Device Drivers */
//RT_Thread： 设备驱动框架---驱动选择
#define RT_USING_DEVICE_IPC
#define RT_UNAMED_PIPE_NUMBER 64
#define RT_USING_SERIAL
#define RT_USING_SERIAL_V1
#define RT_SERIAL_USING_DMA
#define RT_SERIAL_RB_BUFSZ 64
#define RT_USING_ADC
#define RT_USING_WDT
#define RT_USING_PIN
#define RT_USING_HWTIMER
/* end of Device Drivers */

/* C/C++ and POSIX layer */

/* ISO-ANSI C layer */

/* Timezone and Daylight Saving Time */

#define RT_LIBC_USING_LIGHT_TZ_DST
#define RT_LIBC_TZ_DEFAULT_HOUR 8
#define RT_LIBC_TZ_DEFAULT_MIN 0
#define RT_LIBC_TZ_DEFAULT_SEC 0
/* end of Timezone and Daylight Saving Time */
/* end of ISO-ANSI C layer */

/* POSIX (Portable Operating System Interface) layer */


/* Interprocess Communication (IPC) */


/* Socket is in the 'Network' category */

/* end of Interprocess Communication (IPC) */
/* end of POSIX (Portable Operating System Interface) layer */
/* end of C/C++ and POSIX layer */

/* Network */

/* end of Network */

/* Memory protection */

/* end of Memory protection */

/* Utilities */

/* end of Utilities */

/* Using USB legacy version */

/* end of Using USB legacy version */
/* end of RT-Thread Components */

/* RT-Thread Utestcases */

/* end of RT-Thread Utestcases */

/* RT-Thread online packages */

/* IoT - internet of things */


/* Wi-Fi */

/* Marvell WiFi */

/* end of Marvell WiFi */

/* Wiced WiFi */

/* end of Wiced WiFi */

/* CYW43012 WiFi */

/* end of CYW43012 WiFi */

/* BL808 WiFi */

/* end of BL808 WiFi */

/* CYW43439 WiFi */

/* end of CYW43439 WiFi */
/* end of Wi-Fi */

/* IoT Cloud */

/* end of IoT Cloud */
/* end of IoT - internet of things */

/* security packages */

/* end of security packages */

/* language packages */

/* JSON: JavaScript Object Notation, a lightweight data-interchange format */

/* end of JSON: JavaScript Object Notation, a lightweight data-interchange format */

/* XML: Extensible Markup Language */

/* end of XML: Extensible Markup Language */
/* end of language packages */

/* multimedia packages */

/* LVGL: powerful and easy-to-use embedded GUI library */

/* end of LVGL: powerful and easy-to-use embedded GUI library */

/* u8g2: a monochrome graphic library */

/* end of u8g2: a monochrome graphic library */
/* end of multimedia packages */

/* tools packages */

/* end of tools packages */

/* system packages */

/* enhanced kernel services */

/* end of enhanced kernel services */

/* acceleration: Assembly language or algorithmic acceleration packages */

/* end of acceleration: Assembly language or algorithmic acceleration packages */

/* CMSIS: ARM Cortex-M Microcontroller Software Interface Standard */

/* end of CMSIS: ARM Cortex-M Microcontroller Software Interface Standard */

/* Micrium: Micrium software products porting for RT-Thread */

/* end of Micrium: Micrium software products porting for RT-Thread */
/* end of system packages */

/* peripheral libraries and drivers */

/* HAL & SDK Drivers */

/* STM32 HAL & SDK Drivers */

/* end of STM32 HAL & SDK Drivers */

/* Infineon HAL Packages */

/* end of Infineon HAL Packages */

/* Kendryte SDK */

/* end of Kendryte SDK */

/* WCH HAL & SDK Drivers */

/* end of WCH HAL & SDK Drivers */

/* AT32 HAL & SDK Drivers */

/* end of AT32 HAL & SDK Drivers */

/* HC32 DDL Drivers */

/* end of HC32 DDL Drivers */

/* NXP HAL & SDK Drivers */

/* end of NXP HAL & SDK Drivers */
/* end of HAL & SDK Drivers */

/* sensors drivers */

/* end of sensors drivers */

/* touch drivers */

/* end of touch drivers */
/* end of peripheral libraries and drivers */

/* AI packages */

/* end of AI packages */

/* Signal Processing and Control Algorithm Packages */

/* end of Signal Processing and Control Algorithm Packages */

/* miscellaneous packages */

/* project laboratory */

/* end of project laboratory */

/* samples: kernel and components samples */

/* end of samples: kernel and components samples */

/* entertainment: terminal games and other interesting software packages */

/* end of entertainment: terminal games and other interesting software packages */
/* end of miscellaneous packages */

/* Arduino libraries */


/* Projects and Demos */

/* end of Projects and Demos */

/* Sensors */

/* end of Sensors */

/* Display */

/* end of Display */

/* Timing */

/* end of Timing */

/* Data Processing */

/* end of Data Processing */

/* Data Storage */

/* Communication */

/* end of Communication */

/* Device Control */

/* end of Device Control */

/* Other */

/* end of Other */

/* Signal IO */

/* end of Signal IO */

/* Uncategorized */

/* end of Arduino libraries */
/* end of RT-Thread online packages */
#define SOC_SERIES_ACM32F0

/* Hardware Drivers Config */
//BSP配置：配置特殊功能外设芯片的资源范围
#define SOC_ACM32F070RBT7
#define SOC_SRAM_START_ADDR 0x20000000
#define SOC_SRAM_SIZE 0x20
#define SOC_FLASH_START_ADDR 0x00000000
#define SOC_FLASH_SIZE 0x80

/* Onboard Peripheral Drivers */
//BSP配置：板上外设驱动
/* On-chip Peripheral Drivers */
//BSP配置：片内外设驱动开启
/* Hardware GPIO */
//BSP配置： 使用GPIO外设;仅仅使用IO功能

#define BSP_USING_GPIO1
#define BSP_USING_GPIO2
/* end of Hardware GPIO */
#define BSP_USING_ADC

/* Hardware UART */
//BSP配置： 使用UART外设

#define BSP_USING_UART1
#define BSP_USING_UART2
#define BSP_UART2_RX_USING_DMA
#define BSP_UART2_TX_USING_DMA
#define BSP_USING_UART3
#define BSP_UART3_RX_USING_DMA
#define BSP_UART3_TX_USING_DMA
/* end of Hardware UART */

/* Hardware I2C */
//BSP配置： 使用i2c外设

/* end of Hardware I2C */

/* Hardware CAN */
//BSP配置： 使用CAN外设

/* end of Hardware CAN */

/* Hardware TIMER */
//BSP配置： 使用定时器外设

#define BSP_USING_TIM1
#define BSP_USING_TIM3
#define BSP_USING_TIM6
#define BSP_USING_TIM14
#define BSP_USING_TIM15
#define BSP_USING_TIM16
#define BSP_USING_TIM17
/* end of Hardware TIMER */

/* Hardware WDT */
//BSP配置： 使用WDT 看门狗外设
#define BSP_USING_WDT
#define BSP_USING_IWDT
/* end of Hardware WDT */

/* Hardware SPI */
//BSP配置：是否使用SPI
/* end of Hardware SPI */

/* Hardware CRYPTO */

/* end of Hardware CRYPTO */
/* end of On-chip Peripheral Drivers */
//BSP配置：片外扩展驱动
/* Board extended module Drivers */

/* end of Hardware Drivers Config */

#endif
