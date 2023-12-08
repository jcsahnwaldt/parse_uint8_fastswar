BIN := parse_uint8_fastswar

%: %.rs
	rustc $<

$(BIN): $(BIN).rs

run_$(BIN): $(BIN)
	./$<

clean:
	$(RM) $(BIN)
