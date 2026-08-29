#include <stdio.h>
#include "escape.h"

int main() {
    printf("cr=0, ci=0  (deveria ficar preso -> esperado 100): %d\n", escape(0, 0, 100));
    printf("cr=2, ci=2  (deveria escapar rapido -> esperado 1): %d\n", escape(2, 2, 100));
    printf("cr=-1, ci=0 (deveria ficar preso -> esperado 100): %d\n", escape(-1, 0, 100));
    return 0;
}
