#include <string.h>
#include <stdint.h>

int parse_uint8_fastswar(const char *str, size_t len, uint8_t *num) {
  if (len == 0 || len > 3) { return 0; }
  union { uint8_t as_str[4]; uint32_t as_int; } digits;
  memcpy(&digits.as_int, str, sizeof(digits));
  digits.as_int ^= 0x30303030lu;
  digits.as_int <<= ((4 - len) * 8);
  uint32_t all_digits = ((digits.as_int | (0x06060606 + digits.as_int)) & 0xF0F0F0F0) == 0;
  *num = (uint8_t)((0x640a01 * digits.as_int) >> 24);
  return all_digits & ((__builtin_bswap32(digits.as_int) <= 0x020505));
}

int main(void) {
}
