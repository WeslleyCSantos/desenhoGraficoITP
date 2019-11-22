all: main

main: main.o func.o imagem.o poligono.o extra.o circulo.o linha.o safestdlib.o
	gcc main.o func.o imagem.o poligono.o extra.o circulo.o linha.o safestdlib.o -o main

main.o: main.c
	gcc -c main.c

func.o: func.c
	gcc -c func.c

imagem.o: imagem.c
	gcc -c imagem.c

poligono.o: poligono.c
	gcc -c poligono.c

extra.o: extra.c
	gcc -c extra.c

circulo.o: circulo.c
	gcc -c circulo.c

linha.o: linha.c
	gcc -c linha.c

safestdlib.o: safestdlib.c
	gcc -c safestdlib.c

clean:
	rm -rf *.o main