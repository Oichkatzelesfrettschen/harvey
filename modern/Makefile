CC = clang
CFLAGS += -std=c23 -Wall -Wextra -Wpedantic -Werror -pthread
SRC = acd_c23.c args.c spinlock.c

all: acd

acd: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $@

clean:
	rm -f acd
