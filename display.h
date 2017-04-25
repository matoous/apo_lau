//
// Created by matoous on 26.04.17.
//

#ifndef APO_LAU_DISPLAY_H
#define APO_LAU_DISPLAY_H

#include "mzapo_phys.h"
#include "mzapo_parlcd.h"
#include "light_admin_unit.h"
#include "socket_rocket.h"
#include <map>

void init(&lau_t, &std::map<unsigned int, lau_t>);

void next_device();

void prev_device();

void next_color();

void prev_color();

#endif //APO_LAU_DISPLAY_H
