COBJS	+= main.o send.o

#CFLAGS	+= -O2 -Wall -DDEBUG
CFLAGS	+= -I ./includeFile/

#LDFLAGS	+= -lpthread

CROSS_COMPILE	?=

CC = $(CROSS_COMPILE)gcc

TARGET = homeserver

all:$(TARGET)
$(TARGET):$(COBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(LDFLAGS)
	
.PHONY:clean
clean:
	rm -f $(COBJS)
