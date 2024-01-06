CC = gcc-13

CFLAGS = -g -Wall
FILES = ./src/*.c ./lib/cryptocode/crypto_memset.c ./lib/djbsort/*.c ./lib/randombytes/rng.c
VERSION = FAST13  # Standard build version, change by using make VERSION=<SHORT/FAST><1/3/5><1/3>

all: main

main: main.c 
	$(CC) $(CFLAGS) -D$(VERSION) -DKAT -o main main.c $(FILES) -lssl -lcrypto -L/opt/homebrew/opt/openssl/lib -I/opt/homebrew/opt/openssl/include -lXKCP -L/usr/local
	$(CC) $(CFLAGS) -D$(VERSION) -O3 -o bench bench.c $(FILES) -lssl -lcrypto -L/opt/homebrew/opt/openssl/lib -I/opt/homebrew/opt/openssl/include -lXKCP -L/usr/local

clean:
	$(RM) main
	$(RM) bench
	$(RM) -r bench.dSYM
	$(RM) -r main.dSYM