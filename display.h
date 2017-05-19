/*******************************************************************
  APO semestral project by Matous Dzivjak

  (C) Copyright 2017 - 2017 by Matous Dzivjak
      e-mail:   dzivjmat@fel.cvut.cz
      license:  MIT

 *******************************************************************/

#ifndef APO_LAU_DISPLAY_H
#define APO_LAU_DISPLAY_H

// Some color constants
#define WHITE 0xFFFF
#define BLACK 0x0000
#define DEFAULT_BACKGROUND 0x39E7
#define DEFAULT_FONT_COLOR 0xCEDF
#define DEFAULT_SELECTED_FONT_COLOR 0xB006
#define DEFAULT_SELECTED_BACKGROUND_COLOR 0xD69A

/***
 * Runs in separate thread, handles knobs and buttons and updates display regulary
 * @return
 */
void* par_lcder(void*);

#endif //APO_LAU_DISPLAY_H
