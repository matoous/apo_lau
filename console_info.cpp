//
// Created by matoous on 25.04.17.
//

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include "console_info.h"
#include "light_admin_unit.h"

using namespace std;

void console_info(lau_t* lu, std::vector<std::pair<sockaddr_in, lau_t>>* devices, char* run, std::mutex* devices_mutex){
    unique_lock<mutex> devices_lock(*devices_mutex);
    devices_lock.unlock();
    while(*run){
        if(system("@cls||clear") < 0)
            printf("ugly output\n");
        printf("Unit name: %s\n", lu->name);
        printf("Ceiling color: %hu %hu %hu\n",
               (unsigned short)lu->ceiling_color.r,
               (unsigned short)lu->ceiling_color.g,
               (unsigned short)lu->ceiling_color.b);
        printf("Walls color: %hu %hu %hu\n",
               (unsigned short)lu->walls_color.r,
               (unsigned short)lu->walls_color.g,
               (unsigned short)lu->walls_color.b);
        printf("Connected devices: \n");
        devices_lock.lock();
        for(auto const&x : *devices){
            printf("%u : ", x.first.sin_addr.s_addr);
            printf("%s", x.second.name);
            if(strcmp(x.second.name, lu->name) == 0)
                printf(" (me)\n");
            else
                printf("\n");
        }
        devices_lock.unlock();
        printf("\n");
        sleep(5);
    }
    printf("Ending console info display...\n");
}