# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
TARGET = Zeppelin.exe
CFLAGS += -D_WIN32
# OS X/Linux
else
UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
	TARGET = Zeppelin
	LDFLAGS = -lGL -lGLU -lglut
	CFLAGS = -Wall -std=c99
	endif

	ifeq ($(UNAME_S),Darwin)
	LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
	CFLAGS += -D__APPLE__
	TARGET = Zeppelin
	endif
endif


output: zeppelin.o quadmesh.o cube.o
	g++ Zeppelin.o QuadMesh.o cube.o -o output $(LDFLAGS) ${CFLAGS} && ./output

zeppelin.o: Zeppelin.cpp VECTOR3D.h QuadMesh.h surfaceModeller.h stb_image.h
	g++ -c Zeppelin.cpp $(LDFLAGS) ${CFLAGS}

quadmesh.o: QuadMesh.cpp QuadMesh.h VECTOR3D.h stb_image.h
	g++ -c QuadMesh.cpp $(LDFLAGS) ${CFLAGS}

cube.o: Cube.cpp Cube.h stb_image.h
	g++ -c Cube.cpp $(LDFLAGS) ${CFLAGS}

clean:
	rm *.o output
