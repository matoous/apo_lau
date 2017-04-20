//
// Created by matoous on 20.04.17.
//

#ifndef APO_LAU_KNOBS_H
#define APO_LAU_KNOBS_H
/*
 * Next macros provides location of knobs and LEDs peripherals
 * implemented in MZ_APO FPGA design.
 *
 * The complete list of peripheral implemented in the design
 * can be found on the page
 *   https://cw.fel.cvut.cz/wiki/courses/b35apo/documentation/mz_apo/start
 */
#define SPILED_REG_BASE_PHYS 0x43c40000
#define SPILED_REG_SIZE      0x00004000
#define SPILED_REG_LED_LINE_o           0x004
#define SPILED_REG_LED_RGB1_o           0x010
#define SPILED_REG_LED_RGB2_o           0x014
#define SPILED_REG_LED_KBDWR_DIRECT_o   0x018
#define SPILED_REG_KBDRD_KNOBS_DIRECT_o 0x020
#define SPILED_REG_KNOBS_8BIT_o         0x024



#endif //APO_LAU_KNOBS_H
