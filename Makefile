nbc: nbc.c
	gcc -g -pg -Wall -Wstrict-prototypes -Wshadow -o nbc nbc.c -lm

run: nbc.exe
	nbc

clean:
	rm -fr *.exe *.o *.stackdump nbc

