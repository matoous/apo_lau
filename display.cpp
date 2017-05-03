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


using namespace std;

vector<vector<uint16_t>> data;

void draw_string_on_line(char* s, std::vector<uint8_t>* arr, int line){
    for(int i = 0; i < 16; i++){
        for(int u = 0; u < 60; u++){
            (*arr)[(i+line*16)*60 + u] = (uint8_t)(font_rom8x16.bits[(int)s[u]*16+i]>>8);
        }
    }
}

/***
 * @param c | char
 * @param row | 0 <= row <= 20
 * @param column | 0 <= column <= 59
 * @param color
 * @param background
 */
void put_char_there(char c, int row, int column, uint16_t color, uint16_t background){
    for(int i = 0; i < 16; i++){
        data[i+row*16][column*8]     = font_rom8x16.bits[(int)c*16+i]>> 15 & 1 ? color : background;
        data[i+row*16][column*8 + 1] = font_rom8x16.bits[(int)c*16+i]>> 14 & 1 ? color : background;
        data[i+row*16][column*8 + 2] = font_rom8x16.bits[(int)c*16+i]>> 13 & 1 ? color : background;
        data[i+row*16][column*8 + 3] = font_rom8x16.bits[(int)c*16+i]>> 12 & 1 ? color : background;
        data[i+row*16][column*8 + 4] = font_rom8x16.bits[(int)c*16+i]>> 11 & 1 ? color : background;
        data[i+row*16][column*8 + 5] = font_rom8x16.bits[(int)c*16+i]>> 10 & 1 ? color : background;
        data[i+row*16][column*8 + 6] = font_rom8x16.bits[(int)c*16+i]>>  9 & 1 ? color : background;
        data[i+row*16][column*8 + 7] = font_rom8x16.bits[(int)c*16+i]>>  8 & 1 ? color : background;
    }
}

/***
 * inits static elements on display
 */
void draw_init(){
    data.resize(320, vector<uint16_t>(480, 0xFFFF));

    // arrow
    put_char_there('<', 1, 2, WHITE, BLACK);
    put_char_there('>', 1, 48, WHITE, BLACK);

    // underline
    for(int i = 1; i < 59; i++)
        put_char_there('_', 2, i, WHITE, BLACK);

    // ceiling
    int idx = 3;
    for(char &c : string("Ceiling color:"))
        put_char_there(c, 4, idx++, WHITE, BLACK);

    // walls
    idx = 3;
    for(char &c : string("Walls color:"))
        put_char_there(c, 9, idx++, WHITE, BLACK);

    // name
    idx = 36;
    for(char &c : string("♥ dzivjmat@fel.cvut.cz"))
        put_char_there(c, 19, idx++, 0xFCE0, BLACK);
}

/***
 * Draw given light admin unit info to display
 * @param lu
 * @param knob2
 * @param parlcd_mem_base
 */
void draw(lau_t lu, int knob2, unsigned char* parlcd_mem_base){

    char buffer[32];
    knob2 += 5;
    int selected_line = (knob2 == 8 || knob2 == 9) ? knob2 + 2 : knob2;

    // device name
    int idx = 4;
    for(int i = 0; i < 16; i++)
        put_char_there(' ', 1, i+4, WHITE, BLACK);
    for(int i = 0; i < strlen(lu.name); i++)
        put_char_there(lu.name[i], 1, idx++, WHITE, BLACK);

    // Ceiling color red
    for(int i = 0; i < 32; i++)
        buffer[i] = ' ';
    sprintf(buffer, "Red: %.3d", lu.ceiling_color.r);
    idx = 5;
    for(char c : buffer)
        put_char_there(c, 5, idx++, 5 == selected_line ? BLACK : WHITE, 5 == selected_line ? 0x2FF3 : BLACK);

    // Ceiling color green
    for(int i = 0; i < 32; i++)
        buffer[i] = ' ';
    sprintf(buffer, "Green: %.3d", lu.ceiling_color.g);
    idx = 5;
    for(char c : buffer)
        put_char_there(c, 6, idx++, 6 == selected_line ? BLACK : WHITE, 6 == selected_line ? 0x2FF3 : BLACK);

    // Ceiling color blue
    for(int i = 0; i < 32; i++)
        buffer[i] = ' ';
    sprintf(buffer, "Blue: %.3d", lu.ceiling_color.b);
    idx = 5;
    for(char c : buffer)
        put_char_there(c, 7, idx++, 7 == selected_line ? BLACK : WHITE, 7 == selected_line ? 0x2FF3 : BLACK);

    // Walls color red
    for(int i = 0; i < 32; i++)
        buffer[i] = ' ';
    sprintf(buffer, "Red: %.3d", lu.walls_color.r);
    idx = 5;
    for(char c : buffer)
        put_char_there(c, 10, idx++, 10 == selected_line ? BLACK : WHITE, 10 == selected_line ? 0x2FF3 : BLACK);

    // Walls color green
    for(int i = 0; i < 32; i++)
        buffer[i] = ' ';
    sprintf(buffer, "Green: %.3d", lu.walls_color.g);
    idx = 5;
    for(char c : buffer)
        put_char_there(c, 11, idx++, 11 == selected_line ? BLACK : WHITE, 11 == selected_line ? 0x2FF3 : BLACK);

    // Walls color blue
    for(int i = 0; i < 32; i++)
        buffer[i] = ' ';
    sprintf(buffer, "Blue: %.3d", lu.walls_color.b);
    idx = 5;
    for(char c : buffer)
        put_char_there(c, 12, idx++, 12 == selected_line ? BLACK : WHITE, 12 == selected_line ? 0x2FF3 : BLACK);

    // Logo
    for(int i = 0; i < 16; i++)
        for(int u = 0; u < 16; u++)
            data[16+i][u+440] = lu.icon[16*i + u];


    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for(int i = 0; i < 320; i++){
        for(int u = 0; u < 480; u++){
            parlcd_write_data(parlcd_mem_base, data[i][u]);
        }
    }
}

/***
 * Handles display and hardware inputs
 * @param lu
 * @param devices
 * @param run
 * @param sockfd
 * @param local_lau_mutex
 * @param devices_mutes
 */
void par_lcder(lau_t* lu, vector<pair<unsigned int, lau_t>>* devices, char* run, int sockfd, mutex* local_lau_mutex, mutex* devices_mutes){

    uint32_t rgb_knobs_value;
    uint32_t uint_val;

    unsigned char* knobs_mem_base = (unsigned char*)map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if(knobs_mem_base == NULL){
        printf("Error mapping knobs and LED.\n");
        return;
    }

    unsigned char* parlcd_mem_base = (unsigned char*)map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    if(parlcd_mem_base == NULL){
        printf("Error mapping LCD display.\n");
        return;
    }

    // init display
    parlcd_hx8357_init(parlcd_mem_base);
    // init display data
    draw_init();

    uint8_t knob1, knob2, knob3, prev1, prev2, prev3;

    rgb_knobs_value = *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_KNOBS_8BIT_o);

    /* Store the read value to the register controlling individual LEDs */
    *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_LED_LINE_o) = rgb_knobs_value;

    uint_val = rgb_knobs_value;
    prev1 = uint_val & 0xFF;
    prev2 = (uint_val >> 8) & 0xFF;
    prev3 = (uint_val >> 16) & 0xFF;

    // initial values
    int curr_device_num = (prev1 >> 2) % (*devices).size();
    int selected_row = (prev2 >> 2) % 6;
    struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 100 * 1000 * 1000};

    draw(*lu, selected_row, parlcd_mem_base);

    /***
     * !!MAIN!! loop
     */
    while(*run){
        char changed = 0;

        // get knobs value
        rgb_knobs_value = *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_KNOBS_8BIT_o);

        // display knobs value
        *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_LED_LINE_o) = rgb_knobs_value;

        uint_val = rgb_knobs_value;
        knob1 = uint_val & 0xFF;
        knob2 = (uint_val >> 8) & 0xFF;
        knob3 = (uint_val >> 16) & 0xFF;

        // print knobs value
        printf("Knobs %hhu(%hhu) %hhu(%hhu) %hhu(%hhu)\n",
               knob1, prev1,
               knob2, prev2,
               knob3, prev3);

        // Device change
        if(knob1 != prev1){
            prev1 = knob1;
            changed = 1;
            curr_device_num = (knob1 >> 2) % (*devices).size();
        }

        // Color component change
        if(knob2 != prev2){
            prev2 = knob2;
            knob2 = (knob2 >> 2) % 6;
            changed = 1;
        }


        int change = 0;
        // Color change
        if(knob3 != prev3){
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

        // redraw
        if(changed){
            // do changes
            if(curr_device_num == 0){
                // changes on local device
                switch (selected_row){
                    case 0:
                        lu->ceiling_color.r = ((int)lu->ceiling_color.r + change < 0)
                                               ? 0
                                               : ((int)lu->ceiling_color.r + change > 255)
                                                 ? 255
                                                 : lu->ceiling_color.r + change;
                        break;
                    case 1:
                        lu->ceiling_color.g = ((int)lu->ceiling_color.g + change < 0)
                                               ? 0
                                               : ((int)lu->ceiling_color.g + change > 255)
                                                 ? 255
                                                 : lu->ceiling_color.g + change;
                        break;
                    case 2:
                        lu->ceiling_color.b = ((int)lu->ceiling_color.b + change < 0)
                                               ? 0
                                               : ((int)lu->ceiling_color.b + change > 255)
                                                 ? 255
                                                 : lu->ceiling_color.b + change;
                        break;
                    case 3:
                        lu->walls_color.r = ((int)lu->walls_color.r + change < 0)
                                             ? 0
                                             : ((int)lu->walls_color.r + change > 255)
                                               ? 255
                                               : lu->walls_color.r + change;
                        break;
                    case 4:
                        lu->walls_color.g = ((int)lu->walls_color.g + change < 0)
                                             ? 0
                                             : ((int)lu->walls_color.g + change > 255)
                                               ? 255
                                               : lu->walls_color.g + change;
                        break;
                    case 5:
                        lu->walls_color.b = ((int)lu->walls_color.b + change < 0)
                                             ? 0
                                             : ((int)lu->walls_color.b + change > 255)
                                               ? 255
                                               : lu->walls_color.b + change;
                        break;
                }
            }
            else{
                sockaddr_in send_to;
                send_to.sin_family = AF_INET;
                send_to.sin_port = htons((*devices)[curr_device_num].first);
                send_to.sin_addr.s_addr = INADDR_BROADCAST;
                /*send_modify(sockfd, &send_to,
                            (int16_t)selected_row == 0 ? change : 0,
                            (int16_t)selected_row == 1 ? change : 0,
                            (int16_t)selected_row == 2 ? change : 0,
                            (int16_t)selected_row == 3 ? change : 0,
                            (int16_t)selected_row == 4 ? change : 0,
                            (int16_t)selected_row == 5 ? change : 0);*/
            }

            // redraw display
            draw((*devices)[curr_device_num].second, knob2, parlcd_mem_base);
        }

        // sleep
        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    }

    printf("Ending display.\n");
}