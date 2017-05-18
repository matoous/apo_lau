/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz
      license:  MIT

 *******************************************************************/

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
#include "socket_rocket.h"

#define MIN(x,y) (x < y) ? (x) : (y)

using namespace std;

int display_data[320][480];
int curr_device_in_list, frame_begin, frame_end, current_display_style;
char button1, button2, button3;

/***
 * Puts char on specific place on display
 * @param c | char
 * @param row | 0 <= row <= 20
 * @param column | 0 <= column <= 59
 * @param color
 * @param background
 */
void put_char_there(char c, int row, int column, uint16_t color, uint16_t background){
    for(int i = 0; i < 16; i++)
        for(int u = 0; u < 8; u++)
            display_data[i+row*16][column*8 + u] = (font_rom8x16.bits[(int)c*16+i]>>(15-u)) & 1 ? color : background;
}

/***
 * Draws string on line
 * @param c | string
 * @param row | 0 <= row <= 19
 * @param offset | 0 <= offset in row <= 60
 * @param color | font color
 * @param background_color | background color
 */
void put_string_on_line(char* c, int row, int offset, uint16_t color, uint16_t background_color){
    int len = strlen(c);
    for(int i = 0; i < len; i++)
        put_char_there(c[i], row, offset+i, color, background_color);
}

/***
 * Redraw the display
 * @param parlcd_mem_base
 */
void redraw(unsigned char* parlcd_mem_base){
    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for(int i = 0; i < 320; i++)
        for(int u = 0; u < 480; u++)
            parlcd_write_data(parlcd_mem_base, display_data[i][u]);
}

/***
 * Inits static elements on display
 */
void one_device_draw_init(){
    // null display
    for(int i = 0; i < 320; i++)
        for(int u = 0; u < 480; u++)
            display_data[i][u] = DEFAULT_BACKGROUND;
    // arrow
    put_char_there('<', 1, 2, WHITE, DEFAULT_BACKGROUND);
    put_char_there('>', 1, 48, WHITE, DEFAULT_BACKGROUND);
    // underline
    for(int i = 1; i < 59; i++)
        put_char_there('_', 2, i, WHITE, DEFAULT_BACKGROUND);
    // ceiling
    char str[50];
    strcpy(str, "Ceiling color:");
    put_string_on_line(str, 4, 3, WHITE, DEFAULT_BACKGROUND);
    // walls
    strcpy(str, "Walls color:");
    put_string_on_line(str, 9, 3, WHITE, DEFAULT_BACKGROUND);
    // name
    strcpy(str, "  dzivjmat@fel.cvut.cz");
    str[0] = 0x03;
    put_string_on_line(str, 4, 36, WHITE, DEFAULT_BACKGROUND);
}

/***
 * Draw given light admin unit info to display
 * @param lu
 * @param knob2
 * @param parlcd_mem_base
 */
void one_device_draw(lau_t lu, int selected_row, unsigned char* parlcd_mem_base){

    char buffer[32];

    // device name
    int idx = 4;
    for(int i = 0; i < 16; i++)
        put_char_there(' ', 1, i+4, WHITE, DEFAULT_BACKGROUND);
    for(int i = 0; i < (int)strlen(lu.name); i++)
        put_char_there(lu.name[i], 1, idx++, WHITE, DEFAULT_BACKGROUND);

    // Ceiling color red
    for(int i = 0; i < 32; i++)
        buffer[i] = ' ';
    sprintf(buffer, "Red: %.3d", lu.ceiling_color.r);
    put_string_on_line(buffer, 5, 5, 0 == selected_row ? WHITE : BLACK, 0 == selected_row ? DEFAULT_SELECTED_BACKGROUND_COLOR : WHITE);

    // Ceiling color green
    for(int i = 0; i < 32; i++)
        buffer[i] = ' ';
    sprintf(buffer, "Green: %.3d", lu.ceiling_color.g);
    put_string_on_line(buffer, 6, 5, 1 == selected_row ? WHITE : BLACK, 1 == selected_row ? DEFAULT_SELECTED_BACKGROUND_COLOR : WHITE);

    // Ceiling color blue
    for(int i = 0; i < 32; i++)
        buffer[i] = ' ';
    sprintf(buffer, "Blue: %.3d", lu.ceiling_color.b);
    put_string_on_line(buffer, 7, 5, 2 == selected_row ? WHITE : BLACK, 2 == selected_row ? DEFAULT_SELECTED_BACKGROUND_COLOR : WHITE);

    // Walls color red
    for(int i = 0; i < 32; i++)
        buffer[i] = ' ';
    sprintf(buffer, "Red: %.3d", lu.walls_color.r);
    put_string_on_line(buffer, 10, 5, 3 == selected_row ? WHITE : BLACK, 3 == selected_row ? DEFAULT_SELECTED_BACKGROUND_COLOR : WHITE);

    // Walls color green
    for(int i = 0; i < 32; i++)
        buffer[i] = ' ';
    sprintf(buffer, "Green: %.3d", lu.walls_color.g);
    put_string_on_line(buffer, 11, 5, 4 == selected_row ? WHITE : BLACK, 4 == selected_row ? DEFAULT_SELECTED_BACKGROUND_COLOR : WHITE);

    // Walls color blue
    for(int i = 0; i < 32; i++)
        buffer[i] = ' ';
    sprintf(buffer, "Blue: %.3d", lu.walls_color.b);
    put_string_on_line(buffer, 12, 5, 5 == selected_row ? WHITE : BLACK, 5 == selected_row ? DEFAULT_SELECTED_BACKGROUND_COLOR : WHITE);

    // Back option
    for(int i = 0; i < 32; i++)
        buffer[i] = ' ';
    sprintf(buffer, "Back to list");
    put_string_on_line(buffer, 14, 5, 6 == selected_row ? WHITE : BLACK, 6 == selected_row ? DEFAULT_SELECTED_BACKGROUND_COLOR : WHITE);

    // Logo
    for(int i = 0; i < 16; i++)
        for(int u = 0; u < 16; u++)
            display_data[16+i][u+440] = lu.icon[16*i + u];

    redraw(parlcd_mem_base);
}

/***
 * Init all devices list
 */
void all_devices_draw_init(){
    // null display
    for(int i = 0; i < 320; i++)
        for(int u = 0; u < 480; u++)
            display_data[i][u] = DEFAULT_BACKGROUND;
}

/***
 * Draw devices list on display
 * @param devices | vector of all connected devices
 * @param knob_change | change of knob of currently selected device
 */
void all_devices_draw(vector<pair<sockaddr_in, lau_t>> devices, int knob_change){
    // null display
    for(int i = 0; i < 320; i++)
        for(int u = 0; u < 480; u++)
            display_data[i][u] = DEFAULT_BACKGROUND;

    curr_device_in_list += knob_change;
    if(curr_device_in_list >= devices.size())
        curr_device_in_list = devices.size() -1;
    if(curr_device_in_list > frame_end)
        frame_end += 5, frame_begin += 5;
    if(curr_device_in_list < 0)
        curr_device_in_list = 0;
    if(curr_device_in_list < frame_begin);
        frame_begin -= 5, frame_end -= 5;

    char buffer[32];

    for(int i = frame_begin; i < ((unsigned int)frame_end + 1 < devices.size()) ? frame_end +1 : devices.size(); i++){
        for(int u = 0; u < 32; u++)
            buffer[u] = ' ';
        sprintf(buffer, "%s", devices[i].second.name);
        for(int u = 0; u < 16; u++)
            for(int j = 0; j < 16; j++)
                display_data[u+32][j+(i*16)] = devices[i].second.icon[u*16+j];
        put_string_on_line(buffer, i+1, 4, i == curr_device_in_list ? DEFAULT_SELECTED_FONT_COLOR : WHITE, i == curr_device_in_list ? DEFAULT_SELECTED_BACKGROUND_COLOR : DEFAULT_BACKGROUND);
    }
}

/***
 * Reads knobs to provided variables
 * @param k1
 * @param k2
 * @param k3
 * @param b1
 * @param b2
 * @param b3
 * @param knobs_mem_base
 */
void read_knobs(uint8_t* k1, uint8_t* k2, uint8_t* k3, uint8_t* b1, uint8_t* b2, uint8_t* b3, unsigned char* knobs_mem_base){
    uint32_t rgb_knobs_value = *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_KNOBS_8BIT_o);
    *k1 = rgb_knobs_value & 0xFF;
    *k2 = (rgb_knobs_value >> 8) & 0xFF;
    *k3 = (rgb_knobs_value >> 16) & 0xFF;
    *b1 = (rgb_knobs_value >> 24) & 1;
    *b2 = (rgb_knobs_value >> 25) & 1;
    *b3 = (rgb_knobs_value >> 26) & 1;
    /* Store the read value to the register controlling individual LEDs */
    *(volatile uint32_t*)(knobs_mem_base + SPILED_REG_LED_LINE_o) = rgb_knobs_value;
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
void par_lcder(lau_t* lu, vector<pair<sockaddr_in, lau_t>>* devices, char* run, int* sockfd, mutex* local_lau_mutex, mutex* devices_mutes){
    // Map knobs
    unsigned char* knobs_mem_base = (unsigned char*)map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    if(knobs_mem_base == NULL){
        printf("Error mapping knobs and LED.\n");
        return;
    }

    // Map display
    unsigned char* parlcd_mem_base = (unsigned char*)map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    if(parlcd_mem_base == NULL){
        printf("Error mapping LCD display.\n");
        return;
    }

    // Init variables
    curr_device_in_list = 0, frame_begin = 0, frame_end = 18, current_display_style = 1;

    // init display
    parlcd_hx8357_init(parlcd_mem_base);

    all_devices_draw(*devices, 0);
    // init display data

    uint8_t knob1, knob2, knob3, prev1, prev2, prev3, button1, button2, button3;

    read_knobs(&prev1, &prev2, &prev3, &button1, &button2, &button3, knobs_mem_base);

    struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 50 * 1000 * 1000};

    int selected_row = (knob2 >> 2) % 7;

    /***
     * !!MAIN!! loop
     */
     /*
    while(*run){
        // read new values from inputs
        read_knobs(&knob1, &knob2, &knob3, &button1, &button2, &button3, knobs_mem_base);

        // Displaying devices list
        if(current_display_style == 1){
            if(knob1 != prev1){
                int change = 0;
                if(prev1 < 13 && knob1 > 245)
                    change = -((int)256 - knob1 + prev1)/4;
                else if(prev1 > 245 && knob1 < 15)
                    change = ((int)256-prev1+knob1)/4;
                else if(prev1 < knob1)
                    change = ((int)knob1-prev1)/4;
                else if(prev1 > knob1)
                    change = -((int)prev1-knob1)/4;
                prev1 = knob1;
                all_devices_draw(*devices, change);
            }
            if(button1){
                current_display_style = 2;
                selected_row = (knob2 >> 2) % 7;
                one_device_draw_init();
                one_device_draw((*devices)[curr_device_in_list].second, knob2, parlcd_mem_base);
            }
        }
        else if(current_display_style == 2){ // Displaying one device
            bool changed = false;
            if(selected_row == 6 && button1){
                prev1 = knob1;
                current_display_style = 1;
                continue;
            }
            // Device change
            if(knob1 != prev1){
                int change = 0;
                if(prev1 < 13 && knob1 > 245)
                    change = -((int)256 - (int)knob1 + (int)prev1)/4;
                else if(prev1 > 245 && knob1 < 15)
                    change = ((int)256-(int)prev1+(int)knob1)/4;
                else if(prev1 < knob1)
                    change = ((int)knob1-(int)prev1)/4;
                else if(prev1 > knob1)
                    change = -((int)prev1-(int)knob1)/4;
                curr_device_in_list += change;
                if(curr_device_in_list < 0)
                    curr_device_in_list = 0;
                if(curr_device_in_list >= (*devices).size())
                    curr_device_in_list = (*devices).size() -1;
                prev1 = knob1;
                changed = true;
            }
            // Color component change
            if(knob2 != prev2){
                prev2 = knob2;
                selected_row = (knob2 >> 2) % 7;
                changed = true;
            }
            int change = 0;
            // Color change
            if(knob3 != prev3){
                changed = true;
                if(prev3 < 13 && knob3 > 245)
                    change = -((int)256 - (int)knob3 + (int)prev3);
                else if(prev3 > 245 && knob3 < 15)
                    change = ((int)256-(int)prev3+(int)knob3);
                else if(prev3 < knob3)
                    change = ((int)knob3-(int)prev3);
                else if(prev3 > knob3)
                    change = -((int)prev3-(int)knob3);
                prev3 = knob3;
            }
            // redraw
            if(changed){
                // do changes
                if(curr_device_in_list == 0){
                    // changes on local device
                    switch (selected_row){
                        case 0:
                            lu->ceiling_color.r = ((int)lu->ceiling_color.r + change < 0) ? 0 : ((int)lu->ceiling_color.r + change > 255) ? 255 : lu->ceiling_color.r + change;
                            break;
                        case 1:
                            lu->ceiling_color.g = ((int)lu->ceiling_color.g + change < 0) ? 0 : ((int)lu->ceiling_color.g + change > 255) ? 255 : lu->ceiling_color.g + change;
                            break;
                        case 2:
                            lu->ceiling_color.b = ((int)lu->ceiling_color.b + change < 0) ? 0 : ((int)lu->ceiling_color.b + change > 255) ? 255 : lu->ceiling_color.b + change;
                            break;
                        case 3:
                            lu->walls_color.r = ((int)lu->walls_color.r + change < 0) ? 0 : ((int)lu->walls_color.r + change > 255) ? 255 : lu->walls_color.r + change;
                            break;
                        case 4:
                            lu->walls_color.g = ((int)lu->walls_color.g + change < 0) ? 0 : ((int)lu->walls_color.g + change > 255) ? 255 : lu->walls_color.g + change;
                            break;
                        case 5:
                            lu->walls_color.b = ((int)lu->walls_color.b + change < 0) ? 0 : ((int)lu->walls_color.b + change > 255) ? 255 : lu->walls_color.b + change;
                            break;
                    }
                }
                else{
                    if(change != 0)
                        send_modify(sockfd, (*devices)[curr_device_in_list].first,
                                    (int16_t)(selected_row == 0 ? change : 0),
                                    (int16_t)(selected_row == 1 ? change : 0),
                                    (int16_t)(selected_row == 2 ? change : 0),
                                    (int16_t)(selected_row == 3 ? change : 0),
                                    (int16_t)(selected_row == 4 ? change : 0),
                                    (int16_t)(selected_row == 5 ? change : 0));
                }

                one_device_draw((*devices)[curr_device_in_list].second, selected_row, parlcd_mem_base);
            }
        }

        // sleep
        clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
    }*/

    printf("Ending display.\n");
}