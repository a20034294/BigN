#ifndef BIGN_H
#define BIGN_H


typedef struct _BigN {
    int size;
    int *val;

    // void (*init)(struct _BigN *, int);
    struct _BigN *(*new)(int);
    void (*print)(struct _BigN *);
    struct _BigN *(*add)(struct _BigN *, struct _BigN *);
    void (*resize)(struct _BigN *, int);
} BigN;

void initBigN(BigN *target, int size);
BigN *newBigN(int size);
void freeBigN(BigN* target);
void resizeBigN(BigN *target, int size);
BigN *parseBigN(char *s);
int sprintBigN(BigN *this, char *s);
void printBigN(BigN *this);
BigN *addBigN(BigN *a, BigN *b);
void susToBigN(BigN *a, BigN *b);
BigN *mulBigN(BigN *a, BigN *b);

#endif
