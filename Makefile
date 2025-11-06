all:
	gcc -o main vendor/stb/stb_image.c src/app.c src/modules/utils/utils.c src/modules/gl_draw/gl_draw.c src/modules/logger/log.c src/modules/texture/texture.c -Iinclude -lglut -lGL -lGLU -lm  

clean: 
	rm -f main
