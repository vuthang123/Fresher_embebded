#include "Driver_Systick.h"

uint32_t delay_count ;

void Delay_us(uint32_t microsecond)
{
    delay_count = 0; 

    while(delay_count < microsecond);
}
