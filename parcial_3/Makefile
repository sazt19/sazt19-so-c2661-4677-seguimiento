# Makefile para MSYS2 / MinGW-w64
# Usar desde la terminal de MSYS2: make

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g
LDFLAGS = -lpthread

TARGET  = v3
SRC     = v3.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET) $(TARGET).exe
