#include <sys/types.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rgb_control.h"

int main(int argc, char const *argv[])
{
    int ret,led_num = 0, color_num = 0;
    rgb_color_t color;
    color.r =  255;
    color.g =  0;
    color.b =  0;
    ret = rgb_init();
    while(1)
    {
        rgb_set_led_color(led_num,color);
        rgb_refresh();
        sleep(1);
        if(led_num ==(LED_NUM-1) )
        {
            led_num = 0;
            color_num++;
            if((color_num %3)==0)
            {
                color.r =  255;
                color.g =  0;
                color.b =  0;
            }
            else if((color_num %3)==1)
            {
                color.r =  0;
                color.g =  255;
                color.b =  0;
            }
            else
            {
                color.r =  0;
                color.g =  0;
                color.b =  255;
            }
        }
        else
        {
            led_num++;
        }
    }
    rgb_deinit();
    return ret;
}