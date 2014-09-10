
all: rsa.o funcs.o aes_func.o aes.o cbc.o k128.o main.o
	gcc -ansi -O3 $^ -o main -lm -lgmp

aes_func.o: 
	nasm -f elf64 aes_func.nasm

funcs.o: 
	nasm -f elf64 funcs.nasm

%.o: %.c
	gcc -ansi -O3 -c $^

clean: 
	rm *.o main

test:
	./main -f aes -c -i ex.txt -o ex.txt.aes.bin -p 1234567890ABCDEF
	./main -f aes -d -i ex.txt.aes.bin -o ex.txt.aes.dec -p 1234567890ABCDEF
	./main -f rsa -c -i ex.txt -o ex.txt.rsa.bin -p 135a31147278a27658a66182b41c8327@295af01608243d298a5bc98d6ac0d995
	./main -f rsa -d -i ex.txt.rsa.bin -o ex.txt.rsa.dec -p 219dd74223baf758fd5089709370f17@295af01608243d298a5bc98d6ac0d995
	
