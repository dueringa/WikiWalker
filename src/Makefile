SRC=$(wildcard *.cpp)
SRC+=../lib/jsoncpp/jsoncpp.cpp
OBJS=$(SRC:%.cpp=%.o)
LIBS+=-L/usr/lib/i386-linux-gnu -lcurl
#GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always --tags)
GIT_VERSION=$(shell git describe --always)
CXXFLAGS+=-MMD -Wall -Wextra -std=c++11 -I../lib/jsoncpp/ -D _WW_VERSION=\"$(GIT_VERSION)\"

walker: $(OBJS)
	$(CXX) -o walker $(LIBS) $(OBJS)

clean:
	rm -f $(OBJS) *.d
	rm -f walker

debug: CXXFLAGS += -g
debug: walker

-include $(OBJS:%.o=%.d)

