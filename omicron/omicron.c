#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    float a = 1.0, b = 2.0, c = 3.0, d = 4.0;
    char *ops[] = {"+", "\u2212", "\u00D7", "\u00F7"};
    float vals[] = {a, b, c, d};
    char *names[] = {"a", "b", "c", "d"};

    int has_target = 0;
    float target = 0.0f;

    if (argc == 3 && strcmp(argv[1], "--reverse") == 0) {
        char *end = NULL;
        target = strtof(argv[2], &end);
        if (end == argv[2] || *end != '\0') {
            fprintf(stderr, "usage: %s [--reverse <result>]\n", argv[0]);
            return 1;
        }
        has_target = 1;
    } else if (argc != 1) {
        fprintf(stderr, "usage: %s [--reverse <result>]\n", argv[0]);
        return 1;
    }

    int found = 0;

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
                if (!has_target || fabsf(r - target) < 1e-4f) {
                    printf("a %s b %s c %s d = %.7g\n", ops[i], ops[j], ops[k], r);
                    found++;
                }
            }

    if (has_target && found == 0)
        printf("no expression found for result %.7g\n", target);

    return 0;
}
