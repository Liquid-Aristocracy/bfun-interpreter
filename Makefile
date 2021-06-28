CXXFLAGS =	-Wall -D_GLIBCXX_USE_CXX11_ABI=1

OBJS =		bfun.o Interpreter.o TM.o

TARGET =	bfun

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) *.o
