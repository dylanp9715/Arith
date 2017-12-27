/* 
 * pack_or_unpack.h
 * by Derek Sims and Dylan Park
 * HW 4
 *
 * Interface for the functions that packs blocks or unpacks words
 * 
 */

#ifndef PACK_OR_UNPACK_INCLUDED
#define PACK_OR_UNPACK_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "rgb_vid_conversion.h"
#include "arith40.h"
#include "bitpack.h"

extern int BLOCKSIZE;

typedef struct {
        uint64_t a;
        int64_t b;
        int64_t c;
        int64_t d;
        unsigned index_pb;
        unsigned index_pr;
} coded_elements;

/* packs blocks from uarray2b and puts code words into word_array*/
void pack_blocks(UArray2b_T uarray2b, uint64_t word_array[]);
/* unpacks words from word_array and places the coded elements into vid_color
   pixels placed into uarray2b */
void unpack_words(uint64_t word_array[], UArray2b_T uarray2b);

#endif