all:	
	gcc -O3 -o main_4r.o main_4r.c gen.c aes.c utils.c
	gcc -O3 -o main_5r.o main_5r.c gen.c aes.c utils.c
	gcc -O3 -o main_search main_search.c aes.c utils.c
	gcc -O3 -o main_gen.o main_gen.c aes.c utils.c gen.c