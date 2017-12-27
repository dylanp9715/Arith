#include <stdlib.h>
#include <stdio.h>
#include "rgb_vid_conversion.h"
#include "assert.h"

void print(UArray2_T uarray2, int denominator);
void print2(UArray2b_T uarray2b);

int main(int argc, char *argv[]) {
        (void) argc;
        A2Methods_T methods = uarray2_methods_plain;
        assert(methods);
        FILE *fp = fopen(argv[1], "r");
        Pnm_ppm ppm_struct = Pnm_ppmread(fp, methods);
        print(ppm_struct->pixels, ppm_struct->denominator);
        UArray2b_T uarray2b = rgb_to_vid(ppm_struct);
        ppm_struct = vid_to_rgb(uarray2b);
        printf("***********************************************\n");
        print(ppm_struct->pixels, ppm_struct->denominator);
}

void print(UArray2_T uarray2, int denominator) {
        for (int j = 0; j < UArray2_height(uarray2); ++j) {
                for (int i = 0; i < UArray2_width(uarray2); ++i) {
                        struct Pnm_rgb *curr_element = UArray2_at(uarray2, 
                                i, j);
                        printf("R: %f, G: %f, B: %f     ", (float)(curr_element->red)/denominator, 
                                (float)(curr_element->green)/denominator, (float)(curr_element->blue)/denominator);
                        printf("(%i, %i)\n", i, j);
                }
        }
}

void print2(UArray2b_T uarray2b) {
        for (int j = 0; j < UArray2b_height(uarray2b); ++j) {
                for (int i = 0; i < UArray2b_width(uarray2b); ++i) {
                        vid_color *curr_element = UArray2b_at(uarray2b, 
                                i, j);
                        printf("Y: %lg, Pb: %lg, Pr: %lg\n", curr_element->y, 
                                curr_element->pb, curr_element->pr);
                }
        }
}