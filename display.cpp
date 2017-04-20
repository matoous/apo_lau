/*******************************************************************
  Simple program to check LCD functionality on MicroZed
  based MZ_APO board designed by Petr Porazil at PiKRON

  mzapo_lcdtest.c       - main and only file

  (C) Copyright 2004 - 2017 by Pavel Pisa
      e-mail:   pisa@cmp.felk.cvut.cz
      homepage: http://cmp.felk.cvut.cz/~pisa
      work:     http://www.pikron.com/
      license:  any combination GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include <byteswap.h>
#include <getopt.h>
#include <inttypes.h>
#include <time.h>
#include "par_lcd.h"
#include "knobs.h"


char *memdev="/dev/mem";

void *map_phys_address(off_t region_base, size_t region_size, int opt_cached)
{
  unsigned long mem_window_size;
  unsigned long pagesize;
  unsigned char *mm;
  unsigned char *mem;
  int fd;

  fd = open(memdev, O_RDWR | (!opt_cached? O_SYNC: 0));
  if (fd < 0) {
    fprintf(stderr, "cannot open %s\n", memdev);
    return NULL;
  }

  pagesize=sysconf(_SC_PAGESIZE);

  mem_window_size = ((region_base & (pagesize-1)) + region_size + pagesize-1) & ~(pagesize-1);

  mm = mmap(NULL, mem_window_size, PROT_WRITE|PROT_READ,
              MAP_SHARED, fd, region_base & ~(pagesize-1));
  mem = mm + (region_base & (pagesize-1));

  if (mm == MAP_FAILED) {
    fprintf(stderr,"mmap error\n");
    return NULL;
  }

  return mem;
}

void parlcd_write_cmd(unsigned char *parlcd_mem_base, uint16_t cmd)
{
  *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_CMD_o) = cmd;
}

void parlcd_write_data(unsigned char *parlcd_mem_base, uint16_t data)
{
  *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = data;
}

void parlcd_write_data2x(unsigned char *parlcd_mem_base, uint32_t data)
{
  *(volatile uint32_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = data;
}

void parlcd_delay(int msec)
{
  struct timespec wait_delay = {.tv_sec = msec / 1000,
                                .tv_nsec = (msec % 1000) * 1000 * 1000};
  clock_nanosleep(CLOCK_MONOTONIC, 0, &wait_delay, NULL);
}

#include <cmath>
#include "pixel.h"
#include "PPMReader.h"


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

#define _POSIX_C_SOURCE 200112L

typedef unsigned char byte;
int main(int argc, char *argv[])
{
    unsigned char *parlcd_mem_base;
    unsigned char *knobs_mem_base;

    /*
     * Setup memory mapping which provides access to the peripheral
     * registers region of RGB LEDs, knobs and line of yellow LEDs.
     */
    knobs_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if (knobs_mem_base == NULL){
        printf("Knobs allocation failes.\n");
        exit(1);
    }

    parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    if (parlcd_mem_base == NULL){
        printf("Display allocation failes.\n");
        exit(1);
    }
    parlcd_hx8357_init(parlcd_mem_base);

    PPMReader reader(argc>1?argv[1]:"rick.ppm");
    parlcd_write_cmd(parlcd_mem_base, 0x2c);
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

        printf("station: %d color part: %d value: %d\n", (int)curr_LAU/4%num_of_stations, (int)new_SC%3, (int)curr_VAL );

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
