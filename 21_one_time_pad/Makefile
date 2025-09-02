# Build student and (optionally) reference solution, then run tests that
# compare behavior side-by-side.

CC      ?= cc
CFLAGS  ?= -std=c11 -Wall -Wextra -O2
LDFLAGS ?=

STU_SRC := otp.c
STU_BIN := otp

# Put the professor's file at this name:
REF_SRC ?= otp_sol.c
REF_BIN := otp_ref

.PHONY: all clean test tests ref present

all: $(STU_BIN) present

present:
	@mkdir -p tests

$(STU_BIN): $(STU_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Build reference if present; don't fail the whole build if it's missing.
ref:
	@if [ -f "$(REF_SRC)" ]; then \
	  echo "Building reference from $(REF_SRC) -> $(REF_BIN)"; \
	  $(CC) $(CFLAGS) -o $(REF_BIN) $(REF_SRC) $(LDFLAGS); \
	else \
	  echo "No reference source '$(REF_SRC)' found; tests will run without reference comparison."; \
	fi

test: $(STU_BIN) ref
	chmod +x tests/run
	./tests/run "$(abspath $(STU_BIN))" "$(abspath $(REF_BIN))"

tests: test

clean:
	rm -f $(STU_BIN) $(REF_BIN)
	rm -rf tests/tmp