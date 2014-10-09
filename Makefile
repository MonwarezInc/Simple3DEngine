export CXX=c++
export CFLAGS= -I/usr/local/include -Wall -std=c++11 -Os
export LDFLAGS= -L/usr/local/lib -lGL -lSDL2 -lSDL2_image -lGLEW -lassimp
EXEC=test-engine
OUT=graphic-engine.o 
OBJ= tools/tools.obj window/window.obj engine/engine.obj

all: $(EXEC) 

$(EXEC): main.o $(OUT)
	$(CXX) -o $@ $^ $(LDFLAGS)
$(OUT): $(OBJ)
	ld -o $@ -Ur $^
#subdirectories build
tools/tools.obj:
	gmake -C tools
window/window.obj:
	gmake -C window
engine/engine.obj:
	gmake -C engine
#directory build
%.o:	%.cpp
	$(CXX) -o $@ -c $< $(CFLAGS)
.PHONY: clean mrpropre depend

clean:
	rm -rf main.o
	gmake clean -C tools
	gmake clean -C window
	gmake clean -C engine
mrpropre: clean
	rm -f $(EXEC) $(OUT)
depend:
	gmake depend -C tools
	gmake depend -C window
	gmake depend -C engine
