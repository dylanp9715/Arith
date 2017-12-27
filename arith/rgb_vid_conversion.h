/* 
 * rgb_vid_conversion.h
 * by Derek Sims and Dylan Park
 * HW 4 Part 2A
 *
 * 
 */

#ifndef RGB_VID_CONVERSION_INCLUDED
#define RGB_VID_CONVERSION_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "uarray2.h"
#include "uarray2b.h"
#include "a2methods.h"
#include "a2blocked.h"
#include "a2plain.h"
#include "pnm.h"

typedef struct {
        UArray2b_T uarray2b;
        int denominator;
} closure2b;

typedef struct {
        UArray2_T uarray2;
        int denominator;
} closure2;

typedef struct {
        float y;
        float pb;
        float pr;
} vid_color;

/* Converts the rgb pixels in ppm_struct's uarray2 to vid_color pixels */
UArray2b_T rgb_to_vid(Pnm_ppm ppm_struct);
/* Converts the pixels in uarray2b from vid_color to rgb */
Pnm_ppm vid_to_rgb(UArray2b_T uarray2b);
/* Apply functions that do calculations */
void rgb_to_vid_apply(int col, int row, A2Methods_UArray2 uarray2, void *elem, 
                      void *cl);
void vid_to_rgb_apply(int col, int row, UArray2b_T uarray2b, void *elem,
                      void *cl);

#endif