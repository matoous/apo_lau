/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz
      license:  MIT

 *******************************************************************/

#ifndef APO_LAU_DISPLAY_H
#define APO_LAU_DISPLAY_H

#define WHITE 0xFFFF
#define BLACK 0x0000
#define DEFAULT_BACKGROUND 0x39E7
#define DEFAULT_FONT_COLOR 0xCEDF
#define DEFAULT_SELECTED_FONT_COLOR 0xB006
#define DEFAULT_SELECTED_BACKGROUND_COLOR 0xD69A
#include <vector>
#include <utility>
#include <stdint.h>
#include <mutex>
#include <netinet/in.h>
#include "mzapo_phys.h"
#include "mzapo_parlcd.h"
#include "light_admin_unit.h"
#include "socket_rocket.h"

typedef struct board_t{
    uint16_t display_data[320][480];
} board_t;

void par_lcder(lau_t*, std::vector<std::pair<sockaddr_in, lau_t>>*, char*, int*, std::mutex*, std::mutex*);

void draw(lau_t, int, unsigned char*);

#endif //APO_LAU_DISPLAY_H
