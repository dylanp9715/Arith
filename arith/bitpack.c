/* 
 * bitpack.c
 * by Derek Sims and Dylan Park
 * HW 4
 * 
 * Manipulates bit fields to create or access parts of code word called word
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include "bitpack.h"
#include "assert.h"

Except_T Bitpack_Overflow = { "Overflow packing bits" };

/* bitpack_fitsu
 * Purpose: Checks if passed-in uint64_t fits in specified width
 * Arguments: uint64_t and width
 * Return Value: true if it does fit and false if it does not
*/
bool Bitpack_fitsu(uint64_t n, unsigned width) {
        uint64_t base = 1;
        for (unsigned i = 0; i < width; ++i) {
                base = base * 2;
        }
        if (n <= base - 1) {
                return true;
        } else {
                return false;
        }
}

/* bitpack_fitss
 * Purpose: Checks if passed-in int64_t fits in specified width
 * Arguments: int64_t and width
 * Return Value: true if it does fit and false if it does not
*/
bool Bitpack_fitss( int64_t n, unsigned width) {
        int base = 1;
        if (n < 0) {
                n = n * -1;
        }
        for (unsigned i = 0; i < width - 1; ++i) {
                base = base * 2;
        }
        if (n < base) {
                return true;
        } else {
                return false; 
        }
}

/* bitpack_getu
 * Purpose: Gets portion of word of specified length "width" 
 * Arguments: uint64_t, width, and lsb
 * Return Value: The desired portion of the code word
*/
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb) {
        assert(width <= 64);
        assert(width + lsb <= 64);
        uint64_t mask = ~0;
        mask = mask >> (64 - width) << lsb;
        uint64_t value = (mask & word);
        value  = value >> lsb;
        return value;
}

/* bitpack_gets
 * Purpose: Gets portion of word of specified length "width" 
 * Arguments: uint64_t, width, and lsb
 * Return Value: The desired portion of the code word
*/
 int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb) {
        assert(width <= 64);
        assert(width + lsb <= 64);

        uint64_t mask = 1;
        /* if negative */
        if ((mask << (lsb + width - 1)) & word) {
                mask = ~0;
                mask = mask >> (64 - width) << lsb;
                mask = ~mask;
                int64_t value = (mask | word);
                value = value >> lsb;
                return value;
        } else {
                mask = ~0;
                mask = mask >> (64 - width) << lsb;
                uint64_t value = (mask & word);
                value  = value >> lsb;
                int64_t signed_value = value;
                return signed_value;        
        }
 }

/* bitpack_newu
 * Purpose: Adds uint64_t value to the passed-in word at specified location 
 * Arguments: uint64_t, width, lsb, and value
 * Return Value: Updated code word
*/
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t 
                      value) {
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (!Bitpack_fitsu(value, width)) {
                RAISE(Bitpack_Overflow);
        }

        uint64_t mask = ~0;
        mask = mask >> (64 - width) << lsb;
        mask = ~mask;
        uint64_t newu = ((word & mask) | (value << lsb));
        return newu;
}

/* bitpack_news
 * Purpose: Adds int64_t value to the passed-in word at specified location 
 * Arguments: uint64_t, width, lsb, and value
 * Return Value: Updated code word
*/
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  
                      int64_t value) {
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (!Bitpack_fitss(value, width)) {
                RAISE(Bitpack_Overflow);
        }

        uint64_t mask = ~0;
        mask = mask >> (64 - width) << lsb;
        mask = ~mask;
        uint64_t value_mask = ~0;
        value_mask = value_mask >> (64 - width);
        value = (value & value_mask);
        uint64_t news = ((word & mask) | (value << lsb));
        return news;
}
