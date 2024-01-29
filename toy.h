#define TK_K 3
#define TK_N 4
#define TK_Q 97
#define TK_SQRT_W 33
#define TK_W 22
#define NEG(X) (TK_Q - (X))
#define VEC_SIZE (TK_K * TK_N)

static void toyFillSmall(short *buf, int n);
void toyPolyMulNaive(short *dst, const short *a, const short *b, int add);
static void toyMul_mv(short *dst, const short *mat, const short *vec);
static void toyMul_mTv(short *dst, const short *mat, const short *vec);
static void toyDot(short *dst, const short *v1, const short *v2);
static void toyAdd(short *dst, const short *v1, const short *v2, int count, int v2_neg);
void GEN(short *A, short *t, short *s);
void ENC(const short *A, const short *t, int plain, short *u, short *v);
int DEC(const short *s, const short *u, const short *v);
void printBin(int n, int n_bits);
