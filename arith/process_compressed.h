/* 
 * process_image.h
 * by Derek Sims and Dylan Park
 * HW 4
 *
 * 
 */

#ifndef PROCESS_COMPRESSED_INCLUDED
#define PROCESS_COMPRESSED_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "bitpack.h"
#include "assert.h"

/* Reads header of compressed file and retrieves the width and height of ppm */
void read_header(FILE *in, unsigned *width, unsigned *height);
/* Prints each code word in the word_array in big-endian order */
void print_codewords(uint64_t word_array[], int num_words);
/* Reads in code words printed in big-endian order and places into word_array*/
void read_codewords(FILE *fp, uint64_t word_array[], int num_words);

#endif