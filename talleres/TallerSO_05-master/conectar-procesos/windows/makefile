.phony: clean

SRCS=conectar-procesos.c concatenar.c mostrar-archivo.c
OBJS=$(SRCS:.c=.o)
EXECS=$(OBJS:.o=)

all: ${EXECS}

conectar-procesos: conectar-procesos.o

conectar-procesos.o: conectar-procesos.c

concatenar: concatenar.o

concatenar.o: concatenar.c

mostrar-archivo: mostrar-archivo.o

mostrar-archivo.o: mostrar-archivo.c

clean:
	rm -f ${OBJS}
	rm -f ${EXECS}
	rm -f *~
