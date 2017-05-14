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

    FILE* conf_file = fopen(argv[1], "r+");
    if(!conf_file){
        printf("Couldn't read configuration file. Exiting...\n");
        exit(1);
    }

    // load unit name
    char name[17];
    if(!fscanf(conf_file, "%s", name)){
        printf("No unit name provided. Exiting...\n");
        exit(1);
    }
    lu.name = name;

    // load initial color
    pixel_t px;
    if(!fscanf(conf_file, "%hhu %hhu %hhu", &px.r, &px.g, &px.b)){
        printf("No initial ceiling color provided. Setting to default (white).\n");
        px.r = px.g = px.b = 255;
    }
    lu.ceiling_color = px;

    // load initial color
    if(!fscanf(conf_file, "%hhu %hhu %hhu", &px.r, &px.g, &px.b)){
        printf("No initial walls color provided. Setting to default (white).\n");
        px.r = px.g = px.b = 255;
    }
    lu.walls_color = px;

    lu.icon = new uint16_t[256];
    for(int i = 0; i < 256; i++)
        if(!fscanf(conf_file, "%hu", &lu.icon[i]))
            printf("Error reading icon!\n");

    fclose(conf_file);

    // print config
    printf("Unit name: %s\n", lu.name);
    printf("Ceiling color: %hu %hu %hu\n",
           (unsigned short)lu.ceiling_color.r,
           (unsigned short)lu.ceiling_color.g,
           (unsigned short)lu.ceiling_color.b);
    printf("Walls color: %hu %hu %hu\n",
           (unsigned short)lu.walls_color.r,
           (unsigned short)lu.walls_color.g,
           (unsigned short)lu.walls_color.b);
    for(int i = 0; i < 256; i++)
        cout << lu.icon[i] << " ";

    /***
     * Start running
     */
    // devices list
    sockaddr_in ahoj;
    vector<pair<sockaddr_in, lau_t>> devices;
    devices.push_back(pair<sockaddr_in, lau_t>(ahoj,lu));

    printf("Starting application...\n");

    sleep(2);
    if(system("@cls||clear") < 0)
        printf("Console clearing error, you will see ugly console output.\n");

    // run while
    char run = 1;
    int sockfd;

    // start individual threads
    std::thread udp_listener(sr_init, &lu, &devices, &sockfd, &run, &local_lau_mutex, &devices_mutex);
    std::thread udp_updater(sr_updater, &lu, &sockfd, &run, &local_lau_mutex);
    std::thread parlcd_disp(par_lcder, &lu, &devices, &run, &sockfd, &local_lau_mutex, &devices_mutex);
    std::thread console_disp(console_info, &lu, &devices, &run, &devices_mutex);

    // wait for input
    getchar();

    // end application
    run = 0;
    udp_updater.join();
    console_disp.join();
    parlcd_disp.join();
    close(sockfd);

    printf("Ending application...\n");
    exit(0);
}
