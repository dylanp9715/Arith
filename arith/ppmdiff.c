/* 
 * ppmdiff.c
 * by Derek Sims and Dylan Park
 * 
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "assert.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"

/* To do:
        Take in 2 filename images from command line
        For each file
                Open it
                read into a uarray2
        Find min width and min height
        Iterate through images comparing each pixel
        Calculate root mean square diff for pixel values
        Print to stdout with 4 digits after decimal
*/

int main(int argc, char *argv[])
{
        //assert(argc == 3);
        //FILE *fp1;
        //FILE *fp2;
        FILE *filearray[2];
        Pnm_ppm pnm_struct_array[2];
        A2Methods_T methods = uarray2_methods_plain;
        for (int i = 1; i < argc; ++i) {
                if (strcmp(argv[i], "-") == 0) {
                        filearray[i-1] = stdin;
                        pnm_struct_array[i-1] = Pnm_ppmread(filearray[i-1], 
                                methods);
                } else {
                        //printf("opening file\n");
                        filearray[i-1] = fopen(argv[i], "rb");
                        //printf("file open\n");
                        pnm_struct_array[i-1] = Pnm_ppmread(filearray[i-1], 
                                methods);
                        //printf("made pnm_struct\n");
                }
        }
        int height_diff = (pnm_struct_array[0]->height) - 
                (pnm_struct_array[1]->height);
        int width_diff = (pnm_struct_array[0]->width) - 
                (pnm_struct_array[1]->width);
        double root_mean = 0;
        if (!(height_diff >= -1 && height_diff <= 1) || !(width_diff >= -1 && 
                                                  width_diff <= 1)) {
                fprintf(stderr, "Difference between two images is more than 1\n");
                fprintf(stderr, "1.0\n");
        } else {
                int min_height = pnm_struct_array[0]->height;
                int min_width = pnm_struct_array[0]->width;
                if ((int)pnm_struct_array[1]->height < min_height) {
                        min_height = pnm_struct_array[1]->height;
                }
                if ((int)pnm_struct_array[1]->width < min_width) {
                        min_width = pnm_struct_array[1]->width;
                }
                double total_diff = 0;
                for (int j = 0; j < min_height; ++j) {
                        for (int i = 0; i < min_width; ++i) {
                                Pnm_rgb pix1 = methods->at(pnm_struct_array[0]->pixels 
                                        , i, j);
                                Pnm_rgb pix2 = methods->at(pnm_struct_array[1]->pixels 
                                        , i, j);
                                double r1 = (double)(pix1->red)/(double)pnm_struct_array[0]->denominator;
                                double r2 = (double)(pix2->red)/(double)pnm_struct_array[1]->denominator;
                                double g1 = (double)(pix1->green)/(double)pnm_struct_array[0]->denominator;
                                double g2 = (double)(pix2->green)/(double)pnm_struct_array[1]->denominator;
                                double b1 = (double)(pix1->blue)/(double)pnm_struct_array[0]->denominator;
                                double b2 = (double)(pix2->blue)/(double)pnm_struct_array[1]->denominator;
                                double curr_diff = (((r1 - r2) * (r1 - r2)) +
                                        ((g1 - g2) * (g1 - g2)) + 
                                        ((b1 - b2) * (b1 - b2)));
                                //printf("g1: %lg, g2: %lg\n", g1, g2);
                                total_diff = total_diff + curr_diff;
                                //printf("Total diff: %lg\n", total_diff);
                        }
                }
                //printf("total diff outside first for: %lg\n", total_diff);
                root_mean = sqrt((total_diff) / (double)(3 * min_width * 
                        min_height));
        }
        printf("%.4f\n", root_mean);
        fclose(filearray[0]);
        fclose(filearray[1]);
        return 0;
}