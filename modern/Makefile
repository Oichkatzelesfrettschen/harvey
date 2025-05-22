CC_32 = gcc -m32 -std=c23 -Wall -Wextra -pedantic
CC_64 = gcc -m64 -std=c23 -Wall -Wextra -pedantic

all: acd32 acd64

acd32: acd_c23.c
$(CC_32) $< -o $@

acd64: acd_c23.c
$(CC_64) $< -o $@

clean:
rm -f acd32 acd64
