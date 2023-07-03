#include <sys/types.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "spi.h"

void *spi_target = NULL;
#define LED_BYTES_NUM 24
#define LED_NUM  64  // LED灯珠个数
// 模拟bit码: 0x80为逻辑0, 0xF8为逻辑1
const uint8_t code[]={0XE0,0XF8};  //5:3

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
}rgb_color_t;  //颜色结构体

// 常见颜色定义
const rgb_color_t RED = {255, 0, 0};
const rgb_color_t BLUE = {255, 255, 255};

// 灯颜色缓存
rgb_color_t rgb_data[LED_NUM] = {0};

// 转换后数据缓存
uint8_t send_data[LED_NUM *LED_BYTES_NUM];

void rgb_data_clear(void)
{
    memset(rgb_data,0,sizeof(rgb_color_t) *LED_NUM );
    memset(send_data,0,LED_NUM *LED_BYTES_NUM );
}

int  rgb_init(void)
{
    int ret;
    ret =spi_open(&spi_target);
    rgb_data_clear();
    return ret;
}
int  rgb_deinit(void)
{
    int ret;
    ret =spi_close(&spi_target);
    return ret;
}

/**
  * @brief          控制WS2812
  * @param[in]      待发送缓存
  */
void rgb_data_conversion( uint16_t led_id)
{
    uint8_t rgb_buffer[24]={0};
    uint8_t dat_b,dat_r,dat_g;
    // 将数组颜色转化为24个要发送的字节数据
    dat_g = rgb_data[led_id].g;
    dat_r = rgb_data[led_id].r;
    dat_b = rgb_data[led_id].b;
    for (uint16_t j = 0; j < 8; j++)
    {
        rgb_buffer[7-j] =code[dat_g & 0x01];
        rgb_buffer[15-j]=code[dat_r & 0x01];
        rgb_buffer[23-j]=code[dat_b & 0x01];
        dat_g >>=1;
        dat_r >>=1;
        dat_b >>=1;
    }
    memcpy(send_data + led_id*LED_BYTES_NUM, rgb_buffer, LED_BYTES_NUM);
}

/**
  * @brief          设置灯带颜色发送缓存
  * @param[in]      ID 颜色
  */
void set_led_color(uint16_t led_id, rgb_color_t color)
{
    rgb_data[led_id].g = color.g;
    rgb_data[led_id].r = color.r;
    rgb_data[led_id].b = color.b;
    rgb_data_conversion(led_id);
}

/**
  * @brief          SPI发送控制ws2812
  * @param[in]      待发送缓存
  */
void rgb_refresh(void)
{
    spi_write(spi_target,  send_data, LED_BYTES_NUM*LED_NUM);
}


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
        set_led_color(led_num,color);
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