.PHONY: clean

CFLAGS = `pkg-config --cflags unittest++ ` -std=c++11 -MMD
CXXFLAGS = $(CFLAGS)
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
LIBS = `pkg-config --libs unittest++` -lcurl
BINARY = run_tests

$(BINARY): $(OBJS)
	make -C ../src
	g++ $(CXXFLAGS) $(OBJS) -o $(BINARY) $(LIBS) \
	../src/ArticleCollection.o ../src/CurlUrlCreator.o \
	../src/JsonToArticleConverter.o ../lib/jsoncpp/jsoncpp.o \
	../src/Article.o ../src/ToJsonWriter.o

clean:
	rm -f *.o *.d $(BINARY)

-include $(OBJS:%.o=%.d)
