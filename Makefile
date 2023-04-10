all: lab1
lab1:geom.c
		gcc -Wall -Werror -o geom geom.c
run:
		./geom
cl: 
		rm geom