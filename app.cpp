#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <byteswap.h>
#include <getopt.h>
#include <inttypes.h>
#include <time.h>
#include <cmath>
#include "utils/pixel.h"
#include "utils/PPMReader.h"
#include "perif/knobs.h"
#include "perif/lcd_display.h"
#include "perif/map_phys.h"

/*******************************************************************
  Simple program to demostrate binary reprezentation on MicroZed
  based MZ_APO board designed by Petr Porazil at PiKRON

  mzapo_binrep.c       - main and only file

  (C) Copyright 2004 - 2017 by Pavel Pisa
      e-mail:   pisa@cmp.felk.cvut.cz
      homepage: http://cmp.felk.cvut.cz/~pisa
      work:     http://www.pikron.com/
      license:  any combination GPL, LGPL, MPL or BSD licenses

 *******************************************************************/
typedef unsigned char byte;
int main(int argc, char *argv[])
{
    unsigned char *parlcd_mem_base;
    unsigned char *knobs_mem_base;

    /*
     * Setup memory mapping which provides access to the peripheral
     * registers region of RGB LEDs, knobs and line of yellow LEDs.
     */
    knobs_mem_base = (unsigned char*)map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if (knobs_mem_base == NULL){
        printf("Knobs allocation failes.\n");
        exit(1);
    }

    parlcd_mem_base = (unsigned char*)map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    if (parlcd_mem_base == NULL){
        printf("Display allocation failes.\n");
        exit(1);
    }
    parlcd_hx8357_init(parlcd_mem_base);

    PPMReader reader(argc>1?argv[1]:"rick.ppm");
    //parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (uint16_t y = 0; y < 320 ; y++) {
        for (uint16_t x = 0; x < 480 ; x++) {
            parlcd_write_data(parlcd_mem_base, reader.nextColor());
        }
    }

    unsigned char curr_LAU, curr_SC, curr_VAL, prev_VAL, new_SC, new_VAL;

    prev_VAL = 100;
    while(1){

        uint32_t rgb_knobs_value;
        int int_val;
        int num_of_stations = 16;
        unsigned int uint_val;

        struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 100 * 1000 * 1000};
        rgb_knobs_value = *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_KNOBS_8BIT_o);
        uint_val = rgb_knobs_value;
        curr_LAU = (unsigned char)(uint_val>>16)&0xFF;
        new_SC = (unsigned char)(uint_val>>8)&0xFF;
        new_VAL = (unsigned char)uint_val&0xFF;

        if(prev_VAL > 200 && new_VAL < 50 && prev_VAL != new_VAL){
            curr_VAL += 256-prev_VAL/4 + new_VAL/4;
            if(curr_VAL > 256)
                curr_VAL = 256;
            prev_VAL = new_VAL;
        }
        else if(prev_VAL < 50 && new_VAL > 200 && prev_VAL != new_VAL){
            curr_VAL  -= (prev_VAL/4 + 256 - new_VAL/4);
            if(curr_VAL < 0)
                curr_VAL = 0;
            prev_VAL = new_VAL;
        }
        else if(prev_VAL > curr_VAL){
            curr_VAL  -= (prev_VAL/4 - curr_VAL/4);
            if(curr_VAL < 0)
                curr_VAL = 0;
            prev_VAL = new_VAL;
        }
        else if(curr_VAL > prev_VAL){
            curr_VAL  += (curr_VAL/4 - prev_VAL/4);
            if(curr_VAL > 256)
                curr_VAL = 256;
            prev_VAL = new_VAL;
        }

        printf("station: %d color part: %d value: %d\n", (int)curr_LAU/4%num_of_stations, (int)new_SC%3, (int)curr_VAL);

        /* Store the read value to the register controlling individual LEDs */
        *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_LED_LINE_o) = 0;

        /*
         * Store RGB knobs values to the corersponding components controlling
         * a color/brightness of the RGB LEDs
         */
        *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_LED_RGB1_o) = 0;

        *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_LED_RGB2_o) = 0;


        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    }

  return 0;
}
