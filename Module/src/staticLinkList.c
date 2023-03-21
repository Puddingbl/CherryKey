/**
	* �ļ���  staticLinkList.c
	* ����̬����
	* ������������ӦΪ���󳤶�+2
	* ʱ�� 2022-8-4
**/


#include "staticLinkList.h"

/**
	*��̬�����ʼ��
	*���� slink  ������С��Ϣ�ľ�̬����
				sll    ������̬����
				size   �����С
*/
uint8_t slink_init(staticll_t *slink, sLink_t *sll, uint16_t size)
{	
	uint16_t i;
	
	slink->max_size = size;
	slink->sll = sll;
	for(i=0; i < size-1; i++)
	{
		slink->sll[i].cur = i+1;
	}
	slink->sll[size-1].cur = 0;
	
	return 1;
}

/**
	*��̬�����ڴ����뺯��
*/

uint16_t slink_malloc(staticll_t *slink)
{
	uint16_t i = slink->sll[0].cur;
	
	if(slink->sll[0].cur)
	{
		slink->sll[0].cur = slink->sll[i].cur;
	}
	
	return i;
}

/**
	*��̬�����ڴ��ͷź���
*/

void slink_free(staticll_t *slink, uint16_t k)
{
	slink->sll[k].cur = slink->sll[0].cur;
	slink->sll[0].cur = k;
}

/**
	*���ؾ�̬������
*/
uint16_t slink_length(staticll_t *slink)
{
	uint16_t j = 0;
	uint16_t i = slink->sll[slink->max_size-1].cur;
	
	while(i)
	{
		i = slink->sll[i].cur;
		j++;
	}
	
	return j;
}

/**
	*��̬������뺯��
*/

uint8_t slink_insert(staticll_t *slink, uint16_t i, sLinkType e)
{
	uint16_t j, k, l;
	
	k = slink->max_size - 1;
	if(i<1 || i>slink_length(slink)+1)
		return 0;
	
	j = slink_malloc(slink);
	
	if(j)
	{
		slink->sll[j].data = e;
		for(l=1; l<=i-1; l++)
		{
			k = slink->sll[k].cur;
		}
	
		slink->sll[j].cur = slink->sll[k].cur;
		slink->sll[k].cur = j;
		return 1;
	}

	return 0;
}

/**
	* ɾ����̬�����i����Ա
*/

uint8_t slink_delete(staticll_t *slink, uint16_t i)
{
	uint16_t j, k;
	
	k = slink->max_size - 1;
	if(i<1 || i>slink_length(slink)+1)
		return 0;
	
	
	for(j=1; j<i-1; j++)
	{
		k = slink->sll[k].cur;
	}	
	
	j = slink->sll[k].cur;
	slink->sll[k].cur = slink->sll[j].cur;
	slink_free(slink, j);

	return 1;
}

/**
	*��ȡ��̬�����k����Ա����
*/

sLinkType slink_getData(staticll_t *slink, uint16_t k)
{
	uint16_t i;
	uint16_t j = slink->max_size - 1;

	for(i=0; i<k; i++)
	{
		j = slink->sll[j].cur;
	}		

	return slink->sll[j].data;
}
