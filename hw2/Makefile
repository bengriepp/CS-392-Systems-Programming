CC = gcc
CFLAGS = -I.
DEPS = utils.h insertion.h
OBJ = main.o utils.o insertion.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sortmake: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
