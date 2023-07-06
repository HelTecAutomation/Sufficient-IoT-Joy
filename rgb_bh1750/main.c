#include <sys/types.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rgb_control.h"
#include "bh1750.h"

double val_map(double x, double a, double b, double c, double d);
uint16_t coordinate_conversion(uint16_t original_coordinates);


const uint8_t love_pattern[LED_NUM]=
{
	0,0,0,1,1,1,0,0, \
	0,1,1,1,1,1,0,0, \
	0,1,1,1,1,1,1,0, \
	1,1,1,1,1,1,0,0, \
	1,1,1,1,1,1,0,0, \
	0,1,1,1,1,1,1,0, \
	0,1,1,1,1,1,0,0, \
	0,0,0,1,1,1,0,0
};


int main(int argc, char const *argv[])
{
	float lux;
	rgb_color_t rgb_color;
	uint32_t move_step;
	rgb_color.r = 36;
	rgb_color.g = 0;
	rgb_color.b = 0;
    rgb_init();
	bh1750_init(CONTINUOUS_HIGH_RES_MODE, BH1750_DEFAULT_MTREG);

	while(1)
	{
		lux = bh1750_read_light_level(0);
		if(lux > 600)
		{
			rgb_color.r = 255;
		}
		else
		{			
			rgb_color.r = (int)val_map(lux,0,600,0,255);
		}
		for(int i = 0 ; i< LED_NUM ;i++)
		{
			if(love_pattern[i])
			{
				rgb_set_led_color(coordinate_conversion((i)),rgb_color);
			}
			else
			{
				rgb_set_led_color(coordinate_conversion((i)),RGB_BLACK);
			}
		}
		rgb_refresh();
		usleep(500*1000);
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

double val_map(double x, double a, double b, double c, double d) 
{
    return (x - a) * (d - c) / (b - a) + c;
}