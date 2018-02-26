CC=gcc
CFLAGS= -Iinclude -Wall  -g -DDEBUG
LDFLAGS=-L./lib -Wl,-rpath=./lib -Wl,-rpath=/usr/local/lib

ttorrent: main.o parse_metafile.o bitfield.o
	$(CC) -o $@ $(LDFLAGS) $^ -ldl

clean:
	rm -rf *.o ttorrent
