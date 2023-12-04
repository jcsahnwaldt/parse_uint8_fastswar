#include <string.h>
#include <stdint.h>

#include <stdio.h>
#include <limits.h>

static int parse_uint8_fastswar(const char *str, size_t len, uint8_t *num) {
  if (len == 0 || len > 3) { return 0; }
  uint32_t digits;
  memcpy(&digits, str, sizeof(digits));
  digits ^= 0x30303030lu;
  digits <<= ((4 - len) * 8);
#if 1
  uint32_t all_digits = ((digits | (0x06060606 + digits)) & 0xF0F0F0F0) == 0;
#else
  uint32_t all_digits = ((0x06060606 + digits) & 0xF0F0F0F0) == 0;
#endif
  *num = (uint8_t)((0x640a01 * digits) >> 24);
  return all_digits & ((__builtin_bswap32(digits) <= 0x020505));
}

static void error(char *str, size_t len, const char* fmt, int val) {
    printf("%hhX %hhX %hhX %hhX, len %zu: ", str[0], str[1], str[2], str[3], len);
    printf(fmt, val);
    printf("\n");
}

static void check(char *str, size_t len, int want, int got) {
    if (want < 0 || want > 255) {
        if (got >= 0) error(str, len, "expected error, got %d", got);
    }
    else {
        if (got < 0) error(str, len, "expected %d, got error", want);
    }
}

static void test(char *str, size_t len, int want, size_t i) {
    if (i < len) {
        want *= 10;
        for (char c = CHAR_MIN; ; c++) {
            str[i] = c;
            int w = c < '0' || c > '9' ? -1 : want + c - '0';
            test(str, len, w, i + 1);
            if (c == CHAR_MAX) break;
        }
    }
    else {
        uint8_t got;
        int ok = parse_uint8_fastswar(str, len, &got);
        check(str, len, want, ok ? got : -1);
    }
}

int main(void) {
    char str[4] = {0};
    test(str, 1, 0, 0);
    test(str, 2, 0, 0);
    test(str, 3, 0, 0);
}
