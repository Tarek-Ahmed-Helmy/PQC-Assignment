#include <stdio.h>
#include <math.h>
#include "toy.h"
#include "toy.c"


int modExp(int base, int exponent, int modulus){
    int result = 1;
    while (exponent > 0){
        if (exponent % 2 == 1){
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exponent /= 2;
    }
    return result;
}

int reverseBits(int index, int bits){
    int reversed_index = 0;
    for (int i = 0; i < bits; ++i){
        reversed_index = (reversed_index << 1) | (index & 1);
        index >>= 1;
    }
    return reversed_index;
}

void permuteBitReverse(short *src, short *dest){
    int bits = 0;
    int temp_size = VEC_SIZE;
    while (temp_size > 1){
        temp_size >>= 1;
        bits++;
    }

    for (int i = 0; i < VEC_SIZE; ++i){
        int reversed_index = reverseBits(i, bits);
        dest[reversed_index] = src[i];
    }
}

void ntt(int *data, int forward){
    int x[TK_N];

    permuteBitReverse(data, x);

    int factors[TK_N];
    int factor = 1;

    if (forward){
        for (int i = 0; i < TK_N; ++i)
        {
            factors[i] = mod_exp(TK_SQRT_W, i, TK_Q);
            x[i] = (x[i] * factors[i]) % TK_Q;
        }
    }
    else{
        for (int i = 0; i < TK_N; ++i){
            factors[i] = mod_exp(-TK_SQRT_W, i, TK_Q);
            x[i] = (x[i] * factors[i]) % TK_Q;
        }
    }

    for (int s = 1; s <= log2(TK_N); ++s){
        int m = 1 << s;
        for (int b = 0; b < TK_N; b += m){
            factor = 1;
            for (int op = 0; op < m / 2; ++op){
                int a0 = x[b + op];
                int a1 = (x[b + op + m / 2] * factor) % TK_Q;
                x[b + op] = (a0 + a1) % TK_Q;
                x[b + op + m / 2] = (a0 - a1 + TK_Q) % TK_Q;
                factor = (factor * TK_W) % TK_Q;
            }
        }
    }

    for (int i = 0; i < TK_N; ++i){
        data[i] = x[i];
    }
}