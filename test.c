#include "BigN.h"

int main() {
    BigN a, b;
    init(&a, 10);
    init(&b, 10);
    parse(&a, "99999999999999999");
    print(&a);
    parse(&b, "99999999999999999");
    print(&b);

    print(add(&a, &b));


    return 0;
}
