#include "BigN.h"

int main() {
    BigN *a, *b;

    a = parseBigN("0");
    a->print(a);
    b = parseBigN("0");
    b->print(b);

    printBigN(mulBigN(a, b));

    return 0;
}
