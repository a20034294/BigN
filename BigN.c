#include "BigN.h"

#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/printk.h>

#define BLOCK_SIZE 9
#define BLOCK_MAX 1000000000

void initBigN(BigN *target, int size) {
    if (size) {
        target->val = kcalloc(size, sizeof(int), GFP_KERNEL);
    }
    target->size = size;

    target->new = newBigN;
    // target->init = initBigN;
    // target->parse = parseBigN;
    target->print = printBigN;
    target->resize = resizeBigN;
    target->add = addBigN;
}

BigN *newBigN(int size) {
    BigN *ret = kmalloc(sizeof(BigN), GFP_KERNEL);
    initBigN(ret, size);
    return ret;
}

void freeBigN(BigN *target) {
    if (!target) return;
    if (target->size) kfree(target->val);
    kfree(target);
}

void resizeBigN(BigN *target, int size) {
    if (target->size == size) return;

    if (size == 0) {
        if (target->size) {
            kfree(target->val);
        }
        target->val = NULL;
        target->size = size;
        return;
    }
    int *val = kcalloc(sizeof(int), size, GFP_KERNEL);

    for (int i = 0; i < size && i < target->size; i++) {
        val[i] = target->val[i];
    }
    if (target->size) {
        kfree(target->val);
    }
    target->size = size;
    target->val = val;
}

int sprintBigN(BigN *this, char *s) {
    int i, len;
    if (!this->size) {
        len = sprintf(s, "0");
        return len;
    }
    for (i = this->size - 1; i > 0 && this->val[i] == 0; i--)
        ;

    len = sprintf(s, "%d", this->val[i]);
    while (i > 0) {
        len += sprintf(s + len, "%09d", this->val[--i]);
    }
    return len;
}

void printBigN(BigN *this) {
    char *s = kmalloc(this->size * BLOCK_SIZE + 1, GFP_KERNEL);
    sprintBigN(this, s);
    printk(KERN_INFO "%s\n", s);
}

BigN *parseBigN(char *s) {
    int len = strlen(s) - 1, offset;
    if (len == -1) return newBigN(0);

    BigN *ret = newBigN(len / BLOCK_SIZE + 1);
    char tmp[BLOCK_SIZE + 1] = "";

    for (int i = 0; i < ret->size; i++) {
        ret->val[i] = 0;
    }

    strncpy(tmp, s, (len % BLOCK_SIZE) + 1);

    ret->val[len / BLOCK_SIZE] = simple_strtol(tmp, NULL, 10);
    offset = (len % BLOCK_SIZE) + 1;
    for (int i = len / BLOCK_SIZE - 1; i >= 0; i--) {
        strncpy(tmp, s + offset, BLOCK_SIZE);
        tmp[BLOCK_SIZE] = '\0';
        ret->val[i] = simple_strtol(tmp, NULL, 10);
        // printf("%d ", ret->val[i]);
        offset += BLOCK_SIZE;
    }
    return ret;
}

BigN *addBigN(BigN *a, BigN *b) {
    BigN *ret = newBigN(a->size > b->size ? a->size : b->size);
    // 我先不管負號 我好爛

    int carry = 0;
    for (int i = 0; i < ret->size; i++) {
        ret->val[i] = (i < a->size ? a->val[i] : 0) +
                      (i < b->size ? b->val[i] : 0) + carry;
        carry = 0;
        if (ret->val[i] >= BLOCK_MAX) {
            ret->val[i] -= BLOCK_MAX;
            carry = 1;
        }
    }
    if (carry) {
        ret->resize(ret, ret->size + 1);
        ret->val[ret->size - 1] = 1;
    }

    return ret;
}

BigN *addToBigN(BigN *a, BigN *b) {
    int carry = 0;
    a->resize(a, a->size > b->size ? a->size : b->size);

    for (int i = 0; i < a->size; i++) {
        a->val[i] = (i < a->size ? a->val[i] : 0) +
                    (i < b->size ? b->val[i] : 0) + carry;
        carry = 0;
        if (a->val[i] >= BLOCK_MAX) {
            a->val[i] -= BLOCK_MAX;
            carry = 1;
        }
    }
    if (carry) {
        a->resize(a, a->size + 1);
        a->val[a->size - 1] = 1;
    }
    return a;
}

void susToBigN(BigN *a, BigN *b) {  // a-=b;
    // 我先不管負號 我好爛

    int borrow = 0;
    for (int i = 0; i < a->size; i++) {
        a->val[i] = (i < a->size ? a->val[i] : 0) -
                    (i < b->size ? b->val[i] : 0) - borrow;
        borrow = 0;
        if (a->val[i] < 0) {
            a->val[i] += BLOCK_MAX;
            borrow = 1;
        }
    }
    if (borrow) {
        printk(KERN_ERR "出現負數了幹\n");
        //exit(0);
    }
}

BigN *leftshiftBlockBigN(BigN *src, int num) {
    if (num < 0) {
        printk(KERN_ERR "left shift must > 0\n");
        //exit(0);
    }

    BigN *ret = newBigN(src->size + num);
    for (int i = 0; i < src->size; i++) {
        ret->val[i + num] = src->val[i];
    }
    return ret;
}

/**
 * split()
 * @src: src to be splited
 * @l
 * @r
 * [l, r)
 */
BigN *split(BigN *src, int l, int r) {
    BigN *ret = newBigN(r - l);
    int p = 0;
    for (int i = l; i < r; i++) {
        ret->val[p] = src->val[i];
        p++;
    }
    return ret;
}

int isZero(BigN *src) {
    if (!src->size) return 1;
    for (int i = 0; i < src->size; i++) {
        if (src->val[i]) return 0;
    }
    return 1;
}

BigN *karatsuba(BigN *x, BigN *y) {
    BigN *a, *b, *c, *d, *ac, *bd, *acs, *apb, *cpd, *tmp, *tmp2;

    if (x->size == 1 && y->size == 1) {
        char tmp_str[20];
        sprintf(tmp_str, "%lld", (long long)x->val[0] * (long long)y->val[0]);
        return parseBigN(tmp_str);
    }

    a = split(x, x->size >> 1, x->size);
    b = split(x, 0, x->size >> 1);
    c = split(y, y->size >> 1, y->size);
    d = split(y, 0, y->size >> 1);

    apb = addBigN(a, b);
    cpd = addBigN(c, d);
    ac = mulBigN(a, c);
    bd = mulBigN(b, d);
    acs = leftshiftBlockBigN(ac, (x->size >> 1) << 1);

    tmp = mulBigN(apb, cpd);

    freeBigN(apb);
    freeBigN(cpd);

    susToBigN(tmp, ac);
    susToBigN(tmp, bd);

    tmp2 = leftshiftBlockBigN(tmp, x->size >> 1);
    freeBigN(tmp);

    acs = addToBigN(acs, tmp2);
    acs = addToBigN(acs, bd);

    freeBigN(ac);
    freeBigN(bd);
    freeBigN(tmp2);
    return acs;
}

BigN *mulBigN(BigN *a, BigN *b) {
    if (isZero(a) || isZero(b)) {
        return newBigN(0);
    }
    if (a->size > b->size) {
        b->resize(b, a->size);
    } else {
        a->resize(a, b->size);
    }
    BigN *ret = karatsuba(a, b);

    return ret;
}
