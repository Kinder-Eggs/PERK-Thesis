CC = gcc

CFLAGS = -g -Wall
FILES = ./src/*.c ./lib/cryptocode/crypto_memset.c ./lib/djbsort/*.c ./lib/randombytes/rng.c ./lib/XKCP/*.c
VERSION = SHORT55

all: main

main: main.c 
	$(CC) $(CFLAGS) -D$(VERSION) -DKAT -o main main.c $(FILES) -lssl -lcrypto -L/opt/homebrew/opt/openssl/lib -I/opt/homebrew/opt/openssl/include
	$(CC) $(CFLAGS) -D$(VERSION) -O3 -o bench bench.c $(FILES) -lssl -lcrypto -L/opt/homebrew/opt/openssl/lib -I/opt/homebrew/opt/openssl/include

clean:
	$(RM) main