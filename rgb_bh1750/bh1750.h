#ifndef __BH1750_H__
#define __BH1750_H__

#include "stdint.h"
#include "stdio.h"

// No active state
#define BH1750_POWER_DOWN 0x00

// Waiting for measurement command
#define BH1750_POWER_ON 0x01

// Reset data register value - not accepted in POWER_DOWN mode
#define BH1750_RESET 0x07

// Default MTreg value
#define BH1750_DEFAULT_MTREG 69


typedef enum 
{
    UNCONFIGURED = 0,
    // Measurement at 1 lux resolution. Measurement time is approx 120ms.
    CONTINUOUS_HIGH_RES_MODE  = 0x10,
    // Measurement at 0.5 lux resolution. Measurement time is approx 120ms.
    CONTINUOUS_HIGH_RES_MODE_2 = 0x11,
    // Measurement at 4 lux resolution. Measurement time is approx 16ms.
    CONTINUOUS_LOW_RES_MODE = 0x13,
    // Measurement at 1 lux resolution. Measurement time is approx 120ms.
    ONE_TIME_HIGH_RES_MODE = 0x20,
    // Measurement at 0.5 lux resolution. Measurement time is approx 120ms.
    ONE_TIME_HIGH_RES_MODE_2 = 0x21,
    // Measurement at 4 lux resolution. Measurement time is approx 16ms.
    ONE_TIME_LOW_RES_MODE = 0x23
}measurement_mode_t;

void bh1750_init(measurement_mode_t measurement_mode,uint8_t mt_reg);
int bh1750_set_measurement_mode( measurement_mode_t measurement_mode);
int bh1750_set_mt_reg(const uint8_t  mt_reg);
float bh1750_read_light_level(uint8_t  maxWait);


#endif
