all:
	gcc -o main vendor/stb/stb_image.c src/app.c src/modules/utils/utils.c src/modules/gl_draw/gl_draw.c src/modules/logger/log.c src/modules/texture/texture.c src/modules/utils/dynamic_array.c src/modules/obj_parser/obj_parser.c -Iinclude -lglut -lGL -lGLU -lm -fsanitize=address -g -O0

clean: 
	rm -f main
