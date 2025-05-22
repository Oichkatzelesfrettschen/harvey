CC_32 = gcc -m32 -std=c2x -Wall -Wextra -pedantic
CC_64 = gcc -m64 -std=c2x -Wall -Wextra -pedantic
SRC = acd_c23.c args.c

all: acd32 acd64

acd32: $(SRC)
	$(CC_32) $(SRC) -o $@

acd64: $(SRC)
	$(CC_64) $(SRC) -o $@

clean:
	rm -f acd32 acd64
