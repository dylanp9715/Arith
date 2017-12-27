/* 
 * pack_or_unpack.c
 * by Derek Sims and Dylan Park
 * HW 4
 *
 * Packs blocks when compressing and unpacks the code words when decompressing
 *  
 */

#include "pack_or_unpack.h"

int BLOCKSIZE = 2;

/* code_float
 * Purpose: Codes the passed-in float to convert it into the signed-integer
 * set of -15 to 15
 * Arguments: float to be coded
 * Return Value: coded int64_t 
*/
int64_t code_float(float f) {
        float abs_value_f = fabsf(f);
        int64_t coded = 0;
        if (abs_value_f > 0.29) {
                coded = 30;
        } else if (abs_value_f != 0) {
                coded = ceilf(abs_value_f * 100);
        }
        if (f < 0) {
                coded = coded * -1;
        }
        return coded;
}

/* avg_chroma
 * Purpose: Calculates the average chroma of the four pixels in a block at
 * specified index
 * Arguments: UArray2b_T with pixels, ints i and j to specify upper left most 
 * coordinate of current block, and pointer to pb_avg and pr_avg to allow us to
 * change the value
 * Return Value: void
*/
void avg_chroma(UArray2b_T uarray2b, int i, int j, float *pb_avg, float *pr_avg){
        float pb_sum = 0;
        float pr_sum = 0;
        for (int row = j; row < BLOCKSIZE + j; ++row) {
                for (int col = i; col < BLOCKSIZE + i; ++col) {
                        vid_color vc = *(vid_color *)UArray2b_at(uarray2b, col,
                                                                 row);
                        pb_sum += vc.pb;
                        pr_sum += vc.pr;
                }
        }
        *pb_avg = pb_sum / (BLOCKSIZE * BLOCKSIZE);
        *pr_avg = pr_sum / (BLOCKSIZE * BLOCKSIZE);
}

/* dct
 * Purpose: Transforms the four Y values of pixels into cosine coefficients
 * a, b, c, and d. Also, calculates index_pb and index_pr.
 * Arguments: UArray2b_T and indices indicating upper left-most coordinate of
 * block
 * Return Value: updated coded elements struct
*/
coded_elements dct(UArray2b_T uarray2b, int i, int j) {
        vid_color vc1 = *(vid_color *)UArray2b_at(uarray2b, i ,j);
        float y1 = vc1.y;
        vid_color vc2 = *(vid_color *)UArray2b_at(uarray2b, i + 1 , j);
        float y2 = vc2.y;
        vid_color vc3 = *(vid_color *)UArray2b_at(uarray2b, i, j + 1);
        float y3 = vc3.y;
        vid_color vc4 = *(vid_color *)UArray2b_at(uarray2b, i + 1, j + 1);
        float y4 = vc4.y;

        coded_elements ce;
        float a = (y4 + y3 + y2 + y1) / 4;
        float b = (y4 + y3 - y2 - y1) / 4;
        float c = (y4 - y3 + y2 - y1) / 4;
        float d = (y4 - y3 - y2 + y1) / 4;

        ce.a = round(a * 63);
        ce.b = code_float(b);
        ce.c = code_float(c);
        ce.d = code_float(d);
        //(void) a;

        float pb_avg = 0;
        float pr_avg = 0;
        avg_chroma(uarray2b, i, j, &pb_avg, &pr_avg);
        unsigned index_pb = Arith40_index_of_chroma(pb_avg);
        unsigned index_pr = Arith40_index_of_chroma(pr_avg);
        ce.index_pb = index_pb;
        ce.index_pr = index_pr;

        return ce;
}

/* pack_block_at
 * Purpose: Packs a, b, c, d, index_pb, and index_pr into a 32-bit word
 * Arguments: UArray2b_T and indices to pass into dct
 * Return Value: code word
*/
uint64_t pack_block_at(UArray2b_T uarray2b, int i , int j) {
        coded_elements ce = dct(uarray2b, i, j);
        uint64_t word = 0;
        word = Bitpack_newu(word, 6, 26, ce.a);
        word = Bitpack_news(word, 6, 20, ce.b);
        word = Bitpack_news(word, 6, 14, ce.c);
        word = Bitpack_news(word, 6, 8, ce.d);
        word = Bitpack_newu(word, 4, 4, ce.index_pb);
        word = Bitpack_newu(word, 4, 0, ce.index_pr);
        return word;
}

/* pack_blocks
 * Purpose: Calls pack_block_at to retrieve the 32-bit code word and places 
 * into the word_array
 * Arguments: UArray2b_T and word_array
 * Return Value: void
*/
void pack_blocks(UArray2b_T uarray2b, uint64_t word_array[]) {
        int height = UArray2b_height(uarray2b);
        int width = UArray2b_width(uarray2b);

        int index = 0;
        for (int j = 0; j < height; j += BLOCKSIZE) {
                for (int i = 0; i < width; i += BLOCKSIZE) {
                        word_array[index] = pack_block_at(uarray2b, i, j);
                        index++;
                }
        }
}

/* unpack_values
 * Purpose: Unpacks values a, b, c, d, index_pb, and index_pr from code word
 * Arguments: code word
 * Return Value: updated coded elements struct
*/
coded_elements unpack_values(uint64_t word) {
        coded_elements ce;
        ce.a = Bitpack_getu(word, 6, 26);
        ce.b = Bitpack_gets(word, 6, 20);
        ce.c = Bitpack_gets(word, 6, 14);
        ce.d = Bitpack_gets(word, 6, 8);
        ce.index_pb = Bitpack_getu(word, 4, 4);
        ce.index_pr = Bitpack_getu(word, 4, 0);
        return ce;
}

/* uncode_int
 * Purpose: Converts coded int back to float form
 * Arguments: int64_t to be uncoded
 * Return Value: uncoded float
*/
float uncode_int(int64_t i) {
        int64_t abs_value_i = i;
        float uncoded = 0;
        if (i < 0) {
                abs_value_i = abs_value_i * - 1;
        }
        if (i != 0) {
                uncoded = ((float) abs_value_i / 100) - .01;
        }
        if (i < 0) {
                uncoded = uncoded * -1;
        }
        return uncoded;
}

/* inverse_dct
 * Purpose: Compute y1, y2, y3, and y4 from the elements in coded_elements
 * Arguments: UArray2b_T, coded elements struct, and indices of block
 * Return Value: void
*/
void inverse_dct(UArray2b_T uarray2b, coded_elements ce, int i, int j) {
        float a = (float) ce.a / 63;
        float b = uncode_int(ce.b);
        float c = uncode_int(ce.c);
        float d = uncode_int(ce.d);

        float pb = Arith40_chroma_of_index(ce.index_pb);
        float pr = Arith40_chroma_of_index(ce.index_pr);
        
        float y1 = a - b - c + d;
        float y2 = a - b + c - d;
        float y3 = a + b - c - d;
        float y4 = a + b + c + d;
        
        vid_color v1 = { .y = y1, .pb = pb, .pr = pr };
        vid_color v2 = { .y = y2, .pb = pb, .pr = pr };
        vid_color v3 = { .y = y3, .pb = pb, .pr = pr };
        vid_color v4 = { .y = y4, .pb = pb, .pr = pr };
        
        *(vid_color *)UArray2b_at(uarray2b, i ,j) = v1;
        *(vid_color *)UArray2b_at(uarray2b, i + 1 , j) = v2;
        *(vid_color *)UArray2b_at(uarray2b, i ,j + 1) = v3;
        *(vid_color *)UArray2b_at(uarray2b, i + 1, j + 1) = v4;
}

/* unpack_words
 * Purpose: Calls unpack_values and inverse_dct to update uarray2b with 
 * vid_color structs
 * Arguments: word_array and UArray2b_T
 * Return Value: void
*/
void unpack_words(uint64_t word_array[], UArray2b_T uarray2b) {
    int index = 0;
    for (int j = 0; j < UArray2b_height(uarray2b); j += BLOCKSIZE) {
                for (int i = 0; i < UArray2b_width(uarray2b); i += BLOCKSIZE) {
                        coded_elements ce = unpack_values(word_array[index]);
                        inverse_dct(uarray2b, ce, i, j);
                        index++;
                }
    }
}