CC := gcc-13
# CC := clang-17
CFLAGS := -std=c17 -O3 -Wall -Wextra -Wpedantic -Wconversion

parse_uint8_fastswar: parse_uint8_fastswar.c

clean:
	$(RM) parse_uint8_fastswar
