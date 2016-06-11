#Design by CY

COBJS	+= main.o select.o xmlget.o

#CFLAGS	+= -O2 -Wall -DDEBUG
CFLAGS	+= -I./

LDFLAGS	+= -lmxml -lpthread

CROSS_COMPILE	?=

CC = $(CROSS_COMPILE)gcc

TARGET = update_server

all:$(TARGET)
$(TARGET):$(COBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(LDFLAGS)
	
.PHONY:clean
clean:
	rm -f $(COBJS)
