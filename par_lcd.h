//
// Created by matoous on 20.04.17.
//
#define PARLCD_REG_BASE_PHYS 0x43c00000
#define PARLCD_REG_SIZE      0x00004000
#define PARLCD_REG_CMD_o                0x0008
#define PARLCD_REG_DATA_o               0x000C


#ifndef APO_LAU_PAR_LCD_H
#define APO_LAU_PAR_LCD_H

#endif //APO_LAU_PAR_LCD_H

#include "par_lcd.h"
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
#include <byteswap.h>
#include <getopt.h>
#include <inttypes.h>
void parlcd_hx8357_init(unsigned char*){}
void parlcd_write_cmd(unsigned char*, uint16_t){}
void parlcd_write_data(unsigned char*, uint16_t ){}
void parlcd_write_data2x(unsigned char*, uint32_t ){}
void parlcd_delay(int){}