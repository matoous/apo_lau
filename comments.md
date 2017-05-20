# homematoousCLionProjectsapo_lau #

### pixel.c ###

__pixel_t pixel (const uint16_t color)__

    constructs pixel from uint16_t color

    @param color | uint16_t color

    @return pixel


### mzapo_parlcd.c ###

### socket_rocket.c ###

__void _int_tbetb (int x, char* buffer, int offset)__

    Puts int into the buffer (converts it to Big-Endian)

    @param x - number

    @param buffer - buffer to populate

    @param offset - offset in buffer


__void _uint32_t_tbetb (uint32_t x, char* buffer, int offset)__

    Puts uint32_t into the buffer (converts it to Big-Endian)

    @param x - number

    @param buffer - buffer to populate

    @param offset - offset


__void _int16_t_tbetb (int16_t x, char* buffer, int offset)__

    Puts int16_t into the buffer (converts it to Big-Endian)

    @param x - number

    @param buffer - buffer to populate

    @param offset - offset


__void _color_tbetb (pixel_t c, char* buffer, int offset)__

    Puts color into the buffer

    @param c - color

    @param buffer - buffer to populate

    @param offset - offset in buffer


__void _name_tbetb (const char c[], char* buffer, int offset)__

    Puts light admin unit name into the buffer

    @param c - light admin unit name

    @param buffer - buffer to populate

    @param offset - offset in buffer


__void _icon_tbetb (const uint16_t icon[], char* buffer, int offset)__

    Puts light admin unit icon into the buffer on given offset, converts to Big-Endian

    @param icon - icon array

    @param buffer - buffer to populate

    @param offset - offset in buffer


__uint32_t _bt_uint32_t (char* buf, int offset)__

    Gets uint32_t from buffer

    @param buf - buffer

    @param offset - offset in buffer

    @return uint32_t


__pixel_t _bt_color (char* buf, int offset)__

    Gets color from buffer (should be saved as uint32_t, first byte empty, then red, green, blue)

    @param buf - buffer

    @param offset - color offset in buffer

    @return pixel_t color


__void _bt_name (char* buf, int offset, char* uname)__

    Gets light admin unit name from buffer

    @param buf - buffer

    @param offset - name offset in buffer

    @param uname - array to populate


__uint16_t _bt_uint16_t (char* buf, int offset)__

    Gets uint16_t from buffer

    @param buf - buffer

    @param offset - offset in buffer

    @return uint16_t


__int16_t _bt_int16_t (char* buf, int offset)__

    Gets uint16_t from buffer

    @param buf - buffer

    @param offset - offset in buffer

    @return uint16_t


__void _bt_icon (char* buf, int offset, uint16_t icon[256])__

    Gets light admin unit icon from buffer

    @param buf - buffer

    @param offset - icon offset in buffer

    @param icon - array to populate


__void _sr_modify_lau (lau_t* lu, char* buf, pthread_mutex_t* local_lau_mutex)__

    Updates light admin unit

    @param lu - light admin unit

    @param buf - buffer


__void _sr_set_lau (lau_t* lu, char* buf, pthread_mutex_t* local_lau_mutex)__

    Sets light admin unit to received status

    @param lu - light unit

    @param buf - buffer


__void *sr_updater (void* args)__

    Runs as threads

    Sends update about local unit every one second

    @param passer struct with all necessary arguments


__void *sr_init (void* args)__

    Handles incoming messages, updates, etc.

    @param passer struct with all necessary arguments


__void send_modify (__

    Send update packet

    @param sockfd | socket file descriptor

    @param out_addr | address to send at

    @param cr | change in ceiling red

    @param cg | change in ceiling green

    @param cb | change in ceiling blue

    @param wr | change in walls red

    @param wg | change in walls green

    @param wb | change in walls blue


__void send_set (__

    Send set packet

    @param sockfd | socket file descriptor

    @param out_addr | address to send at

    @param cr | ceiling red

    @param cg | ceiling green

    @param cb | ceiling blue

    @param wr | walls red

    @param wg | walls green

    @param wb | walls blue


### devices_list.c ###

__void dl_init (devices_list_t* devices_list)__

    init device list in provided pointer to devices_list_t

    @param devices_list


__int dl_push_back (devices_list_t* devices_list, sockaddr_in addr, lau_t lau)__

    Push sock address : device pair on end of the devices list

    @param devices_list | devices list

    @param addr | sockaddr_in struct with device

    @param lau | light admin unit

    @return 0 if failed, 1 if OK


__unsigned int dl_size (devices_list_t* devices_list)__

    Return count of devices in list as unsigned integer

    @param devices_list | list of devices

    @return count of devices as unsigned int


__void dl_destroy (devices_list_t* devices_list)__

    Destroy the devices list struct

    @param devices_list | pointer to devices list


__void dl_delete (devices_list_t* devices_list, unsigned int index)__

    Delete device from list

    @param devices_list | list of devices

    @param index | index of device to be deleted


### console_info.c ###

__void *console_info (void* args)__

    Runs in separate thread, show updates about current application state

    @return NULL


### display.c ###

__void put_char_there (char c, int row, int column, uint16_t color, uint16_t background)__

    Puts char on specific place on display

    @param c | char

    @param row | 0 <= row <= 20

    @param column | 0 <= column <= 59

    @param color

    @param background


__void put_string_on_line (char* c, int row, int offset, uint16_t color, uint16_t background_color)__

    Draws string on line

    @param c | string

    @param row | 0 <= row <= 19

    @param offset | 0 <= offset in row <= 60

    @param color | font color

    @param background_color | background color


__void redraw (unsigned char* parlcd_mem_base)__

    Redraw the display

    @param parlcd_mem_base


__void one_device_draw_init ()__

    Inits static elements on display


__void one_device_draw (lau_t lu, int selected_row, unsigned char* parlcd_mem_base)__

    Draw given light admin unit info to display

    @param lu | light unit o be displayd

    @param selected_row | selected row (go back, colors, etc.)

    @param parlcd_mem_base | memory base for parlcd


__void all_devices_draw_init ()__

    Init all devices list

    nulls the display


__void all_devices_draw (devices_list_t* devices_list, int curr_device_in_list, unsigned char* parlcd_mem_base)__

    Draw devices list on display

    @param devices | vector of all connected devices

    @param knob_change | change of knob of currently selected device


__void read_knobs (uint8_t* k1, uint8_t* k2, uint8_t* k3, uint8_t* b1, uint8_t* b2, uint8_t* b3, unsigned char* knobs_mem_base)__

    Reads knobs to provided variables

    @param k1 | knob1

    @param k2 | knob2

    @param k3 | knob3

    @param b1 | button1

    @param b2 | button2

    @param b3 | button3

    @param knobs_mem_base | memory base for knobs and buttons


__void *par_lcder (void* args)__

    Handles display and hardware inputs

    @param passer with all necessary data


### font_rom8x16.c ###

### mzapo_phys.c ###

### app.c ###

