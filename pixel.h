#pragma once
#include <stdint.h>
#include <string>
class Pixel {
    public:
        const uint16_t green = 0x7E0;
        const uint16_t blue =  0x1F;
        const uint16_t red =   0xF800;
        Pixel() : r(0), g(0), b(0) {}
        Pixel(const uint16_t rgb565);
        Pixel(const uint8_t r, const uint8_t g, const uint8_t b);
        uint8_t r;
        uint8_t g;
        uint8_t b;
        operator uint16_t() const;
        operator std::string() const;
        uint16_t asRGB565() {return (uint16_t)*this;}
};
