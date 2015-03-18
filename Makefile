CC=gcc
CFLAGS=-g
LIBDIR=/usr/lib/i386-linux-gnu
OBJS=sg

all:	$(OBJS)

clean:
	rm -f *.o $(OBJS)

install: sg
	cp sg /usr/local/bin
	chown root:root sg /usr/local/bin/sg
	chmod 4711 sg /usr/local/bin/sg

sg:	sg.c
	$(CC) $(CFLAGS) sg.c -o sg -L$(LIBDIR)
	chown root:root sg
	chmod 4711 sg
