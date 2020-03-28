#ifndef BIGN_H
#define BIGN_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _BigN {
    int size;
    int *val;
    void (*init)(struct _BigN*, int);
} BigN;
void init(BigN* target, int size);
void parse(BigN *this, char *s);
void print(BigN *this);
BigN* add(BigN *a, BigN *b);

#endif
