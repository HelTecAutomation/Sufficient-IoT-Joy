#include <sys/types.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "spi.h"
#include "rgb_control.h"

void *spi_target = NULL;
#define LED_BYTES_NUM 24

const rgb_color_t RGB_RED   =  {100,   0,   0};
const rgb_color_t RGB_BLUE  =  {  0,   0, 100};
const rgb_color_t RGB_BLACK =  {0  ,   0,   0};
const rgb_color_t RGB_GREEN =  {0  , 100,   0};
const rgb_color_t RGB_CYAN  =  {0  , 128, 128};
const rgb_color_t RGB_YELLOW = {128, 128,   0};
const rgb_color_t RGB_PURPLE = { 66,    0, 160};
const rgb_color_t RGB_CHARTREUSE = {173,255,47};

const uint8_t code[]={0XE0,0XF8};  //5:3

rgb_color_t rgb_data[LED_NUM] = {0};

uint8_t send_data[LED_NUM *LED_BYTES_NUM];

void rgb_data_clear(void)
{
    memset(rgb_data,0,sizeof(rgb_color_t) *LED_NUM );
    memset(send_data,0,LED_NUM *LED_BYTES_NUM );
    rgb_color_t color;
    color.r =  0;
    color.g =  0;
    color.b =  0;
    for(int i=0;i < LED_NUM;i++)
    {
      rgb_set_led_color(i, color);
    }
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


void rgb_data_conversion( uint16_t led_id)
{
    uint8_t rgb_buffer[24]={0};
    uint8_t dat_b,dat_r,dat_g;
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
    memcpy(send_data + led_id*LED_BYTES_NUM , rgb_buffer, LED_BYTES_NUM);
}

void rgb_set_led_color(uint16_t led_id, rgb_color_t color)
{
    rgb_data[led_id].g = color.g;
    rgb_data[led_id].r = color.r;
    rgb_data[led_id].b = color.b;
    rgb_data_conversion(led_id);
}

void rgb_refresh(void)
{
    spi_write(spi_target,  send_data, LED_BYTES_NUM*LED_NUM);
}

