CC = g++
CFLAGS = -pg -g -O2 -Wall
CPPFLAGS = -std=c++11
LDFLAGS = -pg -g
LIBS = -lm -lpng

raytracer:  main.o raytracer.o util.o light_source.o scene_object.o bmp_io.o texture_handler.o
	$(CC) $(CFLAGS) -o raytracer \
	main.o raytracer.o util.o light_source.o scene_object.o bmp_io.o texture_handler.o $(LIBS)

clean:
	-rm -f core *.o
	-rm raytracer