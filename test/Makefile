CFLAGS = `pkg-config --cflags unittest++ `
CXXFLAGS = $(CFLAGS)
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
LIBS = `pkg-config --libs unittest++`

tests: $(OBJS)
	g++ $(OBJS) -o run_tests $(LIBS)
