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

typedef struct {
  parse_fn* fn;
  char str[4];
  size_t len;
  int want;
} test_t;

static void error(test_t* t, const char* fmt, ...) {
  char* s = t->str;
  printf("%02hhX %02hhX %02hhX %02hhX, len %zu: ", s[0], s[1], s[2], s[3], t->len);
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  printf("\n");
}

static void check(test_t* t) {
  int want = t->want;
  uint8_t got;
  int ok = t->fn(t->str, t->len, &got);
  if (want < 0 || want > 255) {
    if (ok) error(t, "expected error, got %d", got);
  }
  else {
    if (! ok) error(t, "expected %d, got error", want);
    else if (got != want) error(t, "expected %d, got %d", want, got);
  }
}

static void test_rec(test_t* t) {
  size_t i = t->len++;
  int want = t->want * 10;
  for (int c = CHAR_MIN; c <= CHAR_MAX; c++) {
    t->str[i] = (char)c;
    t->want = c < '0' || c > '9' ? -1 : want + c - '0';
    check(t);
    if (i < 2) test_rec(t);
  }
  t->len--;
}

static void test(parse_fn* fn, const char* name) {
  printf("testing %s\n", name);
  test_t t = {fn, {0}, 0, 0};
  test_rec(&t);
}

int main(void) {
  test(parse_uint8_fastswar, "parse_uint8_fastswar");
  test(parse_uint8_fastswar_bob, "parse_uint8_fastswar_bob");
}
