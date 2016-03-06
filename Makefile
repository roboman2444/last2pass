CC = gcc
LDFLAGS =
CFLAGS = -Wall -O0 -march=native -g
OBJECTS = last2pass.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

last2pass: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	@echo cleaning oop
	@rm -f $(OBJECTS)
purge:
	@echo purging oop
	@rm -f $(OBJECTS)
	@rm -f last2pass
