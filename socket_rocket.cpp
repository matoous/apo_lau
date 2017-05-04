/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz
      license:  MIT

 *******************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include "socket_rocket.h"

using namespace std;

/***
 * Puts int into the buffer (converts it to Big-Endian)
 * @param x - number
 * @param buffer - buffer to populate
 * @param offset - offset in buffer
 */
void _int_tbetb(int x, char* buffer, int offset){
    x = htonl(x);
    *(buffer + offset) = (x >> 24) & 0xFF;
    *(buffer + offset+1) = (x >> 16) & 0xFF;
    *(buffer + offset+2) = (x >> 8) & 0xFF;
    *(buffer + offset+3) = x & 0xFF;
}

/***
 * Puts uint32_t into the buffer (converts it to Big-Endian)
 * @param x - number
 * @param buffer - buffer to populate
 * @param offset - offset
 */
void _uint32_t_tbetb(uint32_t x, char* buffer, int offset){
    x = htonl(x);
    *(buffer + offset) = (x >> 24) & 0xFF;
    *(buffer + offset+1) = (x >> 16) & 0xFF;
    *(buffer + offset+2) = (x >> 8) & 0xFF;
    *(buffer + offset+3) = x & 0xFF;
}

/***
 * Puts int16_t into the buffer (converts it to Big-Endian)
 * @param x - number
 * @param buffer - buffer to populate
 * @param offset - offset
 */
void _int16_t_tbetb(int16_t x, char* buffer, int offset){
    x = htonl(x);
    *(buffer + offset) = (x >> 8) & 0xFF;
    *(buffer + offset+1) = x & 0xFF;
}

/***
 * Puts color into the buffer
 * @param c - color
 * @param buffer - buffer to populate
 * @param offset - offset in buffer
 */
void _color_tbetb(Pixel c, char* buffer, int offset){
    *(buffer + offset) = 0;
    *(buffer + offset+1) = c.r;
    *(buffer + offset+2) = c.g;
    *(buffer + offset+3) = c.b;
}

/***
 * Puts light admin unit name into the buffer
 * @param c - light admin unit name
 * @param buffer - buffer to populate
 * @param offset - offset in buffer
 */
void _name_tbetb(const char c[], char* buffer, int offset){
    int i = 0;
    while(i < 16 && c[i] != '\0'){
        buffer[i+offset] = c[i];
        i++;
    }
    if(i < 16){
        buffer[offset + i++] = '\0';
        while(i < 16){
            buffer[offset + i++] = 0;
        }
    }
}

/***
 * Puts light admin unit icon into the buffer on given offset, converts to Big-Endian
 * @param icon - icon array
 * @param buffer - buffer to populate
 * @param offset - offset in buffer
 */
void _icon_tbetb(const uint16_t icon[], char* buffer, int offset){
    uint16_t curr_value;
    for(int i = 0; i < 256; i++){
        curr_value = htons(icon[i]);
        buffer[offset + i*2 + 1] = (curr_value)&0xFF;
        buffer[offset + i*2] = (curr_value >> 8)&0xFF;
    }
}

/***
 * Gets uint32_t from buffer
 * @param buf - buffer
 * @param offset - offset in buffer
 * @return uint32_t
 */
uint32_t _bt_uint32_t(char* buf, int offset){
    return ntohl((uint32_t)ntohl((buf[offset] << 24) | (buf[offset+1] << 16) | (buf[offset+2] << 8) | buf[offset+3]));
}

/***
 * Gets color from buffer (should be saved as uint32_t, first byte empty, then red, green, blue)
 * @param buf - buffer
 * @param offset - color offset in buffer
 * @return Pixel color
 */
Pixel _bt_color(char* buf, int offset){
    Pixel c;
    c.r = buf[offset+1];
    c.g = buf[offset+2];
    c.b = buf[offset+3];
    return c;
}

/***
 * Gets light admin unit name from buffer
 * @param buf - buffer
 * @param offset - name offset in buffer
 * @param uname - array to populate
 */
void _bt_name(char* buf, int offset, char uname[17]){
    int index = 0;
    while(index < 16 && buf[index + offset] != '\0'){
        uname[index] = buf[index + offset];
        index++;
    }
    uname[index] = '\0';
}

/***
 * Gets uint16_t from buffer
 * @param buf - buffer
 * @param offset - offset in buffer
 * @return uint16_t
 */
uint16_t _bt_uint16_t(char* buf, int offset){
    uint16_t curr_val;
    curr_val = (uint8_t )buf[offset] << 8;
    curr_val += (uint8_t )buf[offset+1];
    curr_val = ntohs(curr_val);
    return curr_val;
}

/***
 * Gets uint16_t from buffer
 * @param buf - buffer
 * @param offset - offset in buffer
 * @return uint16_t
 */
int16_t _bt_int16_t(char* buf, int offset){
    int16_t curr_val;
    curr_val = (int8_t )buf[offset] << 8;
    curr_val += (int8_t )buf[offset+1];
    curr_val = ntohs(curr_val);
    return curr_val;
}

/***
 * Gets light admin unit icon from buffer
 * @param buf - buffer
 * @param offset - icon offset in buffer
 * @param icon - array to populate
 */
void _bt_icon(char* buf, int offset, uint16_t icon[256]){
    uint16_t curr_val;
    for(int i = 0; i < 256; i++){
        curr_val = (uint8_t )buf[i*2+offset] << 8;
        curr_val += (uint8_t )buf[i*2+offset+1];
        curr_val = ntohs(curr_val);
        icon[i] = curr_val;
    }
}

/***
 * Updates light admin unit
 * @param lu - light admin unit
 * @param buf - buffer
 */
void _sr_modify_lau(lau_t* lu, char* buf, mutex* local_lau_mutex){
    lock_guard<mutex> lu_lock(*local_lau_mutex);
    lu->ceiling_color.r += _bt_int16_t(buf, 12);
    lu->ceiling_color.g += _bt_int16_t(buf, 14);
    lu->ceiling_color.b += _bt_int16_t(buf, 16);
    lu->walls_color.r   += _bt_int16_t(buf, 18);
    lu->walls_color.g   += _bt_int16_t(buf, 20);
    lu->walls_color.b   += _bt_int16_t(buf, 22);
}

/***
 * Sets light admin unit to received status
 * @param lu - light unit
 * @param buf - buffer
 */
void _sr_set_lau(lau_t* lu, char* buf, mutex* local_lau_mutex){
    lock_guard<mutex> lu_lock(*local_lau_mutex);
    lu->ceiling_color.r = _bt_uint16_t(buf, 12);
    lu->ceiling_color.g = _bt_uint16_t(buf, 14);
    lu->ceiling_color.b = _bt_uint16_t(buf, 16);
    lu->walls_color.r = _bt_uint16_t(buf, 18);
    lu->walls_color.g = _bt_uint16_t(buf, 20);
    lu->walls_color.b = _bt_uint16_t(buf, 22);
}

/***
 * This method should be run as thread
 * Sends update about local unit every one second
 * @param lu - pointer to lau_t representing local unit
 * @param sockfd - memory address of socket
 */
void sr_updater(const lau_t* lu, const int* const sockfd, char* run, mutex* local_lau_mutex)
{
    int n;
    sockaddr_in broadcast;

    // buffer
    char* buffer = (char*)malloc(1024);
    bzero(buffer,1024);

    // setup broadcast
    broadcast.sin_family = AF_INET;
    broadcast.sin_port = htons(SOCK_PORT);
    broadcast.sin_addr.s_addr = INADDR_BROADCAST;

    unsigned int len = sizeof(sockaddr_in);

    _uint32_t_tbetb(htonl(ALC_CONTROL_NUM), buffer, 0);
    _uint32_t_tbetb(htonl(ALC_PROTOCOL_VER), buffer, 4);
    _uint32_t_tbetb(htonl(ALC_MESSAGE_STATUS), buffer, 8);

    // things that wont change
    unique_lock<mutex> lu_locker(*local_lau_mutex);
    _name_tbetb(lu->name, buffer, 20);
    _icon_tbetb(lu->icon, buffer, 36);
    lu_locker.unlock();

    // send update every one second
    printf("Broadcasting status.\n");
    while(*run){
        lu_locker.lock();
        _color_tbetb(lu->ceiling_color, buffer, 12);
        _color_tbetb(lu->walls_color, buffer, 16);
        lu_locker.unlock();

        n = sendto(*sockfd, buffer, 1024, 0,(const struct sockaddr *)&broadcast, len);
        if(n < 0){
            printf("Error sending update.\n");
        }
        sleep(1);
    }
    printf("Ending update sender...\n");
}

/***
 * Handles incoming messages, updates, etc.
 * @param lu - pointer to lau_t representing local unit
 */
void sr_init(lau_t* lu, std::vector<std::pair<uint32_t, lau_t>>* devices, int* sockfd, char* run, mutex* local_lau_mutex, mutex* devices_mutex) {
    struct sockaddr_in my_addr, cli_addr;
    char buf[1024];

    // init socket
    *sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // enable broadcast
    int enabled = 1;
    setsockopt(*sockfd, SOL_SOCKET, SO_BROADCAST, &enabled, sizeof(enabled));

    // setup server
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(SOCK_PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    // bind socket
    if (bind(*sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0)
        printf("ERROR on binding");

    // listen
    listen(*sockfd, 5);

    // socket length
    socklen_t clilen;
    clilen = sizeof(cli_addr);

    // main loop
    printf("listening on port 55555.\n");
    while (*run) {
        // receive datagram
        if (recvfrom(*sockfd, buf, 1024, 0, (struct sockaddr *)&cli_addr, &clilen) < 0)
            printf("receive error");

        // get basic datagram info
        uint32_t control_number = _bt_uint32_t(buf, 0);
        uint32_t version = _bt_uint32_t(buf, 4);
        uint32_t type = _bt_uint32_t(buf, 8);

        if(control_number == ALC_CONTROL_NUM && version == ALC_PROTOCOL_VER){
            // update datagram
            if(type == 0){

                lau_t curr_lu;

                Pixel ceiling_color = _bt_color(buf, 12);
                Pixel walls_color = _bt_color(buf, 16);

                curr_lu.ceiling_color = ceiling_color;
                curr_lu.walls_color = walls_color;
                curr_lu.name = new char[17];
                curr_lu.icon = new uint16_t[256];

                _bt_name(buf, 20, curr_lu.name);
                _bt_icon(buf, 36, curr_lu.icon);

                printf("%s (%u) update\n", curr_lu.name, cli_addr.sin_addr.s_addr);

                char added = 0;
                unique_lock<mutex> devices_lock(*devices_mutex);
                if(strcmp((*devices)[0].second.name, curr_lu.name) == 0)
                    (*devices)[0].first = cli_addr.sin_addr.s_addr;
                for(uint32_t i = 0; i < (*devices).size(); i++){
                    if((*devices)[i].first == cli_addr.sin_addr.s_addr){
                        (*devices)[i].second = curr_lu;
                        added = 1;
                        break;
                    }
                }
                if(!added)
                    (*devices).push_back(pair<uint32_t, lau_t>(cli_addr.sin_addr.s_addr, curr_lu));
                devices_lock.unlock();
            }
            else if(type == 1){
                printf("received modify packet\n");
                _sr_modify_lau(lu, buf, local_lau_mutex);
            }
            else if(type == 2){
                printf("received set packet\n");
                _sr_set_lau(lu, buf, local_lau_mutex);
            }
        }
    }

    printf("Ending update listener...\n");
}

/***
 * Send update packet
 * @param sockfd
 * @param out_addr
 * @param cr
 * @param cg
 * @param cb
 * @param wr
 * @param wg
 * @param wb
 */
void send_modify(
        int sockfd, // socket
        int out_addr, // address
        int16_t cr, int16_t cg, int16_t cb,
        int16_t wr, int16_t wg, int16_t wb
) {
    int n;

    sockaddr_in send_to;
    send_to.sin_family = AF_INET;
    send_to.sin_port = htons(SOCK_PORT);
    send_to.sin_addr.s_addr = out_addr;

    // buffer
    char* buffer = (char*)malloc(1024);
    bzero(buffer,1024);

    unsigned int len = sizeof(out_addr);

    _uint32_t_tbetb(ALC_CONTROL_NUM, buffer, 0);
    _uint32_t_tbetb(ALC_PROTOCOL_VER, buffer, 4);

    _uint32_t_tbetb(ALC_MESSAGE_MODIFY, buffer, 8);

    _int16_t_tbetb(cr, buffer, 12);
    _int16_t_tbetb(cg, buffer, 14);
    _int16_t_tbetb(cb, buffer, 16);

    _int16_t_tbetb(wr, buffer, 18);
    _int16_t_tbetb(wg, buffer, 20);
    _int16_t_tbetb(wb, buffer, 22);

    printf("Sending modify.\n");
    n = sendto(sockfd, buffer, 1024, 0,(const struct sockaddr *)&send_to, len);
    if(n < 0)
        printf("Error sending modify.\n");
}

/***
 *
 * @param sockfd
 * @param out_addr
 * @param cr
 * @param cg
 * @param cb
 * @param wr
 * @param wg
 * @param wb
 */
void send_set(
        int sockfd, // socket
        int out_addr, // address
        int16_t cr, int16_t cg, int16_t cb,
        int16_t wr, int16_t wg, int16_t wb
) {
    int n;

    sockaddr_in send_to;
    send_to.sin_family = AF_INET;
    send_to.sin_port = htons(SOCK_PORT);
    send_to.sin_addr.s_addr = out_addr;

    // buffer
    char* buffer = (char*)malloc(1024);
    bzero(buffer,1024);

    unsigned int len = sizeof(out_addr);

    _uint32_t_tbetb(ALC_CONTROL_NUM, buffer, 0);
    _uint32_t_tbetb(ALC_PROTOCOL_VER, buffer, 4);

    _uint32_t_tbetb(ALC_MESSAGE_SET, buffer, 8);

    _int16_t_tbetb(cr, buffer, 12);
    _int16_t_tbetb(cg, buffer, 14);
    _int16_t_tbetb(cb, buffer, 16);

    _int16_t_tbetb(wr, buffer, 18);
    _int16_t_tbetb(wg, buffer, 20);
    _int16_t_tbetb(wb, buffer, 22);

    printf("Sending set.\n");
    n = sendto(sockfd, buffer, 1024, 0,(const struct sockaddr *)&send_to, len);
    if(n < 0)
        printf("Error sending set.\n");
}