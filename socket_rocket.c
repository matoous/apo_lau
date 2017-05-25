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
#include <time.h>
#include <pthread.h>
#include "devices_list.h"
#include "socket_rocket.h"

/***
 * Puts int into the buffer (converts it to Big-Endian)
 * @param x - number
 * @param buffer - buffer to populate
 * @param offset - offset in buffer
 */
void _int_tbetb(int x, char* buffer, int offset){
    x = htonl(x);
    *(buffer + offset) = (char)((x >> 24) & 0xFF);
    *(buffer + offset+1) = (char)((x >> 16) & 0xFF);
    *(buffer + offset+2) = (char)((x >> 8) & 0xFF);
    *(buffer + offset+3) = (char)(x & 0xFF);
}

/***
 * Puts uint32_t into the buffer (converts it to Big-Endian)
 * @param x - number
 * @param buffer - buffer to populate
 * @param offset - offset
 */
void _uint32_t_tbetb(uint32_t x, char* buffer, int offset){
    x = htonl(x);
    *(buffer + offset) = (char)((x >> 24) & 0xFF);
    *(buffer + offset+1) = (char)((x >> 16) & 0xFF);
    *(buffer + offset+2) = (char)((x >> 8) & 0xFF);
    *(buffer + offset+3) = (char)(x & 0xFF);
}

/***
 * Puts int16_t into the buffer (converts it to Big-Endian)
 * @param x - number
 * @param buffer - buffer to populate
 * @param offset - offset
 */
void _int16_t_tbetb(int16_t x, char* buffer, int offset){
    x = htons(x);
    *(buffer + offset) = (char)((x >> 8) & 0xFF);
    *(buffer + offset+1) = (char)(x & 0xFF);
}

/***
 * Puts color into the buffer
 * @param c - color
 * @param buffer - buffer to populate
 * @param offset - offset in buffer
 */
void _color_tbetb(pixel_t c, char* buffer, int offset){
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
        buffer[offset + i*2 + 1] = (char)((curr_value)&0xFF);
        buffer[offset + i*2] = (char)((curr_value >> 8)&0xFF);
    }
}

/***
 * Gets uint32_t from buffer
 * @param buf - buffer
 * @param offset - offset in buffer
 * @return uint32_t
 */
uint32_t _bt_uint32_t(char* buf, int offset){
    return ntohl(ntohl((uint32_t)(buf[offset] << 24) | (buf[offset+1] << 16) | (buf[offset+2] << 8) | buf[offset+3]));
}

/***
 * Gets color from buffer (should be saved as uint32_t, first byte empty, then red, green, blue)
 * @param buf - buffer
 * @param offset - color offset in buffer
 * @return pixel_t color
 */
pixel_t _bt_color(char* buf, int offset){
    pixel_t c;
    c.r = (uint8_t)buf[offset+1];
    c.g = (uint8_t)buf[offset+2];
    c.b = (uint8_t)buf[offset+3];
    return c;
}

/***
 * Gets light admin unit name from buffer
 * @param buf - buffer
 * @param offset - name offset in buffer
 * @param uname - array to populate
 */
void _bt_name(char* buf, int offset, char* uname){
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
    uint16_t curr_val;
    curr_val = (uint8_t )buf[offset] << 8;
    curr_val += (uint8_t )buf[offset+1];
    curr_val = ntohs(curr_val);
    int16_t return_val = (int16_t)curr_val;
    return return_val;
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
void _sr_modify_lau(lau_t* lu, char* buf, pthread_mutex_t* local_lau_mutex){
    pthread_mutex_lock(local_lau_mutex);
    lu->ceiling_color.r += _bt_int16_t(buf, 12);
    lu->ceiling_color.g += _bt_int16_t(buf, 14);
    lu->ceiling_color.b += _bt_int16_t(buf, 16);
    lu->walls_color.r   += _bt_int16_t(buf, 18);
    lu->walls_color.g   += _bt_int16_t(buf, 20);
    lu->walls_color.b   += _bt_int16_t(buf, 22);
    pthread_mutex_unlock(local_lau_mutex);
}

/***
 * Sets light admin unit to received status
 * @param lu - light unit
 * @param buf - buffer
 */
void _sr_set_lau(lau_t* lu, char* buf, pthread_mutex_t* local_lau_mutex){
    pthread_mutex_lock(local_lau_mutex);
    lu->ceiling_color.r = (uint8_t)_bt_uint16_t(buf, 12);
    lu->ceiling_color.g = (uint8_t)_bt_uint16_t(buf, 14);
    lu->ceiling_color.b = (uint8_t)_bt_uint16_t(buf, 16);
    lu->walls_color.r = (uint8_t)_bt_uint16_t(buf, 18);
    lu->walls_color.g = (uint8_t)_bt_uint16_t(buf, 20);
    lu->walls_color.b = (uint8_t)_bt_uint16_t(buf, 22);
    pthread_mutex_unlock(local_lau_mutex);
}

/***
 * Runs as threads
 * Sends update about local unit every one second
 * @param passer struct with all necessary arguments
 */
void *sr_updater(void* args){
    passer_t arguments = *((passer_t*)args);

    lau_t* lu = arguments.local_lau;
    char* run = arguments.run;
    pthread_mutex_t* local_lau_mutex = arguments.local_lau_mutex;
    int* sockfd = arguments.sockfd;

    int n;
    sockaddr_in broadcast;

    // buffer
    char* buffer = (char*)calloc(1024, 1);

    // setup broadcast
    broadcast.sin_family = AF_INET;
    broadcast.sin_port = htons(SOCK_PORT);
    broadcast.sin_addr.s_addr = INADDR_BROADCAST;

    unsigned int len = sizeof(sockaddr_in);

    _uint32_t_tbetb(htonl(ALC_CONTROL_NUM), buffer, 0);
    _uint32_t_tbetb(htonl(ALC_PROTOCOL_VER), buffer, 4);
    _uint32_t_tbetb(htonl(ALC_MESSAGE_STATUS), buffer, 8);

    // things that wont change
    pthread_mutex_lock(local_lau_mutex);
    _name_tbetb(lu->name, buffer, 20);
    _icon_tbetb(lu->icon, buffer, 36);
    pthread_mutex_unlock(local_lau_mutex);

    // send update every one second
    printf("Broadcasting status.\n");
    do {
        pthread_mutex_lock(local_lau_mutex);
        _color_tbetb(lu->ceiling_color, buffer, 12);
        _color_tbetb(lu->walls_color, buffer, 16);
        pthread_mutex_unlock(local_lau_mutex);

        n = sendto(*sockfd, buffer, 1024, 0,(const struct sockaddr *)&broadcast, len);
        if(n < 0)
            printf("Error sending update.\n");
        
    } while(*run && !sleep(1));

    free(buffer);
    printf("Ending update sender...\n");
    return NULL;
}

/***
 * Handles incoming messages, updates, etc.
 * @param passer struct with all necessary arguments
 */
void *sr_init(void* args) {
    passer_t arguments = *((passer_t*)args);

    // get arguments
    lau_t* lu = arguments.local_lau;
    char* run = arguments.run;
    int* sockfd = arguments.sockfd;
    devices_list_t* devices_list = arguments.devices;
    pthread_mutex_t* local_lau_mutex = arguments.local_lau_mutex;
    pthread_mutex_t* devices_mutex = arguments.devices_mutex;

    // init data
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
    if (bind(*sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0){
        fprintf(stderr, "ERROR binding socket, exiting.\n");
        return NULL;
    }

    // listen
    listen(*sockfd, 5);

    // socket length
    socklen_t clilen;
    clilen = sizeof(cli_addr);
    // main loop
    printf("listening on port 55555.\n");
    while (*run) {
        // select
        struct timeval timeout = { 3, 0 };
        fd_set in_set;
        FD_ZERO(&in_set);
        FD_SET(*sockfd, &in_set);
        if(select(*sockfd + 1, &in_set, NULL, NULL, &timeout) == -1){
            fprintf(stderr, "ERROR select error in listener.\n");
        }
        if (FD_ISSET(*sockfd, &in_set))
        {
            // receive datagram
            if (recvfrom(*sockfd, buf, 1024, 0, (struct sockaddr *)&cli_addr, &clilen) < 0)
                fprintf(stderr, "ERROR receiving message error.\n");

            // get basic datagram info
            uint32_t control_number = _bt_uint32_t(buf, 0);
            uint32_t version = _bt_uint32_t(buf, 4);
            uint32_t type = _bt_uint32_t(buf, 8);

            // if control number and protocol version is ok
            if(control_number == ALC_CONTROL_NUM && version == ALC_PROTOCOL_VER){
                // update datagram
                if(type == 0){

                    // debug
                    printf("%u update\n", cli_addr.sin_addr.s_addr);

                    char added = 0;

                    pthread_mutex_lock(devices_mutex);
                    // if it is me, update my ip addr
                    char name[17];
                    _bt_name(buf, 20, name);
                    if(strcmp(devices_list->devices[0].second.name, name) == 0)
                        devices_list->devices[0].first = cli_addr;

                    // run through all devices and update the one that should be updated
                    time_t now = time(NULL);
                    for(uint32_t i = 0; i < dl_size(devices_list); i++){
                        if(devices_list->devices[i].first.sin_addr.s_addr == cli_addr.sin_addr.s_addr){
                            pixel_t ceiling_color = _bt_color(buf, 12);
                            pixel_t walls_color = _bt_color(buf, 16);
                            devices_list->devices[i].second.ceiling_color = ceiling_color;
                            devices_list->devices[i].second.walls_color = walls_color;
                            devices_list->devices[i].second.last_update = time(NULL);
                            _bt_name(buf, 20, devices_list->devices[i].second.name);
                            _bt_icon(buf, 36, devices_list->devices[i].second.icon);
                            added = 1;
                            continue;
                        }
                        if(now - devices_list->devices[i].second.last_update > 20)
                            dl_delete(devices_list, i);
                    }
                    // if not updated the device, add a new one
                    if(!added){
                        lau_t curr_lu;
                        pixel_t ceiling_color = _bt_color(buf, 12);
                        pixel_t walls_color = _bt_color(buf, 16);
                        curr_lu.ceiling_color = ceiling_color;
                        curr_lu.walls_color = walls_color;
                        curr_lu.last_update = time(NULL);
                        curr_lu.name = (char*)calloc(17,1);
                        curr_lu.icon = (uint16_t*)malloc(256*sizeof(uint16_t));
                        _bt_name(buf, 20, curr_lu.name);
                        _bt_icon(buf, 36, curr_lu.icon);
                        dl_push_back(devices_list, cli_addr, curr_lu);
                    }
                    pthread_mutex_unlock(devices_mutex);
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
        else // timeout of receive
            break;
    }
    printf("Ending update listener...\n");
    return NULL;
}

/***
 * Send update packet
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
        int* sockfd, // socket
        sockaddr_in out_addr, // address
        int16_t cr, int16_t cg, int16_t cb,
        int16_t wr, int16_t wg, int16_t wb
) {
    int n;

    // buffer
    char* buffer = (char*)calloc(1024,1);

    unsigned int len = sizeof(out_addr);

    _uint32_t_tbetb(htonl(ALC_CONTROL_NUM), buffer, 0);
    _uint32_t_tbetb(htonl(ALC_PROTOCOL_VER), buffer, 4);
    _uint32_t_tbetb(htonl(ALC_MESSAGE_MODIFY), buffer, 8);

    _int16_t_tbetb(cr, buffer, 12);
    _int16_t_tbetb(cg, buffer, 14);
    _int16_t_tbetb(cb, buffer, 16);

    _int16_t_tbetb(wr, buffer, 18);
    _int16_t_tbetb(wg, buffer, 20);
    _int16_t_tbetb(wb, buffer, 22);

    n = sendto(*sockfd, buffer, 1024, 0,(const struct sockaddr *)&out_addr, len);
    if(n < 0)
        fprintf(stderr, "ERROR sending modify.\n");

    free(buffer);
}

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
        int *sockfd, // socket
        sockaddr_in out_addr, // address
        int16_t cr, int16_t cg, int16_t cb,
        int16_t wr, int16_t wg, int16_t wb
) {
    int n;

    // buffer
    char* buffer = (char*)calloc(1024,1);

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

    n = sendto(*sockfd, buffer, 1024, 0,(const struct sockaddr *)&out_addr, len);
    if(n < 0)
        fprintf(stderr, "ERROR sending set.\n");

    free(buffer);
}
