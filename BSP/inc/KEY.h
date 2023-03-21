#ifndef __KEY_H__
#define __KEY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "public.h"
#include "key_config.h"
#include "staticLinkList.h"

// 按键事件
typedef enum {
	STATELESS = 0,				// 无事件
	PRESS,								// 按下
	BOUNCE,								// 弹起
	CLICK,								// 单击
	LONG_PRESS,						// 长按
	DOUBLE_CLICK,					// 双击
} key_event_e;

// 按键电平
typedef enum {
	KEY_LOW = 0,
	KEY_HIGH,
} key_level_e;

// 按键回调函数同步/异步执行
typedef enum {
	SYNC  = 1,
	ASYNC = 2,		//异步
} key_async_e;


// 对应GPIO_TypeDef
typedef struct {
	key_pin 	pin[KEY_ADC_NUM];
} key_adc_port_t;



// 回调函数
typedef void (*key_cb)(void);

typedef struct {
	key_cb   callback;
	uint8_t  async;
} key_cb_t;

typedef struct {	
	key_level_e  (*read)(key_port *port, key_pin pin);    // 读按键输入函数
	key_port    *gpiox;            // GPIO组	
	key_pin    gpio_pin;					 // GPIO端口		

	uint32_t dblclick_target; 		 // 双击计数目标值，需根据定时器自己定初值	
	uint32_t dblclick_timestamp;   // 判断双击开始计时的时间

	uint32_t lp_target;            // 长按计数目标值，需根据定时器自己定初值	
	uint32_t lp_timestamp;  		   // 判断长按开始计时的时间
	uint32_t lp_interval;          // 时间间隔

	uint32_t pcnt_timestamp;       // 连按时间戳
	uint32_t pcnt_interval;        // 连按时间间隔
	uint32_t pcnt_target;
	
	uint8_t  level:1;					     // 按键按下为高电平还是低电平
	uint8_t  temp:1;    				 	 // 用于消抖	
	
  uint8_t  state:1;              // 读取IO口返回的状态
	uint8_t  press_state:1;              // 按下
	uint8_t  pcnt_state:1;         // 连续按下	
	uint8_t  bounce_state:1;			 // 弹起
	uint8_t  dbclick_state:1;      // 双击
	uint8_t  lp_state:1;           // 长按状态	

	uint8_t  pcnt_flag:1;          // 连续按下标志	
	uint8_t  bounce_flag:1;				 // 判断弹起标志位
	uint8_t  click_flag:1;				 // 判断按下弹起（单击）标志位
	uint8_t  dblclick_flag:1;		   // 	
	uint8_t  lp_flag:1;            // 长按标志	
	
	uint8_t  id;                   // id,辨别各个按键	
	
	key_event_e  event;						 				// 按键状态，供应用层调用	
	key_cb_t     press_cb[KEY_CB_NUM];		// 按下回调	
	key_cb_t     bounce_cb[KEY_CB_NUM];		// 弹起
	key_cb_t     click_cb[KEY_CB_NUM];    // 按下弹起（单击）
	key_cb_t     dbclick_cb[KEY_CB_NUM];	// 双击
	key_cb_t     lp_cb[KEY_CB_NUM];		    // 长按
} key_t;



 /* exported functions ------------------------------------------------------- */
void key_init(void);                        // 初始化
void key_adc_update(key_adc_port_t *KeyADCPort, uint32_t adc_value);    // 更新adc值
void key_detect_scan(void);									// 按键扫描
void key_callback_handle(void);             // 回调处理
key_event_e key_get_event(key_t *key_num);  // 获取按键事件

/* 参数设置函数 */
void key_set_level(key_t *key_num, key_level_e level);
void key_set_dbt(key_t *key_num, uint16_t t);
void key_set_lpt(key_t *key_num, uint16_t t);

// 回调函数注册/注销
void key_onpress_register(key_t *key_num, key_cb callback, uint8_t index, uint8_t async);
void key_onpress_unregister(key_t *key_num, uint8_t index);
void key_onbounce_register(key_t *key_num, key_cb callback, uint8_t index, uint8_t async);
void key_onbounce_unregister(key_t *key_num, uint8_t index);
void key_onclick_register(key_t *key_num, key_cb callback, uint8_t index, uint8_t async);
void key_onclick_unregister(key_t *key_num, uint8_t index);
void key_ondbclick_register(key_t *key_num, key_cb callback, uint8_t index, uint8_t async);
void key_ondbclick_unregister(key_t *key_num, uint8_t index);
void key_onlp_register(key_t *key_num, key_cb callback, uint8_t index, uint8_t async);
void key_onlp_unregister(key_t *key_num, uint8_t index);

/* extern variables-----------------------------------------------------------*/
extern key_adc_port_t  KeyADCPort;

/*自行创建按键*/
extern key_t   key1;
extern key_t   key2;
extern key_t   key3; 
extern key_t   key4;
extern key_t   key5; 
extern key_t   key6;

#ifdef __cplusplus
}
#endif

#endif
