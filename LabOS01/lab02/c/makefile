all: %.out

%.out: %.o
	 gcc -o %.out %.o

%.o: %.c
	 gcc -c %.c
     
clean:
	 rm %.o %.out