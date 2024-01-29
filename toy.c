#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toy.h"

static void toyFillSmall(short *buf, int n){
    for (int k = 0; k < n; k++){
        short val = rand() & 0xF;
        val = (val >> 1 & 1) - (val & 1);
        if (val < 0)
            val += TK_Q;
        buf[k] = val;
    }
}

void toyPolyMulNaive(short *dst, const short *a, const short *b, int add){
    dst[0] = ((dst[0] & -add) + a[0] * b[0] + NEG(a[3]) * b[1] + NEG(a[2]) * b[2] + NEG(a[1]) * b[3]) % TK_Q;
    dst[1] = ((dst[1] & -add) + a[1] * b[0] + a[0] * b[1] + NEG(a[3]) * b[2] + NEG(a[2]) * b[3]) % TK_Q;
    dst[2] = ((dst[2] & -add) + a[2] * b[0] + a[1] * b[1] + a[0] * b[2] + NEG(a[3]) * b[3]) % TK_Q;
    dst[3] = ((dst[3] & -add) + a[3] * b[0] + a[2] * b[1] + a[1] * b[2] + a[0] * b[3]) % TK_Q;
}

static void toyMul_mv(short *dst, const short *mat, const short *vec){
    memset(dst, 0, TK_K * TK_N * sizeof(short));
    for (int kv = 0, idx = 0; kv < TK_K * TK_N; kv += TK_N)
    {
        for (int k = 0; k < TK_K * TK_N; k += TK_N, idx += TK_N)
            toyPolyMulNaive(dst + kv, mat + idx, vec + k, 1);
    }
}

static void toyMul_mTv(short *dst, const short *mat, const short *vec){
    memset(dst, 0, TK_K * TK_N * sizeof(short));
    for (int kv = 0; kv < TK_K * TK_N; kv += TK_N)
    {
        for (int k = 0; k < TK_K * TK_N; k += TK_N)
            toyPolyMulNaive(dst + kv, mat + TK_K * k + kv, vec + k, 1);
    }
}

static void toyDot(short *dst, const short *v1, const short *v2){
    memset(dst, 0, TK_N * sizeof(short));
    for (int k = 0; k < TK_K * TK_N; k += TK_N)
        toyPolyMulNaive(dst, v1 + k, v2 + k, 1);
}

static void toyAdd(short *dst, const short *v1, const short *v2, int count, int v2_neg){
    for (int k = 0; k < count; ++k)
    {
        short val = v2[k];
        if (v2_neg)
            val = NEG(val);
        dst[k] = (v1[k] + val) % TK_Q;
    }
}

void GEN(short *A, short *t, short *s){
    short e[TK_K * TK_N];
    for (int k = 0; k < TK_K * TK_K * TK_N; ++k)
        A[k] = rand() % TK_Q;
    toyFillSmall(s, TK_K * TK_N);
    toyFillSmall(e, TK_K * TK_N);
    toyMul_mv(t, A, s);
    toyAdd(t, t, e, TK_K * TK_N, 0);
}

void ENC(const short *A, const short *t, int plain, short *u, short *v){
    short r[TK_K * TK_N], e1[TK_K * TK_N], e2[TK_N];
    toyFillSmall(r, TK_K * TK_N);
    toyFillSmall(e1, TK_K * TK_N);
    toyFillSmall(e2, TK_N);

    toyMul_mTv(u, A, r);
    toyAdd(u, u, e1, TK_K * TK_N, 0);

    toyDot(v, t, r);
    toyAdd(v, v, e2, TK_N, 9);
    for (int k = 0; k < TK_N; ++k)
        v[k] = (v[k] + ((TK_Q >> 1) & -(plain >> k & 1))) % TK_Q;
}

int DEC(const short *s, const short *u, const short *v){
    short p[TK_N], plain;
    toyDot(p, s, u);
    toyAdd(p, v, p, TK_N, 1);
    plain = 0;
    for (int k = 0; k < TK_N; ++k){
        int val = p[k];
        if (val > TK_Q / 2)
            val -= TK_Q;
        printf("%5d ", val);
        int bit = abs(val) > TK_Q / 4;
        plain |= bit << k;
    }
    return plain;
}

void printBin(int n, int n_bits){
    for (int i = n_bits - 1; i >= 0; i--){
        putchar((n & (1 << i)) ? '1' : '0');
    }
}