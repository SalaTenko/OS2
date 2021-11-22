all: my_bash

rebuild: clean all

my_bash: build/main.o build/foo.o
	gcc build/main.o build/foo.o -o my_bash

build/main.o: src/main.c
	gcc -Wall -Werror -c src/main.c -o build/main.o

build/foo.o: src/foo.c
	gcc -Wall -Werror -c src/foo.c -o build/foo.o

clean:
	rm -rf build/*.o my_bash