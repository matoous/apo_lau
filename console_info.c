/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz
      license:  MIT

 *******************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "light_admin_unit.h"
#include "passer.h"
#include "devices_list.h"
#include "console_info.h"

/***
 * Runs in separate thread, show updates about current application state
 * @return NULL
 */
void *console_info(void* args){

    // load args struct
    passer_t arguments = *((passer_t*)args);

    // set local variables
    lau_t* lu = arguments.local_lau;
    devices_list_t* devices = arguments.devices;
    char* run = arguments.run;
    pthread_mutex_t* devices_mutex = arguments.devices_mutex;
    pthread_mutex_t* local_lau_mutex = arguments.local_lau_mutex;

    do{
        pthread_mutex_lock(local_lau_mutex);
        printf("\nUnit name: %s\n", lu->name);
        printf("Ceiling color: %hu %hu %hu\n",
               (unsigned short)lu->ceiling_color.r,
               (unsigned short)lu->ceiling_color.g,
               (unsigned short)lu->ceiling_color.b);
        printf("Walls color: %hu %hu %hu\n",
               (unsigned short)lu->walls_color.r,
               (unsigned short)lu->walls_color.g,
               (unsigned short)lu->walls_color.b);
        pthread_mutex_unlock(local_lau_mutex);

        // show connected devices
        pthread_mutex_lock(devices_mutex);
        printf("\nConnected devices: \n");
        for(unsigned int i = 0; i < dl_size(devices); i++){
            printf("%u : ", devices->devices[i].first.sin_addr.s_addr);
            printf("%s\n",  devices->devices[i].second.name);
        }
        printf("\n");
        pthread_mutex_unlock(devices_mutex);
    } while(*run && !sleep(5));

    // End
    printf("Ending console info display...\n");
    return NULL;
}
