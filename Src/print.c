/*********************************************************************
*                SEGGER MICROCONTROLLER SYSTEME GmbH                 *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*
* C-file generated by
*
*        �C/GUI-BitmapConvert V3.90.
*        Compiled Aug 19 2004, 09:07:56
*          (c) 2002  Micrium, Inc.
  www.micrium.com

  (c) 1998-2002  Segger
  Microcontroller Systeme GmbH
  www.segger.com
*
**********************************************************************
*
* Source file: print
* Dimensions:  16 * 14
* NumColors:   2
*
**********************************************************************
*/

#include "stdlib.h"

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

/*   Palette
The following are the entries of the palette table.
Every entry is a 32-bit value (of which 24 bits are actually used)
the lower   8 bits represent the Red component,
the middle  8 bits represent the Green component,
the highest 8 bits (of the 24 bits used) represent the Blue component
as follows:   0xBBGGRR
*/

static GUI_CONST_STORAGE GUI_COLOR Colorsprint[] = {
    0xFFFFFF, 0x000000
};

static GUI_CONST_STORAGE GUI_LOGPALETTE Palprint = {
  2,	/* number of entries */
  0, 	/* No transparency */
  &Colorsprint[0]
};

static GUI_CONST_STORAGE unsigned char acprint[] = {
  XXXXX___, ______XX,
  XXXX_XXX, XXXXX_XX,
  XXXX_X__, ___X_XXX,
  XXX_XXXX, XXXX_XXX,
  XXX_X___, __X____X,
  XX_XXXXX, XXX_X_X_,
  X_______, ___X_X__,
  _XXXXXXX, XXX_X_X_,
  ________, _____XX_,
  _XXXXXX_, __XX_X_X,
  _XXXXXXX, XXXX___X,
  ________, _____X_X,
  X_XXXXXX, XXX_X_XX,
  XX______, _____XXX
};

GUI_CONST_STORAGE GUI_BITMAP bmprint = {
  16, /* XSize */
  14, /* YSize */
  2, /* BytesPerLine */
  1, /* BitsPerPixel */
  acprint,  /* Pointer to picture data (indices) */
  &Palprint  /* Pointer to palette */
};

/* *** End of file *** */
