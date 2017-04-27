//
// Created by matoous on 25.04.17.
//

#ifndef APO_LAU_CONSOLE_INFO_H
#define APO_LAU_CONSOLE_INFO_H

#include "light_admin_unit.h"
#include <vector>
#include <utility>
#include <stdint.h>
#include <mutex>

void console_info(lau_t*, std::vector<std::pair<uint32_t, lau_t>>*, char*, std::mutex*);

#endif //APO_LAU_CONSOLE_INFO_H
