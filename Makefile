FILE := parse_uint8_fastswar

CC := gcc-13
# CC := clang-17
CFLAGS := -std=c17 -O3 -Wall -Wextra -Wpedantic -Wconversion

run_$(FILE): $(FILE)
	./$(FILE)

$(FILE): $(FILE).c


clean:
	$(RM) $(FILE)
