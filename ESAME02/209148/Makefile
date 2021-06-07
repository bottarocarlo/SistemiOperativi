BINARIES=$(patsubst %.c,%.o,$(wildcard *.c))

all: $(BINARIES)
	@echo Done! 

%.o: %.c
	gcc $^ -o app

clean:
	rm -rf *.o *.txt file

.PHONY: clean