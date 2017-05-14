#include "pixel.h"

pixel_t pixel(const uint16_t color){
    pixel_t pix;
    pix.r = (color & RED_PART) >> 11;
    pix.g = (color & GREEN_PART) >>6;
    pix.b = (color & BLUE_PART);
}

uint16_t pix_to_uint16_t(const pixel_t pix){
    return ((((uint16_t)pix.r)<<8)&RED_PART) | ((((uint16_t)pix.g)<<3)&GREEN_PART) | ((((uint16_t)pix.b>>3))&BLUE_PART);
}