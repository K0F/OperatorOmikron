#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE (1 << 16)
#define FNV_OFFSET 14695981039346656037ULL
#define FNV_PRIME 1099511628211ULL

static unsigned char buf[BUF_SIZE];

static uint64_t fnv1a(FILE *f, int *err)
{
    uint64_t h = FNV_OFFSET;
    size_t n;

    while ((n = fread(buf, 1, sizeof buf, f)) > 0) {
        for (size_t i = 0; i < n; i++) {
            h ^= buf[i];
            h *= FNV_PRIME;
        }
    }
    if (err)
        *err = ferror(f);
    return h;
}

static int sum_file(const char *path)
{
    const char *label = path;
    FILE *f;
    int err = 0;

    if (strcmp(path, "-") == 0) {
        f = stdin;
        label = "-";
    } else {
        f = fopen(path, "rb");
        if (!f) {
            fprintf(stderr, "kofsum: %s: cannot open\n", path);
            return -1;
        }
    }

    uint64_t h = fnv1a(f, &err);
    if (f != stdin)
        fclose(f);
    if (err) {
        fprintf(stderr, "kofsum: %s: read error\n", path);
        return -1;
    }
    printf("%016llx  %s\n", (unsigned long long)h, label);
    return 0;
}

int main(int argc, char **argv)
{
    int status = 0;

    if (argc == 1)
        return sum_file("-") == 0 ? 0 : 1;

    for (int i = 1; i < argc; i++)
        if (sum_file(argv[i]) != 0)
            status = 1;
    return status;
}
