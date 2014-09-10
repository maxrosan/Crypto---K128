
all: rsa.o aes_func.o aes.o cbc.o k128.o main.o
	gcc -ansi -O3 $^ -o main -lm -lgmp

aes_func.o: 
	nasm -f elf64 aes_func.nasm

%.o: %.c
	gcc -ansi -O3 -c $^

clean: 
	rm *.o main

test:
	./main -f aes -c -i ex.txt -o ex.txt.bin -p 1234567890ABCDEF && ./main -f aes -d -i ex.txt.bin -o ex.txt.dec -p 1234567890ABCDEF
	
