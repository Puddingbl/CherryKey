# CherryKey
面向对象的单片机按键驱动程序，支持gpio和adc按键，具有多种事件处理方式（Microcontroller button driver program）

整个文件是一个完整的STM32H750工程，打开STM32H750_test.ioc可以看到单片机的使用情况，在MDK-ARM文件夹有可以用keil5打开的文件。

移植说明：
1、在BSP文件夹里找到KEY.c、KEY.h、key_config.h、public.c、public.h（key_demo.c、key_demo.h可不要），在Module文件夹找到staticLinkList.c、staticLinkList.h、my_queue.c、my_queue.h，把这些文件加到自己的工程；

2、在key_config.h修改相应参数，令#define KEY_DEBUG  1可以查看调试信息；

3、在key.c里的void key_init(void)创建按键，根据实际情况设置按键有效电平。
   在static key_level_e key_level_read(key_port *port, key_pin pin)里把HAL_GPIO_ReadPin替换成你使用的单片机的读引脚函数。
   如果有使用ADC按键，那在void key_adc_update(key_adc_port_t *KeyADCPort, uint32_t adc_value)根据需要进行修改，key_adcRange数组是我根据自己的板子测量的ADC数据，你需要修改；

4、把key_init()放到初始化，定时器要在按键初始化后再开启；

5、把key_detect_scan()和my_tick_inc(x)放到定时器，建议key_detect_scan()10ms轮询一次，my_tick_inc(x)1ms一次，x填my_tick_inc(x)轮询的时间，如果是1ms就填1；

6、同样地把key_callback_handle()放到定时器，建议20ms以上轮询一次，当然也可以放到while循环；

7、如果有使用ADC按键，那么把读到的ADC值送到key_adc_update(key_adc_port_t *KeyADCPort, uint32_t adc_value)；

8、另外public.c里4-41行是关于printf的代码，你可以选择删掉它而在魔法棒里勾选Use MicroLIB，记得在
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0x000A);
    return ch;
}
把HAL_UART_Transmit更换成你使用的单片机的串口发送函数。

至此CherryKey驱动就移植到你的工程了，按动你的按键就能看到相应的调试信息，如果你想使用回调函数，key_demo.c里提供了一些简单的参考。
