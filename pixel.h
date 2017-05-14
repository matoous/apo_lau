#include <stdint.h>
#define GREEN_PART 0x7E0
#define BLUE_PART 0x1F
#define RED_PART 0xF800

typedef struct pixel_t{
    uint8_t r, g, b;
} pixel_t;
