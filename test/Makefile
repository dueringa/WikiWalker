CFLAGS = `pkg-config --cflags unittest++ `
CXXFLAGS = $(CFLAGS)
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
LIBS = `pkg-config --libs unittest++`
BINARY = run_tests

tests: $(OBJS)
	g++ $(OBJS) -o $(BINARY) $(LIBS)

clean:
	rm -f *.o *.d $(BINARY)
