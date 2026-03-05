.phony: clean

SRCS=conectar-procesos.c
OBJS=$(SRCS:.c=.o)
EXECS=conectar-procesos

all: ${EXECS}

conectar-procesos: conectar-procesos.o
	${CC} -o $@ $<

clean:
	rm -f ${OBJS}
	rm -f ${EXECS}
	rm -f *~
