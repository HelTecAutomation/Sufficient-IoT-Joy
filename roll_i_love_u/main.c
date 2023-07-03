#include <sys/types.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rgb_control.h"
#include "press_key.h"

const uint8_t i_love_u_pattern[]=
{
	1,0,0,0,0,0,0,1, \
	1,1,1,1,1,1,1,1, \
	1,0,0,0,0,0,0,1, \
	0,0,0,0,0,0,0,0, \
	0,0,0,0,0,0,0,0, \

	0,0,0,1,1,1,0,0, \
	0,0,1,1,1,1,1,0, \
	0,1,1,1,1,1,1,0, \
	1,1,1,1,1,1,0,0, \
	0,1,1,1,1,1,1,0, \
	0,0,1,1,1,1,1,0, \
	0,0,0,1,1,1,0,0, \

	0,0,0,0,0,0,0,0, \
	0,0,0,0,0,0,0,0, \
	1,1,1,1,1,1,1,1, \
	1,0,0,0,0,0,0,0, \
	1,1,1,1,1,1,1,1, \
	0,0,0,0,0,0,0,0, \
	0,0,0,0,0,0,0,0
};
uint16_t coordinate_conversion(uint16_t original_coordinates);
int main(int argc, char const *argv[])
{
	rgb_color_t rgb_color;
	uint32_t move_step;
	uint8_t color_flipping_val;
	uint8_t array_overflow_flag;
	rgb_color.r = 64;
	rgb_color.g = 0;
	rgb_color.b = 0;
	move_step = 0;
	color_flipping_val = 1;
    rgb_init();

	while(1)
	{
		for(int j = 0 ;j<sizeof(i_love_u_pattern);j+=8 )
		{
			array_overflow_flag = 0;
			for(int k,i = 0 ; i< LED_NUM ;i++)
			{
				if(((i+j) >= sizeof(i_love_u_pattern)) && (array_overflow_flag==0))
				{
					array_overflow_flag = 1;
					k = 0;
				}
				else if(((i+j) >= sizeof(i_love_u_pattern)) && (array_overflow_flag==1))
				{
					k +=1 ;
				}
				else
				{
					k = i+j;
				}
				if(i_love_u_pattern[k])
				{
					rgb_set_led_color(coordinate_conversion(i),rgb_color);
				}
				else
				{
					rgb_set_led_color(coordinate_conversion(i),RGB_BLACK);
				}
			}
			rgb_refresh();
			usleep(300*1000);
		}
	}

}

uint16_t coordinate_conversion(uint16_t original_coordinates)
{
	const uint8_t conversion_base = 8;
	uint16_t temp  = original_coordinates;

	if ((temp / conversion_base) % 2 == 0)
	{
		return temp;
	}
	temp = ((temp / conversion_base) * 2 + 1) * conversion_base - 1 - temp;
	return temp;
}