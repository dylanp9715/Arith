#include <stdlib.h>
#include <stdio.h>
#include "bitpack.h"
#include <inttypes.h>

void printu64(uint64_t u); 
void prints64(int64_t s);

int main(int argc, char *argv[]) {
        (void) argc;
        (void) argv;
        uint64_t test, width, new_width;
        int64_t value;
        //uint64_t test, width;
        unsigned lsb, new_lsb;
        // while (1) {
        //         printf("Test\n");
        //         scanf("%" PRIu64, &test);
        //         printf("Width\n");
        //         scanf("%" PRIu64, &width);
        //         if (Bitpack_fitsu(test, width)) {
        //                 printf("Fits\n");
        //         } else {
        //                 printf("Doesn't fit\n");
        //         }
                
        // }
        /*while (1) {
                printf("Word\n");
                scanf("%" PRIu64, &test);
                printu64(test);
                printf("Width\n");
                scanf("%" PRIu64, &width);
                printf("Lsb\n");
                scanf("%u", &lsb);
                printf("Value\n");
                scanf("%" PRIu64, &value);
                printu64(value);
                uint64_t new_word = Bitpack_newu(test, width, lsb, value);
                printf("New word: %" PRIu64, new_word);
                printf("\n");
                printu64(new_word);
        }*/
        while (1) {
                printf("Word\n");
                scanf("%" PRIu64, &test);
                printu64(test);
                printf("Width\n");
                scanf("%" PRIu64, &width);
                printf("Lsb\n");
                scanf("%u", &lsb);
                printf("Value\n");
                scanf("%" PRId64, &value);
                printu64(value);
                uint64_t new_word = Bitpack_news(test, width, lsb, value);
                printf("New word: %" PRIu64, new_word);
                printf("\n");
                printu64(new_word);
                printf("Field lsb\n");
                scanf("%u", &new_lsb);
                printf("Field width\n");
                scanf("%" PRIu64, &new_width);
                int64_t field = Bitpack_gets(new_word, new_width, new_lsb);
                printf("Signed Field: %" PRId64, field);
                printf("\n");
                prints64(field);
                printf("\n");
                uint64_t field2 = Bitpack_getu(new_word, new_width, new_lsb);
                printf("Unsigned Field: %" PRIu64, field2);
                printf("\n");
                printu64(field2);
        }
        exit(EXIT_SUCCESS);
}


void printu64(uint64_t u) {
        uint64_t mask = 1;
        mask = mask << 63;
        for(int i = 0; i <= 63; i++) {
                if((mask >> i) & u) {
                        printf("1");
                } else {
                        printf("0");   
                }    
        }
        printf("\n");
}

void prints64(int64_t s) {
        uint64_t mask = 1;
        for (int n = 0; n < 63; n++) {
                mask = mask * 2;
        }
        for(int i = 0; i <= 63; i++) {
                if((mask >> i) & s) {
                        printf("1");
                } else {
                        printf("0");   
                }    
        }
        printf("\n");
}