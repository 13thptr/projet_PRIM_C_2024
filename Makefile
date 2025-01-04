# Le code que j'ai écrit ne renvoie pas d'erreurs avec le warning -std=c99 a priori, mais filename.c, le fichier fourni, oui.
#CFLAGS += -std=c99 
CFLAGS += -Wpedantic -pedantic-errors
CFLAGS += -Werror
CFLAGS += -Wall
CFLAGS += -Wextra
#CFLAGS += -Waggregate-return Il est explicitement demandé de renvoyer une structure "picture" dans read_picture
CFLAGS += -Wbad-function-cast
CFLAGS += -Wcast-align
CFLAGS += -Wcast-qual
CFLAGS += -Wdisabled-optimization 
CFLAGS += -Wfloat-equal
CFLAGS += -Wformat=2
CFLAGS += -Wlogical-op
CFLAGS += -Wmissing-declarations
CFLAGS += -Wmissing-include-dirs
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wnested-externs
CFLAGS += -Wpointer-arith
CFLAGS += -Wredundant-decls
CFLAGS += -Wsequence-point
CFLAGS += -Wshadow
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wswitch
CFLAGS += -Wundef
CFLAGS += -Wunreachable-code
CFLAGS += -Wunused-but-set-parameter
CFLAGS += -Wwrite-strings
CFLAGS += -Wformat=2
CFLAGS += -g
#CFLAGS += -fsanitize=address

CC = gcc
# Define a pattern rule that compiles every .c file into a .o file

#prog: main.o
#	gcc main.o -o prog

#main: main.c
#	$(CC) -c $(CFLAGS) $^ -o $@ 

prog: main.o pictures.o safe_malloc.o pixels.o filename.o lut.o wrappers.o picture_rw.o
	$(CC) $(CFLAGS) -O2  $^ -o $@ -lm

picture_rw.o : picture_rw.c
	$(CC) -c $(CFLAGS) $^ -o $@

pictures.o : pictures.c
	$(CC) -c $(CFLAGS) $^ -o $@

pixels.o : pixels.c
	$(CC) -c $(CFLAGS) $^ -o $@

safe_malloc.o : safe_malloc.c
	$(CC) -c $(CFLAGS) $^ -o $@

filename.o : filename.c
	$(CC) -c $(CFLAGS) $^ -o $@

lut.o : lut.c
	$(CC) -c $(CFLAGS) $^ -o $@

wrappers.o : wrappers.c
	$(CC) -c $(CFLAGS) $^ -o $@

main.o : main.c
	$(CC) -c $(CFLAGS) $^ -o $@


clean:
	rm -rf *.o
	rm -rf Lenna_output/*