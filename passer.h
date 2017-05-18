//
// Created by matoous on 18.05.17.
//

#ifndef APO_LAU_PASSER_T_H
#define APO_LAU_PASSER_T_H

#include <thread>
#include <vector>
#include <mutex>
#include <netinet/in.h>
#include "light_admin_unit.h"

typedef struct passer_t{
    lau_t* local_lau;
    std::vector<std::pair<sockaddr_in, lau_t>>* devices;
    char* run;
    int* sockfd;
    std::mutex* devices_mutex;
    std::mutex* local_lau_mutex;
} passer_t;

#endif //APO_LAU_PASSER_T_H
