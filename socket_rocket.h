/************************************************
 *
 * By Matous Dzivjak
 *
 ************************************************/

#ifndef APO_LAU_SOCKET_ROCKET_H
#define APO_LAU_SOCKET_ROCKET_H

#include <netinet/in.h>
#include <vector>
#include <utility>
#include <mutex>
#include "light_admin_unit.h"

#define SOCK_PORT 55555
#define ALC_CONTROL_NUM 0x414C4331UL
#define ALC_PROTOCOL_VER 0x00010000UL
#define ALC_MESSAGE_STATUS 0UL
#define ALC_MESSAGE_MODIFY 1UL
#define ALC_MESSAGE_SET 2UL

void sr_updater(const lau_t*, const int* const, char*, std::mutex*);

void sr_init(light_unit_t*, std::vector<std::pair<uint32_t, lau_t>>*, int*, char*, std::mutex*, std::mutex*);

void send_modify(
        int, // socket
        sockaddr_in*, // address
        int16_t, int16_t, int16_t, // ceiling
        int16_t, int16_t, int16_t); // walls

void send_set(
        int, // socket
        sockaddr_in*, // address
        int16_t, int16_t, int16_t, // ceiling
        int16_t, int16_t, int16_t); // walls

#endif //APO_LAU_SOCKET_ROCKET_H
