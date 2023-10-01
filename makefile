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


output: zeppelin.o quadmesh.o
	g++ Zeppelin.o QuadMesh.o -o output $(LDFLAGS) ${CFLAGS}

zeppelin.o: Zeppelin.cpp VECTOR3D.h QuadMesh.h
	g++ -c Zeppelin.cpp $(LDFLAGS) ${CFLAGS}

quadmesh.o: QuadMesh.cpp QuadMesh.h VECTOR3D.h
	g++ -c QuadMesh.cpp $(LDFLAGS) ${CFLAGS}

clean:
	rm *.o output
