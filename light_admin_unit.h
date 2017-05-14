/************************************************
 *
 * By Matous Dzivjak
 *
 ************************************************/

#ifndef APO_LAU_LIGHT_UNIT_H
#define APO_LAU_LIGHT_UNIT_H

#include "pixel.h"
#include <stdio.h>

typedef struct lau_t{
    pixel_t ceiling_color, walls_color;
    char* name;
    uint16_t* icon;
} light_unit_t;

#endif //APO_LAU_LIGHT_UNIT_H
