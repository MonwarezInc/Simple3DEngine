export CXX=c++
export CFLAGS= -I/usr/local/include -Wall -ansi -pedantic -Os
export LDFLAGS= -L/usr/local/lib -lGL -lSDL2 -lSDL2_image -lGLEW
EXEC=test-engine
OUT=graphic-engine.o 
OBJ= tools/tools.obj window/window.obj

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

#directory build
%.o:	%.cpp
	$(CXX) -o $@ -c $< $(CFLAGS)
.PHONY: clean mrpropre depend

clean:
	rm -rf main.o
	gmake clean -C tools
	gmake clean -C window
mrpropre: clean
	rm -f $(EXEC) $(OUT)
depend:
	gmake depend -C tools
	gmake depend -C window
