CC = g++
CFLAGS = -I. 
DEPS = pa3.h
OBJ = main.o given.o todo.o 

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $<

dp: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS)

