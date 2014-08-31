export CXX=c++
export CFLAGS= -I/usr/local/include -Wall -ansi -pedantic -Os
export LDFLAGS= -L/usr/local/lib -lGL -lSDL2 -lSDL2_image -lGLEW
EXEC=test-engine
SRC= main.cpp
OBJ= $(SRC:.cpp=.o) tools/tools.o

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)
#subdirectories build
tools/tools.o:
	gmake -C tools
#directory build
%.o:	%.cpp
	$(CXX) -o $@ -c $< $(CFLAGS)
.PHONY: clean mrpropre depend

clean:
	rm -rf *.o
	gmake clean -C tools
mrpropre: clean
	rm -f $(EXEC)
depend:
	gmake depend -C tools
