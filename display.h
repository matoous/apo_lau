//
// Created by matoous on 26.04.17.
//

#ifndef APO_LAU_DISPLAY_H
#define APO_LAU_DISPLAY_H

#include <vector>
#include <utility>
#include <stdint.h>
#include "mzapo_phys.h"
#include "mzapo_parlcd.h"
#include "light_admin_unit.h"
#include "socket_rocket.h"

typedef struct display_t{
    uint8_t* data;
    uint16_t rows;
    uint16_t columns;
};

void init(lau_t*, std::vector<std::pair<unsigned int, lau_t>>*, char*);

void next_device();

void prev_device();

void next_color();

void prev_color();

#endif //APO_LAU_DISPLAY_H
