Just change the number of LED lights. At present, the maximum number is 200.
#define LED_NUM  64  // LED灯珠个数

## 2023062602
1. 增加穿墙功能

## 2023062603
1. 增加按键加速功能

## 2023062801
1. 新增 ctrl + C 退出功能



## 

```c
#include <sys/types.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rgb_control.h"
#include "press_key.h"
uint16_t coordinate_conversion(uint16_t original_coordinates);


#define MAP_WIDE (10)
#define MAP_HIGH (MAP_WIDE)
#define MAP_AREA (MAP_WIDE * MAP_HIGH)
#define SNAKE_INIT_LEN  (2)

int main(int argc, char const *argv[])
{
    rgb_init();
    press_key_init();
	int map[MAP_AREA + 1];
	int walking_trajectory[MAP_AREA], walking_trajectory_tail_coordinates = 0, walking_trajectory_head_coordinates = SNAKE_INIT_LEN, move_direction = 1;
	int snake_head, snake_tail,food, i;
	for (i = walking_trajectory_tail_coordinates; i <= walking_trajectory_head_coordinates; ++i) walking_trajectory[i] = MAP_AREA;
	for (i = 0; i < MAP_AREA; ++i ) 
	{
		/*初始化地图
		i / MAP_WIDE % (MAP_HIGH - 1) 表示上下两侧的墙
		i % MAP_WIDE % (MAP_WIDE - 1) 表示左右两侧的墙*/
		map[i] = !(i / MAP_WIDE % (MAP_HIGH - 1) && i % MAP_WIDE % (MAP_WIDE - 1));
	}
	snake_head = MAP_WIDE / 2 * (MAP_HIGH + 1) - move_direction;
	food = snake_head + move_direction;
	for (srand((unsigned)time(NULL)); i = 1; usleep(1000)) 
    {
		if ( (i = press_key_get_key_val() & 95))
        {
			if (i == 'S' && move_direction != 1)
            {
                move_direction = -1;
            }
			else if (i == 'W' && move_direction != -1)
            {
                move_direction = 1;
            } 
			else if (i == 'A' && move_direction != MAP_WIDE)
            {
                move_direction = -MAP_WIDE;
            }
			else if (i == 'D' && move_direction != -MAP_WIDE)
            {
                move_direction = MAP_WIDE;
            }
            else if(i == 3)
			{
				break;
			}
            printf("i== %c move_direction==%d\r\n",i,move_direction);
		}
		snake_head += move_direction;
		/*map[snake_head += move_direction] 撞墙和撞尾部检测，墙体是 1 ，蛇身是1，其余是0  ，
		snake_head != walking_trajectory[walking_trajectory_tail_coordinates + 1] */
		if (map[snake_head] && snake_head != walking_trajectory[walking_trajectory_tail_coordinates + 1] || i == 27)
        {
            if ((snake_head / MAP_WIDE % (MAP_HIGH - 1)) ==0)
			{
				if (snake_head < MAP_WIDE)
				{
					snake_head = MAP_WIDE * (MAP_WIDE - 2) + snake_head;
				}
				else
				{
					snake_head = snake_head - MAP_WIDE * (MAP_WIDE - 2);
				}
			}
			else if ((snake_head % MAP_WIDE % (MAP_WIDE - 1))==0)
			{
				if (snake_head % MAP_WIDE == 0)
				{
					snake_head = snake_head + MAP_WIDE - 2;
				}
				else
				{
					snake_head = (snake_head / MAP_WIDE)* MAP_WIDE + 1;
				}
			}
			else
			{
				rgb_set_led_color(coordinate_conversion(food),RGB_RED);
				rgb_refresh();
				sleep(1);
				rgb_set_led_color(coordinate_conversion(food),RGB_GREEN);
				rgb_refresh();
				sleep(1);
				rgb_set_led_color(coordinate_conversion(food),RGB_PURPLE);
				rgb_refresh();
				sleep(1);
                printf("%d %d %d %d\r\n",map[snake_head += move_direction],snake_head,walking_trajectory[walking_trajectory_tail_coordinates + 1],i);
                break;
            }
        }
		//如果蛇头坐标等于食物坐标，就需要重新生成食物坐标
		if (snake_head == food)
        {
			//新生成的食物坐标不能在墙上，或者蛇体上
			for (food = rand() % MAP_AREA; map[food]; food = (food + 1) % MAP_AREA)
            {

            }
			//展示食物所在坐标
            rgb_set_led_color(coordinate_conversion(food),RGB_BLUE);
		}
		else
        {
			//把蛇尾部的图形清除
			walking_trajectory_tail_coordinates = (walking_trajectory_tail_coordinates + 1) % MAP_AREA;
			snake_tail = walking_trajectory[walking_trajectory_tail_coordinates];
			map[snake_tail] = 0;
            rgb_set_led_color(coordinate_conversion(i),RGB_BLACK);
		}
        //标记食物所在地
        rgb_set_led_color(coordinate_conversion(food),RGB_BLUE);
		// 记录移动轨迹
		walking_trajectory[walking_trajectory_head_coordinates] = snake_head;
		walking_trajectory_head_coordinates = (walking_trajectory_head_coordinates + 1) % MAP_AREA;
		//把蛇身向前移动一格
		map[snake_head] = 1;
        rgb_set_led_color(coordinate_conversion(snake_head),RGB_RED);
        rgb_refresh();
	}
    printf("\nGame over!\n");
    // rgb_deinit();
    press_key_deinit();
}

uint16_t coordinate_conversion(uint16_t original_coordinates)
{
	const uint8_t matrix_size = 10;
	const uint8_t conversion_base = 8;

	uint16_t temp;
	temp = original_coordinates - (11+ 2*(original_coordinates / matrix_size -1));
	if ((temp / conversion_base)%2== 0)
	{
		return temp;
	}
	temp = ((temp / conversion_base)*2 + 1) * conversion_base -1 - temp;
	return temp;
}



```