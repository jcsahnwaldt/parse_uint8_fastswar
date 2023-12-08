BIN := parse_uint8_fastswar

$(BIN) run_$(BIN) clean: c rs
	@:

c rs:
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: c rs
