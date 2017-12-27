/* 
 * compress40.c
 * by Derek Sims and Dylan Park
 * HW 4
 * 
 * Compresses or decompresses the passed-in file depending on user input
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "compress40.h"
#include "uarray2.h"
#include "uarray2b.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "assert.h"
#include "rgb_vid_conversion.h"
#include "process_compressed.h"
#include "pack_or_unpack.h"

/* trim_dim
 * Purpose: Trims the dimensions of the ppm image if the height or width are
 * odd values
 * Arguments: ppm_struct
 * Return Value: void
*/
void trim_dim(Pnm_ppm ppm_struct) {
        int new_width = ppm_struct->width;
        int new_height = ppm_struct->height;
        
        if (ppm_struct->width % 2 != 0) {
                new_width = ppm_struct->width - 1;
        }
        if (ppm_struct->height % 2 != 0) {
                new_height = ppm_struct->height - 1;
        }

        UArray2_T new_uarray2 = UArray2_new(new_width, new_height, 
                sizeof(struct Pnm_rgb));
        
        for (int j = 0; j < new_height; ++j) {
                for (int i = 0; i < new_width; ++i) {
                        struct Pnm_rgb *curr_element = 
                                UArray2_at(ppm_struct->pixels, i, j);
                        *(struct Pnm_rgb *)UArray2_at(new_uarray2, i, j) = 
                                *curr_element;
                }
        }
        ppm_struct->methods->free(&(ppm_struct->pixels));
        ppm_struct->width = new_width;
        ppm_struct->height = new_height;
        ppm_struct->pixels = new_uarray2;
}

/* compress40
 * Purpose: Compresses an image that is pointed to by passed-in file pointer
 * Arguments: file pointer
 * Return Value: void
*/
void compress40(FILE *fp) {
        A2Methods_T methods = uarray2_methods_plain;
        assert(methods);
        Pnm_ppm ppm_struct = Pnm_ppmread(fp, methods);
        if ((ppm_struct->height % 2 != 0) || (ppm_struct->width % 2 != 0)) {
                trim_dim(ppm_struct);
        }
        unsigned width = ppm_struct->width;
        unsigned height = ppm_struct->height;
        UArray2b_T new_uarray2b = rgb_to_vid(ppm_struct);
        
        int num_words = (width * height) / (BLOCKSIZE * BLOCKSIZE);
        uint64_t word_array[num_words];
        pack_blocks(new_uarray2b, word_array);

        printf("COMP40 Compressed image format 2\n%u %u", width, height);
        printf("\n");

        print_codewords(word_array, num_words);

        UArray2b_free(&new_uarray2b);
        Pnm_ppmfree(&ppm_struct);
}

/* decompress
 * Purpose: Decompresses the compressed image passed in via file pointer
 * Arguments: file pointer
 * Return Value: void
*/
void decompress40(FILE *fp) {
        unsigned width, height;
        read_header(fp, &width, &height);
        
        int num_words = height * width / (BLOCKSIZE * BLOCKSIZE);
        uint64_t word_array[num_words];
        
        read_codewords(fp, word_array, num_words);
        UArray2b_T new_uarray2b = UArray2b_new(width, height, 
                                               sizeof(vid_color), BLOCKSIZE);        

        unpack_words(word_array, new_uarray2b);
        Pnm_ppm new_ppm_struct = vid_to_rgb(new_uarray2b);
        Pnm_ppmwrite(stdout, new_ppm_struct);

        UArray2b_free(&new_uarray2b);
        Pnm_ppmfree(&new_ppm_struct);
}