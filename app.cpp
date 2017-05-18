/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz
      license:  MIT

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>
#include <iostream>
#include <netinet/in.h>
#include <utility>
#include <vector>
#include <thread>
#include "passer.h"
#include "socket_rocket.h"
#include "light_admin_unit.h"
#include "console_info.h"
#include "display.h"
#include "mzapo_parlcd.h"
#include "mzapo_regs.h"

/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz3
      license:  MIT

 *******************************************************************/

using namespace std;

int main(int argc, char *argv[])
{
    /***
     * Load configuration
     */
    lau_t lu;
    std::mutex devices_mutex;
    std::mutex local_lau_mutex;

    if(argc < 2){
        printf("No configuration file provided. Exiting...\n");
        exit(1);
    }

    // Read configuration
    FILE* conf_file = fopen(argv[1], "r+");
    if(!conf_file){
        printf("Couldn't read configuration file. Exiting...\n");
        exit(1);
    }

    // Load unit name
    char name[17];
    if(!fscanf(conf_file, "%s", name)){
        printf("No unit name provided. Exiting...\n");
        exit(1);
    }
    lu.name = name;

    // Load initial color
    pixel_t px;
    if(!fscanf(conf_file, "%hhu %hhu %hhu", &px.r, &px.g, &px.b)){
        printf("No initial ceiling color provided. Setting to default (white).\n");
        px.r = px.g = px.b = 255;
    }
    lu.ceiling_color = px;

    // Load initial color
    if(!fscanf(conf_file, "%hhu %hhu %hhu", &px.r, &px.g, &px.b)){
        printf("No initial walls color provided. Setting to default (white).\n");
        px.r = px.g = px.b = 255;
    }
    lu.walls_color = px;

    // Load unit icon
    lu.icon = new uint16_t[256];
    for(int i = 0; i < 256; i++)
        if(!fscanf(conf_file, "%hu", &lu.icon[i]))
            printf("Error reading icon!\n");

    fclose(conf_file);

    /***
     * Start running
     */
    sockaddr_in me;
    vector<pair<sockaddr_in, lau_t>> devices;
    devices.push_back(pair<sockaddr_in, lau_t>(me, lu));

    printf("Starting application...\n");

    sleep(2);
    if(system("@cls||clear") < 0)
        printf("Console clearing error, you will see ugly console output.\n");

    // run while
    char run = 1;
    int sockfd;

    passer_t passer;
    passer.devices = &devices;
    passer.devices_mutex = &devices_mutex;
    passer.local_lau = &lu;
    passer.local_lau_mutex = &local_lau_mutex;
    passer.run = &run;
    passer.sockfd = &sockfd;

    // start individual threads
    pthread_t udp_listener, udp_updater, parlcd_disp, console_disp;
    pthread_create(&udp_listener, NULL, sr_init, &passer);
    pthread_create(&udp_updater, NULL, sr_updater, &passer);
    pthread_create(&parlcd_disp, NULL, par_lcder, &passer);
    pthread_create(&console_disp, NULL, console_info, &passer);

    // wait for input
    getchar();

    // end application
    run = 0;
    close(sockfd);

    printf("Ending application...\n");
    exit(0);
}
