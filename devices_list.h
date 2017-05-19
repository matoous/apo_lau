/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz
      license:  MIT

 *******************************************************************/

#ifndef APO_LAU_DEVICES_LIST_H
#define APO_LAU_DEVICES_LIST_H

#define DEFAULT_DEVICES_LIST_SIZE 64

#include "light_admin_unit.h"
#include <netinet/in.h>

typedef struct sockaddr_in sockaddr_in;

/***
 * socket address : light admin unit pair struct
 */
typedef struct device_addr_pair_t{
    sockaddr_in first;
    lau_t second;
} device_addr_pair_t;

/***
 * Devices list struct
 */
typedef struct devices_list_t{
    device_addr_pair_t* devices;
    unsigned int capacity;
    unsigned int devices_count;
} devices_list_t;

/***
 * init device list in provided pointer to devices_list_t
 * @param devices_list
 */
void dl_init(devices_list_t*);

/***
 * Push sock address : device pair on end of the devices list
 * @param devices_list | devices list
 * @param addr | sockaddr_in struct with device
 * @param lau | light admin unit
 * @return 0 if failed, 1 if OK
 */
int dl_push_back(devices_list_t*, sockaddr_in, lau_t);

/***
 * Return count of devices in list as unsigned integer
 * @param devices_list | list of devices
 * @return count of devices as unsigned int
 */
unsigned int dl_size(devices_list_t*);

/***
 * Destroy the devices list struct
 * @param devices_list | pointer to devices list
 */
void dl_destroy(devices_list_t*);

/***
 * Delete device from list
 * @param devices_list | list of devices
 * @param index | index of device to be deleted
 */
void dl_delete(devices_list_t*, unsigned int);

#endif //APO_LAU_DEVICES_LIST_H
