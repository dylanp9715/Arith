/* 
 * process_image.c
 * by Derek Sims and Dylan Park
 * HW 4
 *
 * 
 */

#include "process_compressed.h"

/* read_header
 * Purpose: Reads the header of the compressed file, ensures that it is of the
 * right format, and retrieves the height and width of image
 * Arguments: file pointer and pointers to width and height to allow us to 
 * change its values
 * Return Value: void
*/
void read_header(FILE *in, unsigned *width, unsigned *height) {
        int read = fscanf(in, "COMP40 Compressed image format 2\n%u %u", width
                , height); 
        assert(read == 2);
        int c = getc(in);
        assert(c == '\n');

}

/* print_codeword
 * Purpose: Prints the passed-in code word in big-endian order 
 * Arguments: code word
 * Return Value: void
*/
void print_codeword(uint64_t word) {
        char byte_array[4];
        for (int i = 0; i < 4; ++i) {
                unsigned byte = Bitpack_getu(word, 8, 24 - (8 * i));
                byte_array[i] = byte;
        }
        for (int j = 0; j < 4; ++j) {
                putchar(byte_array[j]);
        }
}

/* print_codewords
 * Purpose: Calls print_codeword to print out passed in code word
 * Arguments: word_array and int specifying number of words in array
 * Return Value: void 
*/
void print_codewords(uint64_t word_array[], int num_words) {
        for (int i = 0; i < num_words; ++i) {
                print_codeword(word_array[i]);
        }
}

/* read_codewords
 * Purpose: Reads code words that are in big-endian order and places them into
 * word_array
 * Arguments: file pointer, word_array, and number of words
 * Return Value: void
*/
void read_codewords(FILE *fp, uint64_t word_array[], int num_words) {
        for (int i = 0; i < num_words; ++i) {
                unsigned char byte_array[4];
                for (int j = 0; j < 4; ++j) {
                        unsigned char byte = getc(fp);
                        byte_array[j] = byte;
                }
                uint64_t newu = 0;
                for (int k = 0; k < 4; ++k) {
                        newu = Bitpack_newu(newu, 8, 24 - (8 * k), byte_array[k]);
                }
                word_array[i] = newu;
        }
}
