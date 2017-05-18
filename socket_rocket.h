/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz
      license:  MIT

 *******************************************************************/

#ifndef APO_LAU_SOCKET_ROCKET_H
#define APO_LAU_SOCKET_ROCKET_H

#include <netinet/in.h>
#include <vector>
#include <utility>
#include <mutex>
#include "passer.h"
#include "light_admin_unit.h"

#define SOCK_PORT 55555  // port to run on
#define ALC_CONTROL_NUM 0x414C4331UL  // AL1 in 32bit number as specified by APO HW
#define ALC_PROTOCOL_VER 0x00010000UL  // 1 on 3rd bite in 32bit number, as specified by APO HW
#define ALC_MESSAGE_STATUS 0UL
#define ALC_MESSAGE_MODIFY 1UL
#define ALC_MESSAGE_SET 2UL

void *sr_updater(void*); // mutex for local unit

void *sr_init(void*); // mutex for local unit and vector of units

void send_modify(
        int*, // socket
        sockaddr_in, // address
        int16_t, int16_t, int16_t, // ceiling
        int16_t, int16_t, int16_t); // walls

void send_set(
        int*, // socket
        sockaddr_in, // address
        int16_t, int16_t, int16_t, // ceiling
        int16_t, int16_t, int16_t); // walls

#endif //APO_LAU_SOCKET_ROCKET_H
