
all: k128.o main.o
	gcc -ansi -O3 $^ -o main

%.o: %.c
	gcc -ansi -O3 -c $^

clean: 
	rm *.o main

test:
	./main -c -i ex.txt -o ex.txt.bin -p 123 && ./main -d -i ex.txt.bin -o ex.txt.dec -p 123
	
