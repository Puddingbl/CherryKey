#include "filter.h"


/**
	@ 中值平均滤波
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
                max = data[i];   //一个循环之后max就是最大的值
            }

            if(data[i]<min)
            {
                min = data[i];   //一个循环之后min就是最小的值
            }
        }

        sum = sum-max-min;       //去掉最大的值和最小的值
        return sum/(size-2); //对N-2个数求平均值          
    }
    return 0;
}


