#include "bh1750.h"
#include "i2c.h"
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

static i2c bh1750_i2c;
static measurement_mode_t g_measurement_mode;
static uint8_t g_mt_reg;
static const float BH1750_CONV_FACTOR = 1.2;

#define BH1750_I2CADDR 0x23
#define BH1750_I2C_BUS  "/dev/i2c-2"
#define _delay_ms(ms) usleep(ms*1000)

#define errlog(errmsg) do{\
							perror(errmsg);\
							printf("%s --> %s --> %d\n", __FILE__, __func__, __LINE__);\
							exit(1);\
						 }while(0)

void bh1750_init(measurement_mode_t measurement_mode,uint8_t mt_reg)
{
    int ret;
	bh1750_i2c = i2c_open(BH1750_I2C_BUS);
	if(bh1750_i2c ==NULL)
	{
		errlog("i2c_open error");
		return ;
	}
    g_measurement_mode = measurement_mode;
    g_mt_reg = mt_reg;
    bh1750_set_measurement_mode(measurement_mode);
}

int bh1750_set_measurement_mode( measurement_mode_t measurement_mode)
{
    int ret;
    uint8_t mode;
    mode = measurement_mode;
    ret =i2c_write(bh1750_i2c, BH1750_I2CADDR, (uint8_t *)&mode,1);
	if(ret < 0)
	{
		errlog("i2c_write error");
		return ret  ;
	}
    return 0;
}



/**
 * Configure BH1750 MTreg value
 * MT reg = Measurement Time register
 * @param MTreg a value between 32 and 254. Default: 69
 * @return bool true if MTReg successful set
 * 		false if MTreg not changed or parameter out of range
 */
int bh1750_set_mt_reg(const uint8_t  mt_reg) 
{
    int ret;
    uint8_t mt_reg_val;
    if (mt_reg <= 31 || mt_reg > 254) 
    {
        printf("[BH1750] ERROR: MTreg out of range\r\n");
        return -1;
    }
    g_mt_reg = mt_reg;
    mt_reg_val = (0b01000 << 3) | (mt_reg >> 5);
    ret =i2c_write(bh1750_i2c, BH1750_I2CADDR, (uint8_t *)&mt_reg_val,1);
	if(ret < 0)
	{
		errlog("i2c_write error");
		return ret  ;
	}

    mt_reg_val = (0b011 << 5 )  | (mt_reg & 0b11111);
    ret =i2c_write(bh1750_i2c, BH1750_I2CADDR, (uint8_t *)&mt_reg_val,1);
	if(ret < 0)
	{
		errlog("i2c_write error");
		return ret  ;
	}
    bh1750_set_measurement_mode(g_measurement_mode);

}

/**
 * Read light level from sensor
 * The return value range differs if the MTreg value is changed. The global
 * maximum value is noted in the square brackets.
 * @return Light level in lux (0.0 ~ 54612,5 [117758,203])
 * 	   -1 : no valid return value
 * 	   -2 : sensor not configured
 */
float bh1750_read_light_level(uint8_t  maxWait) 
{
    int ret;
    uint8_t data[2];
  // Measurement result will be stored here
    float level = -1.0;
    bh1750_set_measurement_mode(g_measurement_mode);

  // Wait for measurement to be taken.
  // Measurements have a maximum measurement time and a typical measurement
  // time. The maxWait argument determines which measurement wait time is
  // used when a one-time mode is being used. The typical (shorter)
  // measurement time is used by default and if maxWait is set to True then
  // the maximum measurement time will be used. See data sheet pages 2, 5
  // and 7 for more details.
  // A continuous mode measurement can be read immediately after re-sending
  // the mode command.

    switch (g_measurement_mode) 
    {
        case ONE_TIME_LOW_RES_MODE:
        {
            maxWait ? _delay_ms(24 * g_mt_reg/BH1750_DEFAULT_MTREG) : _delay_ms(16 * g_mt_reg/BH1750_DEFAULT_MTREG);
            break;
        }
        case ONE_TIME_HIGH_RES_MODE:
        case ONE_TIME_HIGH_RES_MODE_2:
        {
            maxWait ? _delay_ms(180 * g_mt_reg/BH1750_DEFAULT_MTREG) :_delay_ms(120 * g_mt_reg/BH1750_DEFAULT_MTREG);
            break;
        }
        default:
        {
            break;
        }
    }

    ret = i2c_read(bh1750_i2c, BH1750_I2CADDR, data, 2);
    if(ret < 0)	
    {
        errlog("i2c_read  error");
        return ret;
    }
    level = (data[0] << 8 | data[1]);


  if (level != -1.0) 
  {
    // printf("[BH1750] Raw value: %d \r\n",level);

    if (g_mt_reg != BH1750_DEFAULT_MTREG) 
    {
        level *= (float)(BH1750_DEFAULT_MTREG/(float)g_mt_reg);
        // Print MTreg factor if debug enabled
        printf("[BH1750] MTreg factor: %f",(float)(BH1750_DEFAULT_MTREG/(float)g_mt_reg));
    }
    if (g_measurement_mode == ONE_TIME_HIGH_RES_MODE_2 || g_measurement_mode == CONTINUOUS_HIGH_RES_MODE_2)
    {
        level /= 2;
    }
    // Convert raw value to lux
    level /= BH1750_CONV_FACTOR;
  }

  return level;
}
