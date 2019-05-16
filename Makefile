CC=gcc
CFLAGS= -Wall -g
OBJECTS= helper.o 

main: runner.o
	$(CC) $(CFLAGS) -o 8-bit runner.o $(OBJECTS)
runner.o: helper.o
helper.o: 

.PHONY clean:
	rm *.o 8-bit