/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz
      license:  MIT

 *******************************************************************/

#include <stdint.h>

#define GREEN_PART 0x7E0
#define BLUE_PART 0x1F
#define RED_PART 0xF800

/***
 * struct for holding rgb color
 */
typedef struct pixel_t{
    uint8_t r, g, b;
} pixel_t;

/***
 * constructs pixel from uint16_t color
 * @param color | uint16_t color
 * @return pixel
 */
pixel_t pixel(const uint16_t color);