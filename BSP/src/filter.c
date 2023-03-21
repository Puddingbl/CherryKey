#include "filter.h"


/**
	@ ��ֵƽ���˲�
	@  size>2
*/
filter_type MedianFilter(filter_type* data,int size)
{
    filter_type max,min;
    int sum = 0;
	
    if(size>2)
    {
        max = data[0];
        min = max;

        for(int i=0; i<size; i++)
        {
            sum += data[i];            
            if(data[i] > max)
            {
                max = data[i];   //һ��ѭ��֮��max��������ֵ
            }

            if(data[i]<min)
            {
                min = data[i];   //һ��ѭ��֮��min������С��ֵ
            }
        }

        sum = sum-max-min;       //ȥ������ֵ����С��ֵ
        return sum/(size-2); //��N-2������ƽ��ֵ          
    }
    return 0;
}


