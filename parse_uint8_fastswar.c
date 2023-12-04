#include <string.h>
#include <stdint.h>

#include <stdio.h>
#include <limits.h>
#include <stdarg.h>

typedef int parse_fn(const char*, size_t, uint8_t*);

static int parse_uint8_fastswar(const char *str, size_t len, uint8_t *num) {
  if (len == 0 || len > 3) { return 0; }
  uint32_t digits;
  memcpy(&digits, str, sizeof(digits));
  digits ^= 0x30303030;
  digits <<= ((4 - len) * 8);
  *num = (uint8_t)((0x640a01 * digits) >> 24);
#if 1
  uint32_t all_digits = ((digits | (0x06060606 + digits)) & 0xF0F0F0F0) == 0;
#else
  uint32_t all_digits = ((0x06060606 + digits) & 0xF0F0F0F0) == 0;
#endif
  return all_digits & ((__builtin_bswap32(digits) <= 0x020505));
}

static int parse_uint8_fastswar_bob(const char *str, size_t len, uint8_t *num) {
  uint32_t digits;
  memcpy(&digits, str, sizeof(digits));
  digits ^= 0x303030;
  digits <<= (len ^ 3) * 8;
  *num = (uint8_t)((0x640a01 * digits) >> 16);
#if 1
  uint32_t all_digits = (((digits + 0x767676) | digits) & 0x808080) == 0;
#else
  uint32_t all_digits = ((digits + 0x767676) & 0x808080) == 0;
#endif
  return all_digits && ((len ^ 3) < 3) && __builtin_bswap32(digits) <= 0x020505ff;
}

static void error(char *str, size_t len, const char* fmt, ...) {
  printf("%02hhX %02hhX %02hhX %02hhX, len %zu: ", str[0], str[1], str[2], str[3], len);
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  printf("\n");
}

static void check(char *str, size_t len, int want, int got) {
  if (want < 0 || want > 255) {
    if (got >= 0) error(str, len, "expected error, got %d", got);
  }
  else {
    if (got < 0) error(str, len, "expected %d, got error", want);
    if (got != want) error(str, len, "expected %d, got %d", want, got);
  }
}

static void test_rec(parse_fn fn, char *str, size_t len, int want, size_t i) {
  if (i < len) {
    want *= 10;
    for (char c = CHAR_MIN; ; c++) {
      str[i] = c;
      int w = c < '0' || c > '9' ? -1 : want + c - '0';
      test_rec(fn, str, len, w, i + 1);
      if (c == CHAR_MAX) break;
    }
  }
  else {
    uint8_t got;
    int ok = fn(str, len, &got);
    check(str, len, want, ok ? got : -1);
  }
}

static void test(parse_fn fn, const char* name) {
  printf("testing %s\n", name);
  char str[4] = {0};
  test_rec(fn, str, 1, 0, 0);
  test_rec(fn, str, 2, 0, 0);
  test_rec(fn, str, 3, 0, 0);
}

int main(void) {
  test(parse_uint8_fastswar, "parse_uint8_fastswar");
  test(parse_uint8_fastswar_bob, "parse_uint8_fastswar_bob");
}
