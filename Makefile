CC = gcc

CFLAGS = -Wall -I./include
CFLAGS += $(FLAG)

LIBS= -lrt -lbcm2835

OBJECTS = rw.o spi_mcp_rasp.o iomapped_gpio.o cbuffer.o

rw: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LIBS)

%.o: ./src/%.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o rw
