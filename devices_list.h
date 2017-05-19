/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz
      license:  MIT

 *******************************************************************/

#ifndef APO_LAU_DEVICES_LIST_H
#define APO_LAU_DEVICES_LIST_H

#define DEFAULT_DEVICES_LIST_SIZE 64

#include "light_admin_unit.h"
#include <netinet/in.h>

typedef struct sockaddr_in sockaddr_in;

typedef struct device_addr_pair_t{
    sockaddr_in first;
    lau_t second;
} device_addr_pair_t;

typedef struct devices_list_t{
    device_addr_pair_t* devices;
    unsigned int capacity;
    unsigned int devices_count;
} devices_list_t;

void dl_init(devices_list_t*);

int dl_push_back(devices_list_t*, sockaddr_in, lau_t);

unsigned int dl_size(devices_list_t*);

void dl_destroy(devices_list_t*);

void dl_delete(devices_list_t*, unsigned int);

#endif //APO_LAU_DEVICES_LIST_H
