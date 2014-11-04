export CXX=c++
export PREFIX=/usr/local/
export COTHERFLAGS= # in case of multiple Include directory
export LOTHERFLAGS=	# in case of multiple Lib directory
export CFLAGS= -I$(PREFIX)include $(COTHERFLAGS) -Wall -std=c++11 -Os
export LDFLAGS= -L$(PREFIX)lib $(LOTHERFLAGS)-lGL -lSDL2 -lSDL2_image -lGLEW -lassimp
export MAKE=gmake

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
	$(MAKE) -C tools
window/window.obj:
	$(MAKE) -C window
engine/engine.obj:
	$(MAKE) -C engine
#directory build
%.o:	%.cpp
	$(CXX) -o $@ -c $< $(CFLAGS)
.PHONY: clean mrpropre depend

clean:
	rm -rf main.o
	$(MAKE) clean -C tools
	$(MAKE) clean -C window
	$(MAKE) clean -C engine
mrpropre: clean
	rm -f $(EXEC) $(OUT)
depend:
	$(MAKE) depend -C tools
	$(MAKE) depend -C window
	$(MAKE) depend -C engine
