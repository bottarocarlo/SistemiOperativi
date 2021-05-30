

BINARIES=$(patsubst %.c,%.o,$(wildcard *.c))

all: $(BINARIES) file.txt file
	@echo Done! 

file.txt:
	@echo "hello wonderful world" > $@

file:
	@touch $@

%.o: %.c
	gcc $^ -o $@

clean:
	rm -rf *.o *.txt file

.PHONY: clean