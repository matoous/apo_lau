//
// Created by matoous on 20.04.17.
//

#ifndef APO_LAU_PAR_LCD_H
#define APO_LAU_PAR_LCD_H

#define PARLCD_REG_BASE_PHYS 0x43c00000
#define PARLCD_REG_SIZE      0x00004000
#define PARLCD_REG_CMD_o                0x0008
#define PARLCD_REG_DATA_o               0x000C

void parlcd_hx8357_init(unsigned char*){}


#endif //APO_LAU_PAR_LCD_H
