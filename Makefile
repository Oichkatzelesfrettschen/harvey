CFLAGS ?=
CFLAGS += -std=c23 -Wall -Wextra -Wpedantic -Werror

all:
	$(MAKE) -C modern CFLAGS="$(CFLAGS)" all

clean:
	$(MAKE) -C modern clean
