#ifndef TB_COMMON_H
#define TB_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int tb_tests_run = 0;
static int tb_tests_failed = 0;

#define TB_EXPECT(cond, msg)                                                     \
    do {                                                                         \
        tb_tests_run++;                                                          \
        if (!(cond)) {                                                           \
            tb_tests_failed++;                                                   \
            fprintf(stderr, "[FAIL] %s:%d: %s\n", __FILE__, __LINE__, (msg));    \
        }                                                                        \
    } while (0)

#define TB_EXPECT_INT(expected, actual, msg)                                     \
    do {                                                                         \
        int tb_expected_value = (expected);                                      \
        int tb_actual_value = (actual);                                          \
        tb_tests_run++;                                                          \
        if (tb_expected_value != tb_actual_value) {                              \
            tb_tests_failed++;                                                   \
            fprintf(stderr, "[FAIL] %s:%d: %s (expected %d, got %d)\n",          \
                    __FILE__, __LINE__, (msg), tb_expected_value,                \
                    tb_actual_value);                                            \
        }                                                                        \
    } while (0)

#define TB_EXPECT_LONG(expected, actual, msg)                                    \
    do {                                                                         \
        long tb_expected_value = (expected);                                     \
        long tb_actual_value = (actual);                                         \
        tb_tests_run++;                                                          \
        if (tb_expected_value != tb_actual_value) {                              \
            tb_tests_failed++;                                                   \
            fprintf(stderr, "[FAIL] %s:%d: %s (expected %ld, got %ld)\n",        \
                    __FILE__, __LINE__, (msg), tb_expected_value,                \
                    tb_actual_value);                                            \
        }                                                                        \
    } while (0)

static int tb_write_file(const char *path, const char *text) {
    FILE *fp = NULL;

    if (!path || !text) {
        return 0;
    }

    fp = fopen(path, "w");
    if (!fp) {
        return 0;
    }

    if (fputs(text, fp) == EOF) {
        fclose(fp);
        return 0;
    }

    fclose(fp);
    return 1;
}

static char *tb_read_file(const char *path) {
    FILE *fp = NULL;
    char *buffer = NULL;
    long size;

    if (!path) {
        return NULL;
    }

    fp = fopen(path, "rb");
    if (!fp) {
        return NULL;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return NULL;
    }

    size = ftell(fp);
    if (size < 0 || fseek(fp, 0, SEEK_SET) != 0) {
        fclose(fp);
        return NULL;
    }

    buffer = (char *)malloc((size_t)size + 1);
    if (!buffer) {
        fclose(fp);
        return NULL;
    }

    if (size > 0 && fread(buffer, 1, (size_t)size, fp) != (size_t)size) {
        free(buffer);
        fclose(fp);
        return NULL;
    }

    buffer[size] = '\0';
    fclose(fp);
    return buffer;
}

static int tb_contains_in_order(const char *text, const char **needles, int count)
#if defined(__GNUC__)
    __attribute__((unused))
#endif
;

static int tb_contains_in_order(const char *text, const char **needles, int count) {
    const char *cursor = NULL;
    int i;

    if (!text || !needles || count < 0) {
        return 0;
    }

    cursor = text;
    for (i = 0; i < count; i++) {
        cursor = strstr(cursor, needles[i]);
        if (!cursor) {
            return 0;
        }
        cursor += strlen(needles[i]);
    }

    return 1;
}

static int tb_run_command(const char *command) {
    if (!command) {
        return -1;
    }

    return system(command);
}

static int tb_finish(const char *name) {
    if (tb_tests_failed == 0) {
        printf("[PASS] %s: %d tests\n", name, tb_tests_run);
        return 0;
    }

    printf("[FAIL] %s: %d/%d tests failed\n", name, tb_tests_failed, tb_tests_run);
    return 1;
}

#endif
