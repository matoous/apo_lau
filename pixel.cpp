#include "pixel.h"
#include <sstream>

Pixel::Pixel(const uint16_t rgb565)
    : r((rgb565&RED)>>11)
    , g((rgb565&GREEN)>>6)
    , b(rgb565&BLUE) {}
Pixel::Pixel(const uint8_t r, const uint8_t g, const uint8_t b)
    : r(r)
    , g(g)
    , b(b) {}

Pixel::operator uint16_t() const {
    return ((((uint16_t)r)<<8)&RED) | ((((uint16_t)g)<<3)&GREEN) | ((((uint16_t)b>>3))&BLUE);
}
Pixel::operator std::string() const {
    std::stringstream s;
    s<<"RGB("<<(int)r<<", "<<(int)g<<", "<<(int)b<<")";
    return s.str();
}
