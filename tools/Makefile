OBJ=tools.o
all:	$(OBJ)

$(OBJ) :	DebugGL.o Shader.o
	ld -o $@ -Ur $^
%.o: %.cpp
	$(CXX) -c $^ $(CFLAGS)
clean:
	rm -f $(OBJ) *.o
depend:
	$(CXX) -E -MM *.cpp $(CFLAGS) > .depend


