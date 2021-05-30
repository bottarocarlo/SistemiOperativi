

BINARIES=$(patsubst %.c,%,$(wildcard *.c))

all: $(BINARIES) file.txt file
	@echo Done! 

file.txt:
	

file:
	@touch $@

%.o: %.c
	gcc $^ -o $@

clean:
	rm -rf  *.txt file

.PHONY: clean