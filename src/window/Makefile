OBJ=window.obj
all:	$(OBJ)

$(OBJ) :	Graphics.o Window.o
	ld -o $@ -Ur $^
%.o: %.cpp
	$(CXX) -c $^ $(CFLAGS)
clean:
	rm -f $(OBJ) *.o
depend:
	$(CXX) -E -MM *.cpp $(CFLAGS) > .depend


