#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toy.h"
#include "toy.c"

int main()
{
    short A[TK_K * TK_K * TK_N], t[TK_K * TK_N]; // public key
    short s[TK_K * TK_N];                        // private key

    GEN(A, t, s);

    for (int msg = 0; msg < 16; msg++){
        short u[TK_K * TK_N], v[TK_N]; // ciphertext
        ENC(A, t, msg, u, v);
        short m2 = DEC(s, u, v);
        printf("%2d %2d ", msg, m2);
        printBin(msg, TK_N);
        printf(" ");
        printBin(m2, TK_N);
        printf("   ");
        printBin(msg ^ m2, TK_N);
        printf("\n");
    }
    printf("Done...\n");
    return 0;
}