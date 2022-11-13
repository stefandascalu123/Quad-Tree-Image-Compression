# Declaratiile de variabile
CC = gcc
 
# Regula de compilare
all: build

build: quadtree.c
	$(CC) -o quadtree quadtree.c  -std=gnu11 -g -Wall -lm

# Regulile de "curatenie" (se folosesc pentru stergerea fisierelor intermediare si/sau rezultate)
.PHONY : clean
clean :
	rm -f quadtree
	rm -f *.out
	
 