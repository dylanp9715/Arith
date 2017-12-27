#include "pack_or_unpack.h"
#include <inttypes.h>
#include "assert.h"

void print2(UArray2b_T uarray2b);

int main(int argc, char *argv[]) {
        (void) argc;
        A2Methods_T methods = uarray2_methods_plain;
        assert(methods);
        FILE *fp = fopen(argv[1], "r");
        Pnm_ppm ppm_struct = Pnm_ppmread(fp, methods);
        //print(ppm_struct->pixels, ppm_struct->denominator);
        UArray2b_T uarray2b = rgb_to_vid(ppm_struct);
        uint64_t word_array[4];
        pack_blocks(uarray2b, word_array);
        UArray2b_T new_uarray2b = UArray2b_new(2, 8, 
                                               sizeof(vid_color), 2);
        unpack_words(word_array, new_uarray2b);
        print2(uarray2b);
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