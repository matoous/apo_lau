//
// Created by matoous on 26.04.17.
//

#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <byteswap.h>
#include <getopt.h>
#include <inttypes.h>
#include <time.h>
#include "display.h"
#include "font_types.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "mzapo_parlcd.h"

void init(lau_t* lu, std::map<unsigned int, lau_t>* devices_map, char* run){

    display_t disp;
    disp.columns = 60;
    disp.rows = 40;
    disp.data = (uint8_t*)malloc(480*320/8);

    uint8_t knob1, knob2, knob3, prev1, prev2, prev3;
    char *curr_device_name;
    lau_t *curr_device;
    uint32_t rgb_knobs_value;
    unsigned int uint_val;
    struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 50 * 1000 * 1000};

    unsigned char* knobs_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if(knobs_mem_base == NULL){
        printf("Error mapping knobs and LED.\n");
        exit(1);
    }

    unsigned char* parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    if(parlcd_mem_base == NULL){
        printf("Error mapping LCD display.\n");
        exit(1);
    }

    parlcd_hx8357_init(parlcd_mem_base);

    /***
     * !!MAIN!! loop
     */
    while(*run){
        char changed = 0;

        rgb_knobs_value = *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_KNOBS_8BIT_o);

        /* Store the read value to the register controlling individual LEDs */
        *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_LED_LINE_o) = rgb_knobs_value;
        uint_val = rgb_knobs_value;
        knob1 = uint_val & 0xFF;
        knob2 = (uint_val >> 8) & 0xFF;
        knob3 = (uint_val >> 16) & 0xFF;
        printf("Knobs %huu(%huu) %huu(%huu) %huu(%huu)\n",
               knob1, prev1,
               knob2, prev2,
               knob3, prev3);

        // Device change
        if(knob1 != prev1){

            changed = 1;
            prev2 = knob2;
        }

        // Color component change
        if(knob2 != prev2){
            prev2 = knob2;
            knob2 = (knob2 >> 2) % 6;
            /*switch (knob2){
                case 0:
                    // ceiling r
                    break;
                case 1:
                    // ceiling g
                    break;
                case 2:
                    // ceiling b
                    break;
                case 3:
                    // walls r
                    break;
                case 4:
                    // walls g
                    break;
                case 5:
                    // walls b
                    break;
            }*/
            changed = 1;
        }

        // Color change
        if(knob3 != prev3){
            // subtract through 0 down
            if(prev3 < 13 && knob3 > 245){
                if(knob2 < 3){
                    if(knob2 == 0){
                        (*curr_device).ceiling_color.r -= (256 - knob3 + prev3)/4;
                    }
                    if(knob2 == 1){
                        (*curr_device).ceiling_color.g -= (256 - knob3 + prev3)/4;
                    }
                    if(knob2 == 2){
                        (*curr_device).ceiling_color.b -= (256 - knob3 + prev3)/4;
                    }
                }
                else{
                    if(knob2 == 3){
                        (*curr_device).walls_color.r -= (256 - knob3 + prev3)/4;
                    }
                    if(knob2 == 4){
                        (*curr_device).walls_color.g -= (256 - knob3 + prev3)/4;
                    }
                    if(knob2 == 5){
                        (*curr_device).walls_color.b -= (256 - knob3 + prev3)/4;
                    }
                }
            }
            // add through 0

            // add
            // subtract
            changed = 1;
            prev1 = knob1;
        }

        /*
         * Store RGB knobs values to the corersponding components controlling
         * a color/brightness of the RGB LEDs
         */
        *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_LED_RGB1_o) = rgb_knobs_value;
        *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_LED_RGB2_o) = rgb_knobs_value;

        /* Print values */
        printf("uint 0x%08x\n", uint_val);


        if(changed){
            // draw display
        }
        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    }

    printf("Ending display.\n");
}