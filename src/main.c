#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LETTERS 26
#define EPS 1e-4
#define ABS_SLACK 1e-9
#define REL_SLACK 1e-11
#define PRINT_CAP 1048576ULL

static const char *ops[] = {"+", "\u2212", "\u00D7", "\u00F7"};

static int n_letters = MAX_LETTERS;
static int has_target = 0;
static int force = 0;
static double target = 0.0;
static long long limit = 1000;

static double bnd_lo[MAX_LETTERS + 1];
static double bnd_hi[MAX_LETTERS + 1];

static char expr[512];
static size_t expr_len = 0;

static long long found = 0;
static int stop = 0;

static void usage(FILE *out, const char *prog)
{
    fprintf(out,
        "usage: %s [-n N] [--reverse RESULT] [--limit K] [--force]\n"
        "  -n N           use letters a..(a+N-1), 1..26; default: a..z\n"
        "  --reverse R    list expressions that evaluate to R (left-to-right)\n"
        "  --limit K      max matches reported in reverse mode; default 1000\n"
        "  --force        allow printing more than one million expressions\n",
        prog);
}

static double apply_op(int op, double x, double v)
{
    switch (op) {
    case 0: return x + v;
    case 1: return x - v;
    case 2: return x * v;
    default: return x / v;
    }
}

static void build_bounds(void)
{
    double lo = target - EPS;
    double hi = target + EPS;
    for (int g = n_letters; g >= 2; g--) {
        double v = g;
        double l[4] = { lo - v, lo + v, lo / v, lo * v };
        double h[4] = { hi - v, hi + v, hi / v, hi * v };
        double m = l[0], M = h[0];
        for (int k = 1; k < 4; k++) {
            if (l[k] < m) m = l[k];
            if (h[k] > M) M = h[k];
        }
        bnd_lo[g] = m - ABS_SLACK - REL_SLACK * (1.0 + fabs(m));
        bnd_hi[g] = M + ABS_SLACK + REL_SLACK * (1.0 + fabs(M));
        lo = bnd_lo[g];
        hi = bnd_hi[g];
    }
}

static void dfs(int consumed, double acc)
{
    if (stop)
        return;
    if (consumed == n_letters) {
        if (!has_target || fabs(acc - target) < EPS) {
            expr[expr_len] = '\0';
            printf("%s = %.7g\n", expr, acc);
            found++;
            if (has_target && found >= limit)
                stop = 1;
        }
        return;
    }
    int vnext = consumed + 1;
    for (int op = 0; op < 4 && !stop; op++) {
        double next = apply_op(op, acc, vnext);
        if (has_target && (next < bnd_lo[vnext] || next > bnd_hi[vnext]))
            continue;
        const char *o = ops[op];
        size_t olen = strlen(o);
        expr[expr_len++] = ' ';
        memcpy(expr + expr_len, o, olen);
        expr_len += olen;
        expr[expr_len++] = ' ';
        expr[expr_len++] = (char)('a' + vnext - 1);
        dfs(consumed + 1, next);
        expr_len -= olen + 3;
    }
}

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            char *end = NULL;
            long v = strtol(argv[++i], &end, 10);
            if (end == argv[i] || *end != '\0' || v < 1 || v > MAX_LETTERS) {
                usage(stderr, argv[0]);
                return 1;
            }
            n_letters = (int)v;
        } else if (strcmp(argv[i], "--reverse") == 0 && i + 1 < argc) {
            char *end = NULL;
            target = strtod(argv[++i], &end);
            if (!isfinite(target) || end == argv[i] || *end != '\0') {
                usage(stderr, argv[0]);
                return 1;
            }
            has_target = 1;
        } else if (strcmp(argv[i], "--limit") == 0 && i + 1 < argc) {
            char *end = NULL;
            long long v = strtoll(argv[++i], &end, 10);
            if (end == argv[i] || *end != '\0' || v < 1) {
                usage(stderr, argv[0]);
                return 1;
            }
            limit = v;
        } else if (strcmp(argv[i], "--force") == 0) {
            force = 1;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            usage(stdout, argv[0]);
            return 0;
        } else {
            usage(stderr, argv[0]);
            return 1;
        }
    }

    unsigned long long combos = 1;
    for (int i = 2; i <= n_letters; i++)
        combos *= 4;

    if (!has_target && combos > PRINT_CAP && !force) {
        printf("a..%c: %d gaps x 4 operations = %llu possible expressions\n",
               'a' + n_letters - 1, n_letters - 1, combos);
        printf("too many to print; try:\n");
        printf("  omicron --reverse <number>   find expressions equal to it\n");
        printf("  omicron -n <N>               enumerate a smaller prefix\n");
        printf("  omicron -n <N> --force       print everything anyway\n");
        return 0;
    }
    if (!has_target && combos > PRINT_CAP)
        fprintf(stderr,
                "warning: printing %llu expressions; this may run for a very long time\n",
                combos);

    expr[expr_len++] = 'a';
    if (has_target)
        build_bounds();
    dfs(1, 1.0);

    if (has_target && found == 0)
        printf("no expression found for result %.7g\n", target);
    else if (has_target && stop)
        fprintf(stderr, "stopped after %lld matches (--limit %lld)\n", found, limit);

    return 0;
}
