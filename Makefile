
all: k128.o main.o
	gcc $^ -o main

%.o: %.c
	gcc -c $^

clean: 
	rm *.o main
