/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz
      license:  MIT

 *******************************************************************/

#ifndef APO_LAU_PASSER_T_H
#define APO_LAU_PASSER_T_H

#include <netinet/in.h>
#include "devices_list.h"
#include "light_admin_unit.h"

/***
 * struct used for passing arguments to pthreads
 */
typedef struct passer_t{
    lau_t* local_lau;
    devices_list_t* devices;
    char* run;
    int* sockfd;
    pthread_mutex_t* devices_mutex;
    pthread_mutex_t* local_lau_mutex;
} passer_t;

#endif //APO_LAU_PASSER_T_H
