.phony: clean

SRCS=manejar-senal.c captura-senal.c manejadores.c
OBJS=$(SRCS:.c=.o)
EXECS=manejar-senal captura-senal

all: ${EXECS}

manejar-senal: manejar-senal.o
	${CC} -o $@ $<

captura-senal: captura-senal.o manejadores.o
	${CC} -o $@ $^

captura-senal.o: captura-senal.c manejadores.h

manejadores.o: manejadores.c manejadores.h

clean:
	rm -f ${OBJS}
	rm -f ${EXECS}
	rm -f *~
