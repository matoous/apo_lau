# homematoousCLionProjectsapo_lau #

### pixel.c ###

<i>pixel_t pixel (const uint16_t color)</i>

    constructs pixel from uint16_t color

    @param color | uint16_t color

    @return pixel


### mzapo_parlcd.c ###

### socket_rocket.c ###

<i>void _int_tbetb (int x, char* buffer, int offset)</i>

    Puts int into the buffer (converts it to Big-Endian)

    @param x - number

    @param buffer - buffer to populate

    @param offset - offset in buffer


<i>void _uint32_t_tbetb (uint32_t x, char* buffer, int offset)</i>

    Puts uint32_t into the buffer (converts it to Big-Endian)

    @param x - number

    @param buffer - buffer to populate

    @param offset - offset


<i>void _int16_t_tbetb (int16_t x, char* buffer, int offset)</i>

    Puts int16_t into the buffer (converts it to Big-Endian)

    @param x - number

    @param buffer - buffer to populate

    @param offset - offset


<i>void _color_tbetb (pixel_t c, char* buffer, int offset)</i>

    Puts color into the buffer

    @param c - color

    @param buffer - buffer to populate

    @param offset - offset in buffer


<i>void _name_tbetb (const char c[], char* buffer, int offset)</i>

    Puts light admin unit name into the buffer

    @param c - light admin unit name

    @param buffer - buffer to populate

    @param offset - offset in buffer


<i>void _icon_tbetb (const uint16_t icon[], char* buffer, int offset)</i>

    Puts light admin unit icon into the buffer on given offset, converts to Big-Endian

    @param icon - icon array

    @param buffer - buffer to populate

    @param offset - offset in buffer


<i>uint32_t _bt_uint32_t (char* buf, int offset)</i>

    Gets uint32_t from buffer

    @param buf - buffer

    @param offset - offset in buffer

    @return uint32_t


<i>pixel_t _bt_color (char* buf, int offset)</i>

    Gets color from buffer (should be saved as uint32_t, first byte empty, then red, green, blue)

    @param buf - buffer

    @param offset - color offset in buffer

    @return pixel_t color


<i>void _bt_name (char* buf, int offset, char* uname)</i>

    Gets light admin unit name from buffer

    @param buf - buffer

    @param offset - name offset in buffer

    @param uname - array to populate


<i>uint16_t _bt_uint16_t (char* buf, int offset)</i>

    Gets uint16_t from buffer

    @param buf - buffer

    @param offset - offset in buffer

    @return uint16_t


<i>int16_t _bt_int16_t (char* buf, int offset)</i>

    Gets uint16_t from buffer

    @param buf - buffer

    @param offset - offset in buffer

    @return uint16_t


<i>void _bt_icon (char* buf, int offset, uint16_t icon[256])</i>

    Gets light admin unit icon from buffer

    @param buf - buffer

    @param offset - icon offset in buffer

    @param icon - array to populate


<i>void _sr_modify_lau (lau_t* lu, char* buf, pthread_mutex_t* local_lau_mutex)</i>

    Updates light admin unit

    @param lu - light admin unit

    @param buf - buffer


<i>void _sr_set_lau (lau_t* lu, char* buf, pthread_mutex_t* local_lau_mutex)</i>

    Sets light admin unit to received status

    @param lu - light unit

    @param buf - buffer


<i>void *sr_updater (void* args)</i>

    Runs as threads

    Sends update about local unit every one second

    @param passer struct with all necessary arguments


<i>void *sr_init (void* args)</i>

    Handles incoming messages, updates, etc.

    @param passer struct with all necessary arguments


<i>void send_modify (</i>

    Send update packet

    @param sockfd | socket file descriptor

    @param out_addr | address to send at

    @param cr | change in ceiling red

    @param cg | change in ceiling green

    @param cb | change in ceiling blue

    @param wr | change in walls red

    @param wg | change in walls green

    @param wb | change in walls blue


<i>void send_set (</i>

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

<i>void dl_init (devices_list_t* devices_list)</i>

    init device list in provided pointer to devices_list_t

    @param devices_list


<i>int dl_push_back (devices_list_t* devices_list, sockaddr_in addr, lau_t lau)</i>

    Push sock address : device pair on end of the devices list

    @param devices_list | devices list

    @param addr | sockaddr_in struct with device

    @param lau | light admin unit

    @return 0 if failed, 1 if OK


<i>unsigned int dl_size (devices_list_t* devices_list)</i>

    Return count of devices in list as unsigned integer

    @param devices_list | list of devices

    @return count of devices as unsigned int


<i>void dl_destroy (devices_list_t* devices_list)</i>

    Destroy the devices list struct

    @param devices_list | pointer to devices list


<i>void dl_delete (devices_list_t* devices_list, unsigned int index)</i>

    Delete device from list

    @param devices_list | list of devices

    @param index | index of device to be deleted


### console_info.c ###

<i>void *console_info (void* args)</i>

    Runs in separate thread, show updates about current application state

    @return NULL


### display.c ###

<i>void put_char_there (char c, int row, int column, uint16_t color, uint16_t background)</i>

    Puts char on specific place on display

    @param c | char

    @param row | 0 <= row <= 20

    @param column | 0 <= column <= 59

    @param color

    @param background


<i>void put_string_on_line (char* c, int row, int offset, uint16_t color, uint16_t background_color)</i>

    Draws string on line

    @param c | string

    @param row | 0 <= row <= 19

    @param offset | 0 <= offset in row <= 60

    @param color | font color

    @param background_color | background color


<i>void redraw (unsigned char* parlcd_mem_base)</i>

    Redraw the display

    @param parlcd_mem_base


<i>void one_device_draw_init ()</i>

    Inits static elements on display


<i>void one_device_draw (lau_t lu, int selected_row, unsigned char* parlcd_mem_base)</i>

    Draw given light admin unit info to display

    @param lu | light unit o be displayd

    @param selected_row | selected row (go back, colors, etc.)

    @param parlcd_mem_base | memory base for parlcd


<i>void all_devices_draw_init ()</i>

    Init all devices list

    nulls the display


<i>void all_devices_draw (devices_list_t* devices_list, int curr_device_in_list, unsigned char* parlcd_mem_base)</i>

    Draw devices list on display

    @param devices | vector of all connected devices

    @param knob_change | change of knob of currently selected device


<i>void read_knobs (uint8_t* k1, uint8_t* k2, uint8_t* k3, uint8_t* b1, uint8_t* b2, uint8_t* b3, unsigned char* knobs_mem_base)</i>

    Reads knobs to provided variables

    @param k1 | knob1

    @param k2 | knob2

    @param k3 | knob3

    @param b1 | button1

    @param b2 | button2

    @param b3 | button3

    @param knobs_mem_base | memory base for knobs and buttons


<i>void *par_lcder (void* args)</i>

    Handles display and hardware inputs

    @param passer with all necessary data


### font_rom8x16.c ###

### mzapo_phys.c ###

### app.c ###

