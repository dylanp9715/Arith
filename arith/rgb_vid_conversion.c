/* 
 * rgb_vid_conversion.c
 * by Derek Sims and Dylan Park
 * HW 4
 *
 * 
 */

#include "rgb_vid_conversion.h"

/* rgb_to_vid
 * Purpose: Converts the pixels in the uarray2 of ppm_struct from rgb values 
 * into component-video color
 * Arguments: ppm_struct
 * Return Value: UArray2b_T populated with vid_color structs 
*/
UArray2b_T rgb_to_vid(Pnm_ppm ppm_struct) {
        int width = ppm_struct->width;
        int height = ppm_struct->height;
        
        UArray2b_T uarray2b = UArray2b_new(width, height, sizeof(vid_color), 2);
        closure2b mycl = { uarray2b, ppm_struct->denominator };
        
        ppm_struct->methods->map_row_major(ppm_struct->pixels, rgb_to_vid_apply
                , &mycl);
        return uarray2b;
}

/* rgb_to_vid_apply
 * Purpose: Performs calculations necessary to convert from rgb to vid_color
 * Arguments: ints for row and col number, uarray2, element, and cl which has 
 * uarray2b we want to populate and the denominator of the ppm image
 * Return Value: void
*/
void rgb_to_vid_apply(int col, int row, A2Methods_UArray2 uarray2, void *elem, 
                      void *cl) {
        (void) uarray2;
        closure2b *mycl = cl;
        vid_color vc;

        struct Pnm_rgb *rgb_struct = elem;
        float r = (float)(rgb_struct->red)/(float)mycl->denominator;
        float g = (float)(rgb_struct->green)/(float)mycl->denominator;
        float b = (float)(rgb_struct->blue)/(float)mycl->denominator;
        
        float y = 0.299 * r + 0.587 * g + 0.114 * b;
        float pb = -0.168736 * r - 0.331264 * g + 0.5 * b;
        float pr = 0.5 * r -0.418688 * g -0.081312 * b;
        
        vc.y = y;
        vc.pb = pb;
        vc.pr = pr;
        *(vid_color *)UArray2b_at(mycl->uarray2b, col, row) = vc;
}

/* vid_to_rgb
 * Purpose: Converts the pixels in the uarray2b from component-video color into
 * rgb values
 * Arguments: uarray2b with vid_color structs
 * Return Value: updated pnm_ppm struct 
*/
Pnm_ppm vid_to_rgb(UArray2b_T uarray2b) {
        int height = UArray2b_height(uarray2b);
        int width = UArray2b_width(uarray2b);
        UArray2_T uarray2 = UArray2_new(width, height, sizeof(struct Pnm_rgb));
        int denominator = 50000;
        closure2 mycl = { uarray2, denominator };
        
        UArray2b_map(uarray2b, vid_to_rgb_apply, &mycl);
        
        struct Pnm_ppm *pixmap_p = malloc(sizeof(struct Pnm_ppm));
        pixmap_p->width = width;
        pixmap_p->height = height;
        pixmap_p->denominator = denominator;
        pixmap_p->pixels = uarray2;
        pixmap_p->methods = uarray2_methods_plain;
        return pixmap_p;
}

/* vid_to_rgb_apply
 * Purpose: Performs calculations necessary to convert from vid_color to rgb
 * Arguments: ints for row and col number, uarray2, element, and cl which has 
 * uarray2 we want to populate and the denominator of the ppm image
 * Return Value: void
*/
void vid_to_rgb_apply(int col, int row, UArray2b_T uarray2b, void *elem,
                      void *cl) {
        (void) uarray2b;
        closure2 *mycl = cl;
        vid_color *vid_struct = elem;
        
        float my_y = vid_struct->y;
        float my_pb = vid_struct->pb;
        float my_pr = vid_struct->pr;
        int r = (1.0 * my_y + 0.0 * my_pb + 1.402 * my_pr) * mycl->denominator;
        int g = (1.0 * my_y - 0.344136 * my_pb - 0.714136 * my_pr) 
                * mycl->denominator;
        int b = (1.0 * my_y + 1.772 * my_pb + 0.0 * my_pr) * mycl->denominator;
        
        if (r < 0)
                r = 0;
        if (g < 0)
                g = 0;
        if (b < 0)
                b = 0;

        struct Pnm_rgb rgb_struct = { r, g, b };
        *(struct Pnm_rgb *)UArray2_at(mycl->uarray2, col, row) = rgb_struct;
}
