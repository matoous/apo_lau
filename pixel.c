/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz
      license:  MIT

 *******************************************************************/

#include "pixel.h"

/***
 * constructs pixel from uint16_t color
 * @param color | uint16_t color
 * @return pixel
 */
pixel_t pixel(const uint16_t color){
    pixel_t pix;
    pix.r = (color & RED_PART) >> 11;
    pix.g = (color & GREEN_PART) >>6;
    pix.b = (color & BLUE_PART);
    return pix;
}