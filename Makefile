CC      ?= cc
CFLAGS  ?= -std=c11 -O2 -Wall -Wextra
LDLIBS  ?= -lm
BUILD   := build
BIN     := $(BUILD)/omicron
SRC     := src/main.c

.PHONY: all build test clean
.DELETE_ON_ERROR:

all: build

build: $(BIN)

$(BIN): $(SRC)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

test: build
	@set -e; \
	tmp=$$(mktemp); rev=$$(mktemp); \
	trap 'rm -f "$$tmp" "$$rev"' EXIT INT TERM; \
	$(BIN) -n 4 > "$$tmp"; \
	n=$$($(BIN) -n 4 | wc -l); \
	[ "$$n" -eq 64 ] || { echo "FAIL: expected 64 expressions, got $$n"; exit 1; }; \
	for want in \
	  'a + b + c + d = 10' \
	  'a + b + c − d = 2' \
	  'a ÷ b + c × d = 14' \
	  'a + b × c ÷ d = 2.25' \
	  'a + b − c × d = 0' \
	  'a × b × c × d = 24' \
	  'a ÷ b ÷ c ÷ d = 0.04166667'; \
	do \
	  grep -qF "$$want" "$$tmp" || { echo "FAIL: missing '$$want'"; exit 1; }; \
	done; \
	$(BIN) -n 4 --reverse 24 > "$$rev"; \
	for want in 'a × b × c × d = 24' 'a + b + c × d = 24'; do \
	  grep -qF "$$want" "$$rev" || { echo "FAIL: --reverse 24 missing '$$want'"; exit 1; }; \
	done; \
	$(BIN) -n 4 --reverse 100 | grep -qF 'no expression found' || { echo 'FAIL: --reverse 100 should find nothing'; exit 1; }; \
	if $(BIN) -n 4 --reverse abc >/dev/null 2>&1; then echo 'FAIL: invalid target accepted'; exit 1; fi; \
	if $(BIN) bogus >/dev/null 2>&1; then echo 'FAIL: bad args accepted'; exit 1; fi; \
	echo 'All tests passed.'

clean:
	rm -rf $(BUILD)
