OBJ=tools.obj
all:	$(OBJ)

$(OBJ) :	DebugGL.o Shader.o Texture.o RcManager.o
	ld -o $@ -Ur $^
%.o: %.cpp
	$(CXX) -c $^ $(CFLAGS)
clean:
	rm -f $(OBJ) *.o
depend:
	$(CXX) -E -MM *.cpp $(CFLAGS) > .depend


