CPPFLAGS=$(shell sdl2-config --cflags) $(EXTRA_CPPFLAGS)
LDLIBS=$(shell sdl2-config --libs) -lGLEW $(EXTRA_LDLIBS)
EXTRA_LDLIBS?=-lGL
all: triangle
	./triangle
triangle: lib/shader_utils.o
clean:
	rm -f *.o triangle
.PHONY: all clean
