/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz
      license:  MIT

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "passer.h"
#include "socket_rocket.h"
#include "light_admin_unit.h"
#include "console_info.h"
#include "display.h"
#include "mzapo_parlcd.h"
#include "devices_list.h"
#include "mzapo_regs.h"


pthread_t udp_listener, udp_updater, parlcd_disp, console_disp;

void _start_threads(passer_t* passer){
    // start individual threads
    pthread_create(&udp_listener, NULL, sr_init, passer);
    pthread_create(&udp_updater, NULL, sr_updater, passer);
    pthread_create(&parlcd_disp, NULL, par_lcder, passer);
    pthread_create(&console_disp, NULL, console_info, passer);
}

void _stop_threads(){
    if(pthread_join(udp_listener, NULL)){
        fprintf(stderr, "ERROR joining udp_listener thread.\n");
    }
    if(pthread_join(udp_updater, NULL)){
        fprintf(stderr, "ERROR joining udp_updater thread.\n");
    }
    if(pthread_join(parlcd_disp, NULL)){
        fprintf(stderr, "ERROR joining parlcd_disp thread.\n");
    }
    if(pthread_join(console_disp, NULL)){
        fprintf(stderr, "ERROR joining console_disp thread.\n");
    }
}


int _load_lau(const char* file_name, lau_t* lu){
    // Read configuration
    FILE* conf_file = fopen(file_name, "r+");
    if(conf_file == NULL) {
        printf("ERROR opening configuration file\n");
        return 1;
    }

    // Load unit name
    char* name = (char*)calloc(17,1);
    if(!fscanf(conf_file, "%s", name)){
        printf("No unit name provided. Exiting...\n");
        return 1;
    }
    lu->name = name;
    // Load initial color
    pixel_t cl;
    if(!fscanf(conf_file, "%hhu %hhu %hhu", &cl.r, &cl.g, &cl.b)){
        printf("No initial ceiling color provided.\n");
        return 1;
    }
    lu->ceiling_color = cl;

    // Load initial color
    pixel_t wl;
    if(!fscanf(conf_file, "%hhu %hhu %hhu", &wl.r, &wl.g, &wl.b)){
        printf("No initial walls color provided.\n");
        return 1;
    }
    lu->walls_color = wl;

    // Load unit icon
    lu->icon = (uint16_t*)calloc(256, sizeof(uint16_t));
    for(int i = 0; i < 256; i++)
        if(!fscanf(conf_file, "%hu", &lu->icon[i])){
            printf("Error reading icon!\n");
            return 1;
        }

    // Close file
    fclose(conf_file);

    return 0;
}


int main(int argc, char *argv[])
{
    // check if configuration file is provided
    if(argc < 2){
        printf("No configuration file provided. Exiting...\n");
        return 1;
    }

    // load initial configuration
    lau_t lu;
    if(_load_lau(argv[1], &lu)){
        printf("Configuration error\n");
        return 1;
    }

    // Application start
    sockaddr_in me;

    // Init devices list
    devices_list_t devices_list;
    dl_init(&devices_list);
    dl_push_back(&devices_list, me, lu);

    // Debug
    printf("Starting application...\n\n");

    // run while
    char run = 1;
    int sockfd;

    // init mutexes
    pthread_mutex_t devices_mutex,  local_lau_mutex;
    if(pthread_mutex_init(&devices_mutex, NULL) != 0){
        printf("Error initializing devices mutex.");
    }
    if(pthread_mutex_init(&local_lau_mutex, NULL) != 0){
        printf("Error initializing local lau mutex.");
    }

    // create passer
    passer_t passer = {
            .devices = &devices_list,
            .devices_mutex = &devices_mutex,
            .local_lau = &lu,
            .local_lau_mutex = &local_lau_mutex,
            .run = &run,
            .sockfd = &sockfd, // this work in C not in CPP, remake later
    };

    // start individual threads
    _start_threads(&passer);

    // wait for input
    getchar();

    // end application
    run = 0;

    // stop threads
    _stop_threads();

    // close socket
    close(sockfd);

    // free devices list
    dl_destroy(&devices_list);

    // exit
    printf("Ending application...\n");
    return 0;
}
