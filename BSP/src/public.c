
#include "public.h"

/* support printf function, usemicrolib is unnecessary */
#if (__ARMCC_VERSION > 6000000)
  __asm (".global __use_no_semihosting\n\t");
  void _sys_exit(int x)
  {
    x = x;
  }
  /* __use_no_semihosting was requested, but _ttywrch was */
  void _ttywrch(int ch)
  {
    ch = ch;
  }
  FILE __stdout;
#else
 #ifdef __CC_ARM
  #pragma import(__use_no_semihosting)
  struct __FILE
  {
    int handle;
  };
  FILE __stdout;
  void _sys_exit(int x)
  {
    x = x;
  }
  /* __use_no_semihosting was requested, but _ttywrch was */
  void _ttywrch(int ch)
  {
    ch = ch;
  }
 #endif
#endif

#if defined (__GNUC__) && !defined (__clang__)
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

	
/**
	* @name   fputc
	* @brief  fputcӳ��������
	* @param  ch -> �������ַ�
	* @retval ch -> �ѷ����ַ�      
*/
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0x000A);
  return ch;
}


static uint32_t my_sys_time = 0;
static volatile uint8_t my_tick_irq_flag;
/**
 * �δ�
 * @param tick_period ��������
 */
void my_tick_inc(uint32_t tick_period)
{
    my_tick_irq_flag = 0;
    my_sys_time += tick_period;
}

/**
 * ���ص�ǰʱ��
 * @return ���ص�ǰʱ��
 */
uint32_t my_tick_get(void)
{
    uint32_t result;
    do {
        my_tick_irq_flag = 1;
        result        = my_sys_time;
    } while(!my_tick_irq_flag); /*Continue until see a non interrupted cycle*/

    return result;
}

/**
 * ��ȡ����һ��ʱ������������ĺ�����
 * @param prev_tick ǰһ��ʱ��� 
 * @return �ԡ�prev_tick�����������ĺ�����
 */
uint32_t my_tick_elaps(uint32_t prev_tick)
{
    uint32_t act_time = my_tick_get();

    if(act_time >= prev_tick) {
        prev_tick = act_time - prev_tick;
    }
    else {
        prev_tick = UINT32_MAX - prev_tick + 1;
        prev_tick += act_time;
    }

    return prev_tick;
}

