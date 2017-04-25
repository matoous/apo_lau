//
// Created by matoous on 25.04.17.
//

#include <stdio.h>
#include "console_info.h"
#include "light_admin_unit.h"
#include <unistd.h>
#include <iostream>
#include <string.h>

void console_info(lau_t* lu, std::map<unsigned long, lau_t>* devices_map){
    while(1){
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
        for(auto &x : *devices_map){
            printf("%lu : ", x.first);
            printf("%s", x.second.name);
            if(strcmp(x.second.name, lu->name) == 0)
                printf(" (me)\n");
            else
                printf("\n");
        }
        printf("\n");
        sleep(5);
    }
}