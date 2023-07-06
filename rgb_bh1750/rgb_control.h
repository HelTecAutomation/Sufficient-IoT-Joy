#ifndef __RGB_CONTROL_H__
#define __RGB_CONTROL_H__

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
}rgb_color_t;  

#define LED_NUM  64  

extern const rgb_color_t RGB_RED   ;
extern const rgb_color_t RGB_BLUE  ;
extern const rgb_color_t RGB_BLACK ;
extern const rgb_color_t RGB_GREEN ;
extern const rgb_color_t RGB_CYAN  ;
extern const rgb_color_t RGB_YELLOW ;
extern const rgb_color_t RGB_PURPLE ;
extern const rgb_color_t RGB_CHARTREUSE ;

void rgb_set_led_color(uint16_t led_id, rgb_color_t color);
void rgb_data_clear(void);
int  rgb_init(void);
int  rgb_deinit(void);
void rgb_data_conversion( uint16_t led_id);
void rgb_refresh(void);


#endif