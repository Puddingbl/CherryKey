#include "key_demo.h"

void key3_click(void);

void key1_press(void)
{
	printf("����1����\n");
}
void key1_bounce(void)
{
	printf("����1����\n");
}

void key1_click(void)
{
	key_onclick_unregister(&key3, 0);
	printf("����1����\n");
}

void key1_dbclick(void)
{
	key_onclick_register(&key3, key3_click, 0, SYNC);
	printf("����1˫��\n");
}

void key1_lp(void)
{
	printf("����1����\n");
}

void key3_click(void) {
	HAL_GPIO_WritePin(rgb_g_GPIO_Port, rgb_g_Pin, GPIO_PIN_SET);  // rgb�ƹر���ɫ
}

void key4_click(void) {
	HAL_GPIO_WritePin(rgb_g_GPIO_Port, rgb_g_Pin, GPIO_PIN_RESET);  // rgb����ʾ��ɫ
}

void key5_click(void) {
	HAL_GPIO_WritePin(rgb_g_GPIO_Port, rgb_r_Pin, GPIO_PIN_SET);  // rgb�ƹرպ�
}

void key6_click(void) {
	HAL_GPIO_WritePin(rgb_g_GPIO_Port, rgb_r_Pin, GPIO_PIN_RESET);  // rgb�ƹرպ�
}

void key_demo_init(void) {
	/* ע��ص����� */
	key_onpress_register(&key1, key1_press, 0, SYNC);
	key_onbounce_register(&key1, key1_bounce, 0, SYNC);
	key_onclick_register(&key1, key1_click, 0, SYNC);
	key_ondbclick_register(&key1, key1_dbclick, 0, SYNC);
	key_onlp_register(&key1, key1_lp, 0, SYNC);
		
	key_onclick_register(&key3, key3_click, 0, ASYNC);
	key_onclick_register(&key4, key4_click, 0, ASYNC);
	key_onclick_register(&key5, key5_click, 0, ASYNC);
	key_onclick_register(&key6, key6_click, 0, ASYNC);		
}
