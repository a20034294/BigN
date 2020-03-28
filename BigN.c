#include "BigN.h"
#include <string.h>
#define BLOCK_SIZE 9
#define BLOCK_MAX 1000000000


void init(BigN *target, int size)
{
    target->val = calloc(size, sizeof(int));
    target->size = size;
}

void print(BigN *this)
{
    int i;
    for (i = this->size - 1; i >= 0 && this->val[i] == 0; i--);
    printf("%d", this->val[i]);
    while (i) {
        printf(" %09d", this->val[--i]);
    }
    printf("\n");
}

void parse(BigN *this, char *s)
{
    int len = strlen(s), offset = 0;
    char tmp[15] = "";
    for (int i = 0; i < this->size; i++) {
        this->val[i] = 0;
    }

    strncpy(tmp, s, len % BLOCK_SIZE);

    this->val[(len - 1) / BLOCK_SIZE] = atoi(tmp);
    offset += len % BLOCK_SIZE;
    for (int i = (len - 1) / BLOCK_SIZE - 1; i >= 0; i--) {
        strncpy(tmp, s + offset, BLOCK_SIZE);
        tmp[BLOCK_SIZE] = '\0';
        this->val[i] = atoi(tmp);
        // printf("%d ", this->val[i]);
        offset += BLOCK_SIZE;
    }
}

BigN* add(BigN *a, BigN *b)
{
    BigN *ret = malloc(sizeof(BigN));
    // 我先不管 a 跟 b 的 size 也不管負號 我好爛

    init(ret, a->size > b-> size ? a->size : b->size);

    int carry = 0;
    for (int i = 0; i < ret->size; i++) {
        ret->val[i] = a->val[i] + b->val[i] + carry;
        carry = 0;
        if (ret->val[i] >= BLOCK_MAX) {
            ret->val[i] -= BLOCK_MAX;
            carry = 1;
        }
    }

    return ret;
}

BigN* split(BigN *x, int l, int r)
{
    BigN *ret = malloc(sizeof(BigN));
    init(ret, x->size);
    int p = 0;
    for (i = l; i < r; i++) {
        ret->val[p++] = x->val[i];
    }
}

BigN* karatsuba(BigN *x, BigN *y)
{
    int mid = (l + r) >> 1;
    long long tmp;
    BigN *ret = malloc(sizeof(BigN));
    init(ret, x->size);

    if () {
        tmp = (long long) a
        return ret;
    }
}

BigN* mul(BigN *a, BigN *b) {

}

