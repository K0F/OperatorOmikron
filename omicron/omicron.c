#include <stdio.h>

int main(void)
{
    float a = 1.0, b = 2.0, c = 3.0, d = 4.0;
    char *ops[] = {"+", "\u2212", "\u00D7", "\u00F7"};
    float vals[] = {a, b, c, d};
    char *names[] = {"a", "b", "c", "d"};

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++) {
                float r = a;
                switch (i) {
                case 0: r += b; break;
                case 1: r -= b; break;
                case 2: r *= b; break;
                case 3: r /= b; break;
                }
                switch (j) {
                case 0: r += c; break;
                case 1: r -= c; break;
                case 2: r *= c; break;
                case 3: r /= c; break;
                }
                switch (k) {
                case 0: r += d; break;
                case 1: r -= d; break;
                case 2: r *= d; break;
                case 3: r /= d; break;
                }
                printf("a %s b %s c %s d = %.7g\n", ops[i], ops[j], ops[k], r);
            }
    return 0;
}
