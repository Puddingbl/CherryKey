#ifndef __KEY_H__
#define __KEY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "public.h"
#include "key_config.h"
#include "staticLinkList.h"

// �����¼�
typedef enum {
	STATELESS = 0,				// ���¼�
	PRESS,								// ����
	BOUNCE,								// ����
	CLICK,								// ����
	LONG_PRESS,						// ����
	DOUBLE_CLICK,					// ˫��
} key_event_e;

// ������ƽ
typedef enum {
	KEY_LOW = 0,
	KEY_HIGH,
} key_level_e;

// �����ص�����ͬ��/�첽ִ��
typedef enum {
	SYNC  = 1,
	ASYNC = 2,		//�첽
} key_async_e;


// ��ӦGPIO_TypeDef
typedef struct {
	key_pin 	pin[KEY_ADC_NUM];
} key_adc_port_t;



// �ص�����
typedef void (*key_cb)(void);

typedef struct {
	key_cb   callback;
	uint8_t  async;
} key_cb_t;

typedef struct {	
	key_level_e  (*read)(key_port *port, key_pin pin);    // ���������뺯��
	key_port    *gpiox;            // GPIO��	
	key_pin    gpio_pin;					 // GPIO�˿�		

	uint32_t dblclick_target; 		 // ˫������Ŀ��ֵ������ݶ�ʱ���Լ�����ֵ	
	uint32_t dblclick_timestamp;   // �ж�˫����ʼ��ʱ��ʱ��

	uint32_t lp_target;            // ��������Ŀ��ֵ������ݶ�ʱ���Լ�����ֵ	
	uint32_t lp_timestamp;  		   // �жϳ�����ʼ��ʱ��ʱ��
	uint32_t lp_interval;          // ʱ����

	uint32_t pcnt_timestamp;       // ����ʱ���
	uint32_t pcnt_interval;        // ����ʱ����
	uint32_t pcnt_target;
	
	uint8_t  level:1;					     // ��������Ϊ�ߵ�ƽ���ǵ͵�ƽ
	uint8_t  temp:1;    				 	 // ��������	
	
  uint8_t  state:1;              // ��ȡIO�ڷ��ص�״̬
	uint8_t  press_state:1;              // ����
	uint8_t  pcnt_state:1;         // ��������	
	uint8_t  bounce_state:1;			 // ����
	uint8_t  dbclick_state:1;      // ˫��
	uint8_t  lp_state:1;           // ����״̬	

	uint8_t  pcnt_flag:1;          // �������±�־	
	uint8_t  bounce_flag:1;				 // �жϵ����־λ
	uint8_t  click_flag:1;				 // �жϰ��µ��𣨵�������־λ
	uint8_t  dblclick_flag:1;		   // 	
	uint8_t  lp_flag:1;            // ������־	
	
	uint8_t  id;                   // id,����������	
	
	key_event_e  event;						 				// ����״̬����Ӧ�ò����	
	key_cb_t     press_cb[KEY_CB_NUM];		// ���»ص�	
	key_cb_t     bounce_cb[KEY_CB_NUM];		// ����
	key_cb_t     click_cb[KEY_CB_NUM];    // ���µ��𣨵�����
	key_cb_t     dbclick_cb[KEY_CB_NUM];	// ˫��
	key_cb_t     lp_cb[KEY_CB_NUM];		    // ����
} key_t;



 /* exported functions ------------------------------------------------------- */
void key_init(void);                        // ��ʼ��
void key_adc_update(key_adc_port_t *KeyADCPort, uint32_t adc_value);    // ����adcֵ
void key_detect_scan(void);									// ����ɨ��
void key_callback_handle(void);             // �ص�����
key_event_e key_get_event(key_t *key_num);  // ��ȡ�����¼�

/* �������ú��� */
void key_set_level(key_t *key_num, key_level_e level);
void key_set_dbt(key_t *key_num, uint16_t t);
void key_set_lpt(key_t *key_num, uint16_t t);

// �ص�����ע��/ע��
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

/*���д�������*/
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
