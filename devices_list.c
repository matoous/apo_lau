/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz
      license:  MIT

 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "devices_list.h"

void dl_init(devices_list_t* devices_list){
    devices_list->devices = (device_addr_pair_t*)calloc(DEFAULT_DEVICES_LIST_SIZE,sizeof(device_addr_pair_t));
    devices_list->capacity = DEFAULT_DEVICES_LIST_SIZE;
    devices_list->devices_count = 0;
}

int dl_push_back(devices_list_t* devices_list, sockaddr_in addr, lau_t lau){
    if(devices_list->devices_count == devices_list->capacity)
        return 0;

    device_addr_pair_t dapt = {
            .first = addr,
            .second = lau
    };
    devices_list->devices[devices_list->devices_count++] = dapt;
    return 1;
}

unsigned int dl_size(devices_list_t* devices_list){
    return devices_list->devices_count;
}

void dl_destroy(devices_list_t* devices_list){
    for(int i = 0; i < devices_list->devices_count; i++){
        free(devices_list->devices[i].second.name);
        free(devices_list->devices[i].second.icon);
    }
    free(devices_list->devices);
}

void dl_delete(devices_list_t* devices_list, unsigned int index){
    for(unsigned int i = index; i < dl_size(devices_list)-1; i++){
        devices_list->devices[i] = devices_list->devices[i+1];
    }
    devices_list->devices--;
}