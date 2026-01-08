appName := Vizualizator3D
all:
	@clear
	@rm -f assets/object.me
	gcc -o build/${appName} src/main.c -Iinclude -Llib -lSDL3 -lSDL3_image -lm -lObjReader -Wall -Werror -g

run:
	./build/${appName}
debug:
	gdb build/${appName}
