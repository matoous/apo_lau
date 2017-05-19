/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz
      license:  MIT

 *******************************************************************/

#ifndef APO_LAU_SOCKET_ROCKET_H
#define APO_LAU_SOCKET_ROCKET_H

#include "passer.h"
#include "light_admin_unit.h"
#include "devices_list.h"

#define SOCK_PORT 55555  // port to run on
#define ALC_CONTROL_NUM 0x414C4331UL  // AL1 in 32bit number as specified by APO HW
#define ALC_PROTOCOL_VER 0x00010000UL  // 1 on 3rd bite in 32bit number, as specified by APO HW
#define ALC_MESSAGE_STATUS 0UL
#define ALC_MESSAGE_MODIFY 1UL
#define ALC_MESSAGE_SET 2UL

/***
 * Runs as threads
 * Sends update about local unit every one second
 * @param passer struct with all necessary arguments
 */
void *sr_updater(void*);

/***
 * Handles incoming messages, updates, etc.
 * @param passer struct with all necessary arguments
 */
void *sr_init(void*);

/***
 * Send set packet
 * @param sockfd | socket file descriptor
 * @param out_addr | address to send at
 * @param cr | change in ceiling red
 * @param cg | change in ceiling green
 * @param cb | change in ceiling blue
 * @param wr | change in walls red
 * @param wg | change in walls green
 * @param wb | change in walls blue
 */
void send_modify(
        int*, // socket
        sockaddr_in, // address
        int16_t, int16_t, int16_t, // ceiling
        int16_t, int16_t, int16_t); // walls

/***
 * Send set packet
 * @param sockfd | socket file descriptor
 * @param out_addr | address to send at
 * @param cr | ceiling red
 * @param cg | ceiling green
 * @param cb | ceiling blue
 * @param wr | walls red
 * @param wg | walls green
 * @param wb | walls blue
 */
void send_set(
        int*, // socket
        sockaddr_in, // address
        int16_t, int16_t, int16_t, // ceiling
        int16_t, int16_t, int16_t); // walls

#endif //APO_LAU_SOCKET_ROCKET_H
