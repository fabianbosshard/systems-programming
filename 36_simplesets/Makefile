CXXFLAGS=-Wall -g -std=c++11
CFLAGS=-Wall -g
# CC=$(CXX)

SHELL=/bin/bash

LIBRARY=simplesets

TIMEOUT=8

TESTS_DIR=tests

TESTS_C:=$(wildcard $(TESTS_DIR)/*.c)
TESTS_BIN:=$(patsubst $(TESTS_DIR)/%.c, $(TESTS_DIR)/%, $(TESTS_C))
TESTS:=$(patsubst $(TESTS_DIR)/%.c, %, $(TESTS_C))

.PHONY: all
all: check

$(TESTS_DIR)/%: $(TESTS_DIR)/%.o $(LIBRARY).o
	$(CXX) $(CFLAGS) $(LDFLAGS) $(TESTS_DIR)/$*.o $(LIBRARY).o -o $@

.PHONY: check
check: $(TESTS_BIN)
	@exec 2> /dev/null; \
	for t in $(TESTS); do \
		echo -n "Running test $$t..." ; \
		"$(TESTS_DIR)/$$t" &\
		prog_pid=$$!; \
		( sleep $(TIMEOUT); kill $$prog_pid > /dev/null 2>&1 ) & \
		killer_pid=$$!; \
		wait $$prog_pid; \
		res=$$?; \
		if test $$res -gt 128; \
		then \
			case `kill -l $$(($$res - 128))` in \
				ABRT ) echo "FAIL"; ;; \
				TERM ) echo "TIME OUT"; ;; \
				* ) echo "UNKNOWN"; ;; \
			esac ; \
			echo "you may run $(TESTS_DIR)/$$t to see what went wrong" ;\
		else \
			kill $$killer_pid > /dev/null 2>&1 ;\
			echo "PASS" ;\
		fi; \
	done


.PHONY: clean
clean:
	rm -f $(LIBRARY).o tests/*.o $(TESTS_BIN)


