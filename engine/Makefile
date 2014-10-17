OBJ=engine.obj
all:	$(OBJ)

$(OBJ) :	Engine-main.o Mesh.o MiscObject.o
	ld -o $@ -Ur $^
%.o: %.cpp
	$(CXX) -c $^ $(CFLAGS)
clean:
	rm -f $(OBJ) *.o
depend:
	$(CXX) -E -MM *.cpp $(CFLAGS) > .depend


