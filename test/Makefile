CFLAGS = `pkg-config --cflags unittest++ ` -std=c++11
CXXFLAGS = $(CFLAGS)
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
LIBS = `pkg-config --libs unittest++`
BINARY = run_tests

tests: $(OBJS)
	g++ $(CXXFLAGS) $(OBJS) -o $(BINARY) $(LIBS) ../src/ArticleCollection.o

clean:
	rm -f *.o *.d $(BINARY)
