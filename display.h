//
// Created by matoous on 26.04.17.
//

#ifndef APO_LAU_DISPLAY_H
#define APO_LAU_DISPLAY_H

#define WHITE 0xFFFF
#define BLACK 0x0000
#include <vector>
#include <utility>
#include <stdint.h>
#include <mutex>
#include "mzapo_phys.h"
#include "mzapo_parlcd.h"
#include "light_admin_unit.h"
#include "socket_rocket.h"

void par_lcder(lau_t*, std::vector<std::pair<unsigned int, lau_t>>*, char*, int, std::mutex*, std::mutex*);

void draw(lau_t, int, unsigned char*);

#endif //APO_LAU_DISPLAY_H
