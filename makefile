pasta_o = objects

pasta_s = source

shell: main.o batch.o interativo.o
	gcc $(pasta_o)/main.o $(pasta_o)/batch.o $(pasta_o)/interativo.o -o shell -lm

main.o: pasta $(pasta_s)/main.c $(pasta_s)/batch.h $(pasta_s)/interativo.h
	gcc -c $(pasta_s)/main.c -o $(pasta_o)/main.o

batch.o: pasta $(pasta_s)/batch.c $(pasta_s)/batch.h
	gcc -c $(pasta_s)/batch.c -o $(pasta_o)/batch.o

interativo.o: pasta $(pasta_s)/interativo.c $(pasta_s)/interativo.h
	gcc -c $(pasta_s)/interativo.c -o $(pasta_o)/interativo.o

pasta:
	mkdir -p $(pasta_o)

test: shell
	./shell

test_batch: shell
	./shell teste.bat

clean:
	rm -rf *$(pasta_o)/.o $(pasta_o) shell