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

void draw_string_on_line(char* s, std::vector<uint8_t>* arr, int line){
    for(int i = 0; i < 16; i++){
        for(int u = 0; u < 60; u++){
            (*arr)[(i+line*16)*60 + u] = (uint8_t)(font_rom8x16.bits[(int)s[u]*16+i]>>8);
        }
    }
}

void draw(lau_t* lu, int knob2, unsigned char* parlcd_mem_base){

    std::vector<uint8_t> final_array(19200, 0);
    char buffer[60];


    /***
     * HEADER - device name and arrows
     */
     for(int i = 0; i < 60; i++)
         buffer[i] = ' ';
    sprintf(buffer, "  <    %.16s    >  ", (*lu).name);
    draw_string_on_line(buffer, &final_array, 1);

    /***
     * HEADER UNDERLINE
     */
    sprintf(buffer, " ________________________________________________________ ");
    draw_string_on_line(buffer, &final_array, 2);

    /***
     * CEILING COLOR
     */
    for(int i = 0; i < 60; i++)
        buffer[i] = ' ';
     sprintf(buffer, "  Ceiling color:          ");
    draw_string_on_line(buffer, &final_array, 4);

    /***
    * CEILING COLOR R
    */
    for(int i = 0; i < 60; i++)
        buffer[i] = ' ';
    sprintf(buffer, "  Red: %d  ", (*lu).ceiling_color.r);
    draw_string_on_line(buffer, &final_array, 5);

    /***
    * CEILING COLOR G
    */
    sprintf(buffer, "  Green: %d  ", (*lu).ceiling_color.g);
    draw_string_on_line(buffer, &final_array, 6);

    /***
    * CEILING COLOR B
    */
    sprintf(buffer, "  Blue: %d  ", (*lu).ceiling_color.b);
    draw_string_on_line(buffer, &final_array, 7);

    /***
    * WALLS COLOR
    */
    sprintf(buffer, "  Walls color:          ");
    draw_string_on_line(buffer, &final_array, 9);
    for(int i = 0; i < 60; i++)
        buffer[i] = ' ';

    /***
    * WALLS COLOR R
    */
    sprintf(buffer, "  Red: %d  ", (*lu).walls_color.r);
    draw_string_on_line(buffer, &final_array, 10);

    /***
    * WALLS COLOR G
    */
    sprintf(buffer, "  Green: %d  ", (*lu).walls_color.g);
    draw_string_on_line(buffer, &final_array, 11);

    /***
    * WALLS COLOR B
    */
    sprintf(buffer, "  Blue: %d  ", (*lu).walls_color.b);
    draw_string_on_line(buffer, &final_array, 12);

    for(int i = 16260; i < 18000; i++)
        final_array[i] = (uint8_t)0xF;


    int selected_line = knob2 + 5;
    if(selected_line == 8 || selected_line == 9){
        selected_line += 2;
    }
    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for(int i = 0; i < 19200; i++){
        if(i > 13440 && i < 14400){
            for(int u = 0; u < 16; u++, i++){
                for(int v = 0; v < 16; v++, i++){
                    parlcd_write_data(parlcd_mem_base, (*lu).icon[v+(u*16)]);
                }
                for(int v = 16; v < 480; v++, i++){
                    parlcd_write_data(parlcd_mem_base, (uint16_t)0x630C);
                }
            }
        }
        if(i >= selected_line*60*16 && i < (selected_line+1)*60*16){
            parlcd_write_data(parlcd_mem_base, (uint16_t) (final_array[i]>>7) % 2 ? 0x401F : 0xF800);
            parlcd_write_data(parlcd_mem_base, (uint16_t) (final_array[i]>>6) % 2 ? 0x401F : 0xF800);
            parlcd_write_data(parlcd_mem_base, (uint16_t) (final_array[i]>>5) % 2 ? 0x401F : 0xF800);
            parlcd_write_data(parlcd_mem_base, (uint16_t) (final_array[i]>>4) % 2 ? 0x401F : 0xF800);
            parlcd_write_data(parlcd_mem_base, (uint16_t) (final_array[i]>>3) % 2 ? 0x401F : 0xF800);
            parlcd_write_data(parlcd_mem_base, (uint16_t) (final_array[i]>>2) % 2 ? 0x401F : 0xF800);
            parlcd_write_data(parlcd_mem_base, (uint16_t) (final_array[i]>>1) % 2 ? 0x401F : 0xF800);
            parlcd_write_data(parlcd_mem_base, (uint16_t)final_array[i] % 2 ? 0x401F : 0xF800);
        }
        else{
            parlcd_write_data(parlcd_mem_base, (uint16_t) (final_array[i]>>7) % 2 ? 0xF80A : 0x630C);
            parlcd_write_data(parlcd_mem_base, (uint16_t) (final_array[i]>>6) % 2 ? 0xF80A : 0x630C);
            parlcd_write_data(parlcd_mem_base, (uint16_t) (final_array[i]>>5) % 2 ? 0xF80A : 0x630C);
            parlcd_write_data(parlcd_mem_base, (uint16_t) (final_array[i]>>4) % 2 ? 0xF80A : 0x630C);
            parlcd_write_data(parlcd_mem_base, (uint16_t) (final_array[i]>>3) % 2 ? 0xF80A : 0x630C);
            parlcd_write_data(parlcd_mem_base, (uint16_t) (final_array[i]>>2) % 2 ? 0xF80A : 0x630C);
            parlcd_write_data(parlcd_mem_base, (uint16_t) (final_array[i]>>1) % 2 ? 0xF80A : 0x630C);
            parlcd_write_data(parlcd_mem_base, (uint16_t)final_array[i] % 2 ? 0xF80A : 0x630C);
        }
    }
}

void init(lau_t* lu, std::vector<std::pair<unsigned int, lau_t>>* devices, char* run){

    uint32_t rgb_knobs_value;
    uint32_t uint_val;

    unsigned char* knobs_mem_base = (unsigned char*)map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if(knobs_mem_base == NULL){
        printf("Error mapping knobs and LED.\n");
    }

    unsigned char* parlcd_mem_base = (unsigned char*)map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    if(parlcd_mem_base == NULL){
        printf("Error mapping LCD display.\n");
    }

    parlcd_hx8357_init(parlcd_mem_base);

    uint8_t knob1, knob2, knob3, prev1, prev2, prev3;

    rgb_knobs_value = *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_KNOBS_8BIT_o);

    /* Store the read value to the register controlling individual LEDs */
    *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_LED_LINE_o) = rgb_knobs_value;
    uint_val = rgb_knobs_value;
    prev1 = uint_val & 0xFF;
    prev2 = (uint_val >> 8) & 0xFF;
    prev3 = (uint_val >> 16) & 0xFF;

    int curr_device_num = 0;
    struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 50 * 1000 * 1000};

    draw(lu, curr_device_num, parlcd_mem_base);

    /***
     * !!MAIN!! loop
     */
     /*
    while(*run){
        char changed = 0;

        rgb_knobs_value = *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_KNOBS_8BIT_o);

        *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_LED_LINE_o) = rgb_knobs_value;
        uint_val = rgb_knobs_value;
        knob1 = uint_val & 0xFF;
        knob2 = (uint_val >> 8) & 0xFF;
        knob3 = (uint_val >> 16) & 0xFF;
        printf("Knobs %hhu(%hhu) %hhu(%hhu) %hhu(%hhu)\n",
               knob1, prev1,
               knob2, prev2,
               knob3, prev3);

        // Device change
        if(knob1 != prev1){
            prev1 = knob1;
            changed = 1;
            curr_device_num = (knob1 << 2) % (*devices).size();
        }

        // Color component change
        if(knob2 != prev2){
            prev2 = knob2;
            knob2 = (knob2 >> 2) % 6;
            changed = 1;
        }

        // Color change
        if(knob3 != prev3){
            int change;
            if(prev3 < 13 && knob3 > 245)
                change = -((int)256 - knob3 + prev3)/4;
            else if(prev3 > 245 && knob3 < 15)
                change = ((int)256-prev3+knob3)/4;
            else if(prev3 < knob3)
                change = ((int)knob3-prev3)/4;
            else if(prev3 > knob3)
                change = -((int)prev3-knob3)/4;
            changed = 1;
            prev1 = knob1;
        }

        *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_LED_RGB1_o) = rgb_knobs_value;
        *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_LED_RGB2_o) = rgb_knobs_value;


        if(changed)
            draw(&(*devices)[curr_device_num].second, 0, parlcd_mem_base);
        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    }
    */
    printf("Ending display.\n");
}