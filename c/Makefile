BIN := parse_uint8_fastswar

# CC := gcc-13
# CC := clang-17
CFLAGS := -std=c17 -O3 -Wall -Wextra -Wpedantic -Wconversion

$(BIN): $(BIN).c

run_$(BIN): $(BIN)
	./$<

clean:
	$(RM) $(BIN)
