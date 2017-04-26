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
#include "light_admin_unit.h"

#define SOCK_PORT 55555
#define ALC_CONTROL_NUM 0x414C4331UL
#define ALC_PROTOCOL_VER 0x00010000UL
#define ALC_MESSAGE_STATUS 0UL
#define ALC_MESSAGE_MODIFY 1UL
#define ALC_MESSAGE_SET 2UL
#define RUN 1

void sr_updater(const lau_t*, const int* const, char*);
void sr_init(light_unit_t*, std::vector<std::pair<unsigned long, lau_t>>*, char*, int*);
void send_modify(
        const int, // socket
        sockaddr_in*, // address
        int16_t, int16_t, int16_t, // ceiling
        int16_t, int16_t, int16_t); // walls
void send_set(
        const int, // socket
        sockaddr_in*, // address
        int16_t, int16_t, int16_t, // ceiling
        int16_t, int16_t, int16_t); // walls
void sr_stop();

#endif //APO_LAU_SOCKET_ROCKET_H
