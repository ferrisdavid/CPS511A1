# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
TARGET = Robot3D.exe
CFLAGS += -D_WIN32
# OS X/Linux
else
UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
	TARGET = Robot3D
	LDFLAGS = -lGL -lGLU -lglut
	CFLAGS = -Wall -std=c99
	endif

	ifeq ($(UNAME_S),Darwin)
	LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
	CFLAGS += -D__APPLE__
	TARGET = Robot3D
	endif
endif


output: robot.o quadmesh.o
	g++ Robot3D.o QuadMesh.o -o output $(LDFLAGS) ${CFLAGS}

robot.o: Robot3D.cpp VECTOR3D.h QuadMesh.h
	g++ -c Robot3D.cpp $(LDFLAGS) ${CFLAGS}

quadmesh.o: QuadMesh.cpp QuadMesh.h VECTOR3D.h
	g++ -c QuadMesh.cpp $(LDFLAGS) ${CFLAGS}

clean:
	rm *.o output
