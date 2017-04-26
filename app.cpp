#define _POSIX_C_SOURCE 200112L

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>
#include <iostream>
#include <utility>
#include <vector>
#include <thread>
#include "socket_rocket.h"
#include "light_admin_unit.h"
#include "PPMReader.h"
#include "console_info.h"

/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz3
      license:  MIT

 *******************************************************************/

int main(int argc, char *argv[])
{
    /***
     * Load configuration
     */
    lau_t lu;

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
    Pixel px;
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

    // load unit icon
    char image_file[128];
    if(!fscanf(conf_file, "%s", image_file)){
        printf("No unit icon provided. Exiting...\n");
        exit(1);
    }

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
    printf("Icon: %s\n", image_file);

    PPMReader reader(image_file);
    uint16_t ic[256];
    for(int i = 0; i < 256; i++)
        ic[i] = reader.nextColor();
    lu.icon = ic;
    /***
     * END
     * Configuration loaded
     */

    /***
     * Start running
     */
    std::vector<std::pair<unsigned long, lau_t>> devices;

    printf("Starting application...\n");

    sleep(2);
    if(!system("@cls||clear")){
        printf("Console clearing error, you will see ugly console output.\n");
    }

    char run = 1;
    int sockfd;
    std::thread listener(sr_init, &lu, &devices, &run, &sockfd);
    std::thread updater(sr_updater, &lu, &sockfd, &run);
    std::thread console_display(console_info, &lu, &devices, &run);


    // wait for input
    getchar();

    // end application
    run = 0;
    updater.join();
    console_display.join();
    close(sockfd);
    printf("Ending application...\n");
    sleep(1);
    exit(0);
}
