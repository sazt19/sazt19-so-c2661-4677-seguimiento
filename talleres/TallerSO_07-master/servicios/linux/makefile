SRCS=systemd-service.c
OBJS=$(SRCS:.c=.o)
EXECS=$(OBJS:.o=)
INSTALLDIR=/usr/local/bin

all: ${EXECS}

systemd-service: systemd-service.o

systemd-service.o: systemd-service.c

install: systemd-service systemd-service.service
	install $< -t ${INSTALLDIR}
	cp systemd-service.service /etc/systemd/system/systemd-service.service

clean:
	rm -f ${OBJS}
	rm -f ${EXECS}
	rm -f *~

.phony: clean

servicio1: servicio1.c
	cc servicio1.c -o servicio1

cliente1: cliente1.c
	cc cliente1.c -o cliente1

servicio2: servicio2.c
	cc servicio2.c -o servicio2

install_servicio1: servicio1 servicio1.service
	install servicio1 -t ${INSTALLDIR}
	cp servicio1.service /etc/systemd/system/servicio1.service
	systemctl daemon-reload

install_servicio2: servicio2 servicio2.service
	install servicio2 -t ${INSTALLDIR}
	cp servicio2.service /etc/systemd/system/servicio2.service
	systemctl daemon-reload
