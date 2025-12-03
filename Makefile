all:
	@clear
	gcc -o build/Editor3D src/main.c -Iinclude -Llib -lSDL3 -lm -Wall -Werror

run:
	./build/Editor3D
