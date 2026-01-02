all:
	@clear
	@rm -f assets/object.me
	gcc -o build/Editor3D src/main.c -Iinclude -Llib -lSDL3 -lSDL3_image -lm -lObjReader -Wall -Werror

run:
	./build/Editor3D
obj:
	@rm -f build/obj
	@clear
	gcc -o build/obj src/ObjLoader/main.c -Wall -Werror && ./build/obj
