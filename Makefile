.POSIX:

include config.mk

SRC = swdb.c client.c
OBJ = $(SRC:.c=.o)

all: swdb client
.c.o:
	${CC} ${CFLAGS} -c $<

swdb.o: swdb.c
client.o: client.c
${OBJ}: config.mk

swdb: clean ${OBJ}
	${CC} swdb.o -o swdb ${CFLAGS}

client: clean ${OBJ}
	${CC} client.o -o client ${CFLAGS}

clean:
	rm -f swdb client

.PHONY: all swdb client clean
