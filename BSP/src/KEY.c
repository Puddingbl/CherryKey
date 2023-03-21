/**
	* 文件名：key.c
	* 描述：按键驱动
	* 
	* 时间：2022-8-8
**/

#include "key.h"
#include "my_queue.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


/* Private functions----------------------------------------------------------*/
static void key_doube_click(key_t *key_num);
static void key_detect(key_t *key_num);
static void key_debounce(key_t *key_num);

static void key_press_callback(key_t *key_num);
static void key_bounce_callback(key_t *key_num);
static void key_click_callback(key_t *key_num);
static void key_dbclick_callback(key_t *key_num);
static void key_lp_callback(key_t *key_num);	

static key_level_e key_level_read(key_port *port, key_pin pin);
static key_level_e key_adc_read(key_port *port, key_pin pin);

static void key_create(key_t *key_num, void *read, key_port *gpiox, key_pin gpio_pin, uint8_t id); // 增加新按键调用此函数
/* Private variables----------------------------------------------------------*/
/* 链表 */
static sLink_t 		  key_sll[KEY_NUMBER+2];
static staticll_t   key_link;

// 存储异步回调函数的队列
QElemType   key_cb_array[KEY_QUEUE_LEN];
Queue_t     key_cb_queue;

/* According to adc value judge which key is press*/
/* 4个ADC按键有15种情况 */
static const uint16_t key_adcRange[KEY_ADC_NUM+11][2] = 
{
	{0xdbe, 0xdaa},  //1
	{0xcca, 0xcbb},  //2
	{0xae5, 0xad5},  //3
	{0x7cf, 0x7c0},  //4
		
	{0xc37, 0xc2a},  //1 2
	{0xa38, 0xa28},  //1 3
	{0x72a, 0x71d},  //1 4
	{0x9ca, 0x9be},  //2 3
	{0x6c9, 0x6b0},  //2 4
	{0x60f, 0x5fa},  //3 4
	{0x985, 0x970},  //1 2 3
	{0x678, 0x660},  //1 2 4
	{0x5ba, 0x5a0},  //1 3 4
	{0x57f, 0x56d},	 //2 3 4
	{0x555, 0x53a}	 //1 2 3 4
};

/* Public variables-----------------------------------------------------------*/
/* ADC按键端口 */
key_adc_port_t  KeyADCPort;

/* 按键 */
key_t   key1; 
key_t   key2;
key_t   key3; 
key_t   key4;
key_t   key5; 
key_t   key6;

void key_init(void) {
	
	/* 链表、队列初始化 */
	slink_init(&key_link, key_sll, KEY_NUMBER+2);
	queue_init(&key_cb_queue, key_cb_array, KEY_QUEUE_LEN);
	
	/* 创建按键 */
	/* gpio按键 */
	key_create(&key1, &key_level_read, (key_port*)keya_GPIO_Port, keya_Pin, 1);
	key_create(&key2, &key_level_read, (key_port*)keyb_GPIO_Port, keyb_Pin, 2);	
	
	/* adc按键 */
	key_create(&key3, &key_adc_read, (key_port*)&KeyADCPort, 0, 3);	
	key_create(&key4, &key_adc_read, (key_port*)&KeyADCPort, 1, 4);	
	key_create(&key5, &key_adc_read, (key_port*)&KeyADCPort, 2, 5);	
	key_create(&key6, &key_adc_read, (key_port*)&KeyADCPort, 3, 6);	
	
	/* 设置adc按键高有效 */
	key_set_level(&key3, KEY_HIGH);
	key_set_level(&key4, KEY_HIGH);
	key_set_level(&key5, KEY_HIGH);
	key_set_level(&key6, KEY_HIGH);
}

/**
	* @name   Key_get_event
	* @brief  返回按键事件
	* @param  key_num：按键结构体
	* @return 按键事件      
*/
key_event_e key_get_event(key_t *key_num) {
	return key_num->event;
}

/**
	* @name   key_create
	* @brief  创建按键
	* @param  key_num：按键结构体
						read：读按键输入函数
						gpiox/gpio_pin：按键对应的GPIO口
            ID:辨别各个按键
	* @retval None      
*/
static void key_create(key_t *key_num, void *read, key_port *gpiox, key_pin gpio_pin, uint8_t id) {
	uint8_t i;
	
	key_num->gpiox = (key_port*)gpiox;
	key_num->gpio_pin = gpio_pin;
	
	key_num->read = read;
	key_num->id = id;
	key_num->level = KEY_LOW;
	key_num->temp = 0;	
	
	key_num->dblclick_timestamp = 0;      //
	key_num->lp_timestamp = 0;	
	
	key_num->dblclick_target = 400/KEY_TIME_BASE;    //
	key_num->lp_target = 800/KEY_TIME_BASE;          //定时器1ms
	key_num->pcnt_target = 400/KEY_TIME_BASE;	
	
	key_num->lp_interval = 0;
	key_num->pcnt_interval = 0;

	key_num->pcnt_flag = FALSE;
	key_num->bounce_flag = FALSE;
	key_num->click_flag = FALSE;
	key_num->dblclick_flag = FALSE;
	key_num->lp_flag = FALSE;	

	key_num->state = FALSE;
	key_num->press_state = FALSE;
	key_num->pcnt_state = FALSE;	
	key_num->bounce_state = FALSE;
	key_num->dbclick_state = FALSE;
	key_num->lp_state = FALSE;	
	
	key_num->event = STATELESS;
	
	for (i=0; i<KEY_CB_NUM; i++) {
		key_num->press_cb[i].callback = NULL;
		key_num->bounce_cb[i].callback = NULL;
		key_num->click_cb[i].callback = NULL;
		key_num->dbclick_cb[i].callback = NULL;
		key_num->lp_cb[i].callback = NULL;
		
		key_num->press_cb[i].async    = 0;	
		key_num->bounce_cb[i].async    = 0;	
		key_num->click_cb[i].async    = 0;	
		key_num->dbclick_cb[i].async    = 0;	
		key_num->lp_cb[i].async    = 0;	
	}	
	
	slink_insert(&key_link, id, (uint16_t*)key_num);
}

static key_level_e key_level_read(key_port *port, key_pin pin) {
	return (key_level_e)HAL_GPIO_ReadPin((GPIO_TypeDef *)port, pin);
}

/**
	* key_adc_read
	* 返回请求读取的按键的状态
	*/
static key_level_e key_adc_read(key_port *port, key_pin pin) {
	key_adc_port_t *adc_port = (key_adc_port_t *)port;	
	return adc_port->pin[pin] ? KEY_HIGH : KEY_LOW;
}

/**
	* key_adc_update
	* 所有ADC按键根据ADC值更新状态：KEY_HIGH\KEY_RESET
	* 示例函数，用户根据按键情况更改
	*/
void key_adc_update(key_adc_port_t *KeyADCPort, uint32_t adc_value) {
	uint8_t i; 
  uint32_t voltage = adc_value;
	//单个按键按下
	for (i = 0; i < KEY_ADC_NUM; i++) {
			KeyADCPort->pin[i] = (voltage < key_adcRange[i][0] && voltage > key_adcRange[i][1]) ? KEY_HIGH : KEY_LOW;
	}	

	//2个按键按下
	if (voltage < key_adcRange[4][0] && voltage > key_adcRange[4][1]) {
		KeyADCPort->pin[0] = KEY_HIGH;
		KeyADCPort->pin[1] = KEY_HIGH;
	}			
	if (voltage < key_adcRange[5][0] && voltage > key_adcRange[5][1]) {
		KeyADCPort->pin[0] = KEY_HIGH;
		KeyADCPort->pin[2] = KEY_HIGH;
	}		
	if (voltage < key_adcRange[6][0] && voltage > key_adcRange[6][1]) {
		KeyADCPort->pin[0] = KEY_HIGH;
		KeyADCPort->pin[3] = KEY_HIGH;
	}		
	if (voltage < key_adcRange[7][0] && voltage > key_adcRange[7][1]) {
		KeyADCPort->pin[1] = KEY_HIGH;
		KeyADCPort->pin[2] = KEY_HIGH;
	}		
	if (voltage < key_adcRange[8][0] && voltage > key_adcRange[8][1]) {
		KeyADCPort->pin[1] = KEY_HIGH;
		KeyADCPort->pin[3] = KEY_HIGH;
	}		
	if (voltage < key_adcRange[9][0] && voltage > key_adcRange[9][1]) {
		KeyADCPort->pin[2] = KEY_HIGH;
		KeyADCPort->pin[3] = KEY_HIGH;
	}	
	//3个按键按下
	if (voltage < key_adcRange[10][0] && voltage > key_adcRange[10][1]) {
		KeyADCPort->pin[0] = KEY_HIGH;
		KeyADCPort->pin[1] = KEY_HIGH;
		KeyADCPort->pin[2] = KEY_HIGH;
	}			
	if (voltage < key_adcRange[11][0] && voltage > key_adcRange[11][1]) {
		KeyADCPort->pin[0] = KEY_HIGH;
		KeyADCPort->pin[1] = KEY_HIGH;
		KeyADCPort->pin[3] = KEY_HIGH;
	}		
	if (voltage < key_adcRange[12][0] && voltage > key_adcRange[12][1]) {
		KeyADCPort->pin[0] = KEY_HIGH;
		KeyADCPort->pin[2] = KEY_HIGH;
		KeyADCPort->pin[3] = KEY_HIGH;
	}		
	if (voltage < key_adcRange[13][0] && voltage > key_adcRange[13][1]) {
		KeyADCPort->pin[1] = KEY_HIGH;
		KeyADCPort->pin[2] = KEY_HIGH;
		KeyADCPort->pin[3] = KEY_HIGH;
	}
	//4个按键按下
	if (voltage < key_adcRange[14][0] && voltage > key_adcRange[14][1]) {
		KeyADCPort->pin[0] = KEY_HIGH;
		KeyADCPort->pin[1] = KEY_HIGH;
		KeyADCPort->pin[2] = KEY_HIGH;
		KeyADCPort->pin[3] = KEY_HIGH;
	}		
}

/**
	* 设置按键有效电平
	* 参数 level:HIGH/LOW
	*/
void key_set_level(key_t *key_num, key_level_e level) {
	key_num->level = level;
}

/**
	* 设置双击间隔时间
	*/
void key_set_dbt(key_t *key_num, uint16_t t) {
	key_num->dblclick_target = t;
}

/**
	* 设置长按时间
	*/
void key_set_lpt(key_t *key_num, uint16_t t) {
	key_num->lp_target = t;
}

/**
	* @name   Key_Detect_scan
	* @brief  按键事件检测扫描函数
	* @param  NULL
	* @retval None      
*/
void key_detect_scan() {
	uint16_t i, length;

	length = slink_length(&key_link);
	
	for (i=0; i<length; i++) {	
		key_detect((key_t*)slink_getData(&key_link, i+1));
	}	
}

/**
	* @name   key_debounce
	* @brief  按键消抖函数
	* @param  key_num：按键结构体
	* @retval None      
*/
static void key_debounce(key_t *key_num) {	
	uint32_t now = my_tick_get();
	
	key_num->state = key_num->read(key_num->gpiox, key_num->gpio_pin); // 读键值

	if (key_num->state) {
		if (key_num->level) {
			key_num->temp = 1;
		}
		else {
			key_num->temp = 0;
		}		
	}
	else {
		if (key_num->level) {
			key_num->temp = 0;
		}
		else {
			key_num->temp = 1;
		}		
	}
		
	key_num->press_state = (key_num->temp & (key_num->temp ^ key_num->lp_state));
	key_num->bounce_state = key_num->temp;
	key_num->lp_state = key_num->temp;	
	key_num->pcnt_state = key_num->temp;
}


/**
	* @name   key_register_cb
	* @brief  注册按下回调函数
	* @param  key_num  按键对象
						callback 需要注册的回调函数
						index    插入到数组的第几位，决定执行顺序
						async		 该回调函数同步或者异步执行
											@arg  SYNC  同步
											@arg  ASYNC 异步
	* @return 无
	*/
void key_onpress_register(key_t *key_num, key_cb callback, uint8_t index, uint8_t async) {
	
	if((index > KEY_CB_NUM-1) || (index < 0) || (callback == NULL))
		return;
	
	key_num->press_cb[index].callback = callback;
	key_num->press_cb[index].async = async;
}

/**
	*		注销已经注册的回调函数
	*/
void key_onpress_unregister(key_t *key_num, uint8_t index) {
	
	if(index > KEY_CB_NUM-1)
		return;
	
	key_num->press_cb[index].callback = NULL;
	key_num->press_cb[index].async = 0;
}

void key_onbounce_register(key_t *key_num, key_cb callback, uint8_t index, uint8_t async) {
	
	if((index > KEY_CB_NUM-1) || (index < 0) || (callback == NULL))
		return;
	
	key_num->bounce_cb[index].callback = callback;
	key_num->bounce_cb[index].async = async;
}

void key_onbounce_unregister(key_t *key_num, uint8_t index) {
	
	if((index > KEY_CB_NUM-1) || (index < 0))
		return;
	
	key_num->bounce_cb[index].callback = NULL;
	key_num->bounce_cb[index].async = 0;
}

void key_onclick_register(key_t *key_num, key_cb callback, uint8_t index, uint8_t async) {
	
	if((index > KEY_CB_NUM-1) || (index < 0) || (callback == NULL))
		return;
	
	key_num->click_cb[index].callback = callback;
	key_num->click_cb[index].async = async;
}

void key_onclick_unregister(key_t *key_num, uint8_t index) {
	
	if((index > KEY_CB_NUM-1) || (index < 0))
		return;
	
	key_num->click_cb[index].callback = NULL;
	key_num->click_cb[index].async = 0;
}

void key_ondbclick_register(key_t *key_num, key_cb callback, uint8_t index, uint8_t async) {
	
	if((index > KEY_CB_NUM-1) || (index < 0) || (callback == NULL))
		return;
	
	key_num->dbclick_cb[index].callback = callback;
	key_num->dbclick_cb[index].async = async;
}

void key_ondbclick_unregister(key_t *key_num, uint8_t index) {
	
	if((index > KEY_CB_NUM-1) || (index < 0))
		return;
	
	key_num->dbclick_cb[index].callback = NULL;
	key_num->dbclick_cb[index].async = 0;
}

void key_onlp_register(key_t *key_num, key_cb callback, uint8_t index, uint8_t async) {
	
	if((index > KEY_CB_NUM-1) || (index < 0) || (callback == NULL))
		return;
	
	key_num->lp_cb[index].callback = callback;
	key_num->lp_cb[index].async = async;
}

void key_onlp_unregister(key_t *key_num, uint8_t index) {
	
	if((index > KEY_CB_NUM-1) || (index < 0))
		return;
	
	key_num->lp_cb[index].callback = NULL;
	key_num->lp_cb[index].async = 0;
}

/**
	*  异步处理按键回调函数
	*/
void key_callback_handle(void) {
	uint8_t     i, len;
	key_cb      callback = NULL;
	
	len = queue_length(key_cb_queue);
	for (i=0; i<len; i++) {
		queue_extract(&key_cb_queue, (QElemType*)&callback);
		callback();
	}
}


/**
	*  按键事件判断
	*/
static void key_detect(key_t *key_num) {

	key_debounce(key_num);																			// 消抖
	if (key_num->bounce_state == FALSE) {		
		if (key_num->bounce_flag == FALSE) {											// 如果不是按下后的释放，直接返回
			key_num->event = STATELESS;
			return;
		}
		else {                       
			key_num->event = BOUNCE;																// bounce
			key_num->bounce_flag = FALSE;
			KEY_LOG("key%d bounce\n", key_num->id);	
			key_bounce_callback(key_num);
			
			if (key_num->click_flag) {										 		
				key_num->event = CLICK;											 					// click	
				key_num->click_flag = FALSE;
				KEY_LOG("key%d click\n", key_num->id);
				key_click_callback(key_num);
				key_doube_click(key_num);															// 双击判断				
			}		
		}
	}	
	else if (key_num->press_state == TRUE) {       
		key_num->event = PRESS;																		// press
		key_num->bounce_flag = TRUE;
		key_num->click_flag = TRUE;	
		key_num->pcnt_timestamp = my_tick_get();									// 连按开始时间戳
		key_num->lp_flag = TRUE;
		key_num->lp_timestamp = my_tick_get();									  // 长按开始时间戳
		KEY_LOG("key%d press\n", key_num->id);
		key_press_callback(key_num); 
	}
	else if (key_num->pcnt_state == TRUE) { 										// 连按
		key_num->pcnt_interval = key_num->pcnt_timestamp;
		if (my_tick_elaps(key_num->pcnt_interval) >= key_num->pcnt_target) {
			key_num->event = PRESS;
			key_num->click_flag = FALSE;														// 连按后弹起不触发click		
			key_num->pcnt_timestamp = my_tick_get();
			key_press_callback(key_num);
			KEY_LOG("key%d press_cnt\n", key_num->id);
			
		}
	}
	if ((key_num->lp_state == TRUE) && (key_num->lp_flag == TRUE)) {
		key_num->lp_interval = key_num->lp_timestamp;
		if (my_tick_elaps(key_num->lp_interval) >= key_num->lp_target) {
			key_num->event = LONG_PRESS;
			key_num->click_flag = FALSE;															// 长按后弹起不触发click
			key_num->lp_flag = FALSE;																	// 长按只触发一次
			KEY_LOG("key%d lp\n", key_num->id);
			key_lp_callback(key_num);
		}		
	}	
}

static void key_press_callback(key_t *key_num) {
	uint8_t i;
	
	for (i=0; i<KEY_CB_NUM; i++) {
		if (key_num->press_cb[i].async == SYNC) {     // 同步，立即执行
			key_num->press_cb[i].callback();  							
		}     
		else if(key_num->press_cb[i].async == ASYNC) {				// 异步，加入队列
			queue_insert(&key_cb_queue, key_num->press_cb[i].callback);
		}																 						
	}	
}

static void key_bounce_callback(key_t *key_num) {
	uint8_t i;
	
	for (i=0; i<KEY_CB_NUM; i++) {
		if (key_num->bounce_cb[i].async == SYNC) {     // 同步，立即执行
 			key_num->bounce_cb[i].callback();  							
		}     
		else if(key_num->bounce_cb[i].async == ASYNC) {				// 异步，加入队列
			queue_insert(&key_cb_queue, key_num->bounce_cb[i].callback);
		}																 						
	}	
}

static void key_click_callback(key_t *key_num) {
	uint8_t i;
	
	for (i=0; i<KEY_CB_NUM; i++) {
		if (key_num->click_cb[i].async == SYNC) {     // 同步，立即执行
			key_num->click_cb[i].callback();  							
		}     
		else if(key_num->click_cb[i].async == ASYNC) {				// 异步，加入队列
			queue_insert(&key_cb_queue, key_num->click_cb[i].callback);
		}																 						
	}	
}

static void key_dbclick_callback(key_t *key_num) {
	uint8_t i;
	
	for (i=0; i<KEY_CB_NUM; i++) {
		if (key_num->dbclick_cb[i].async == SYNC) {     // 同步，立即执行
			key_num->dbclick_cb[i].callback();  							
		}     
		else if(key_num->dbclick_cb[i].async == ASYNC) {				// 异步，加入队列
			queue_insert(&key_cb_queue, key_num->dbclick_cb[i].callback);
		}																 						
	}	
}

static void key_lp_callback(key_t *key_num) {
	uint8_t i;
	
	for (i=0; i<KEY_CB_NUM; i++) {
		if (key_num->lp_cb[i].async == SYNC) {     // 同步，立即执行
			key_num->lp_cb[i].callback();  							
		}     
		else if(key_num->lp_cb[i].async == ASYNC) {				// 异步，加入队列
			queue_insert(&key_cb_queue, key_num->lp_cb[i].callback);
		}																 						
	}	
}

/**
	*  双击判断
	*/
static void key_doube_click(key_t *key_num) {
	
	if (key_num->dblclick_flag == 0) {
		key_num->dblclick_timestamp = my_tick_get();
		key_num->dblclick_flag = 1;
	}
	else if (key_num->dblclick_flag == 1) {	
		key_num->dblclick_timestamp = my_tick_elaps(key_num->dblclick_timestamp);	
		if (key_num->dblclick_timestamp < key_num->dblclick_target) {
			key_num->event = DOUBLE_CLICK;
			key_num->dblclick_flag = 0;
			KEY_LOG("key%d dbclick\n", key_num->id);
			key_dbclick_callback(key_num);
		}	
		else {
			key_num->dblclick_timestamp = my_tick_get();
		}
	}
}

