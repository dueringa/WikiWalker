.PHONY: clean

SRC=$(wildcard *.cpp)
OBJS=$(SRC:%.cpp=%.o)

LIBSRC=../lib/jsoncpp/jsoncpp.cpp
LIBOBJ=$(LIBSRC:%.cpp=%.o)

LIBS+=-L/usr/lib/i386-linux-gnu -lcurl
GIT_VERSION:=$(shell git describe --always --dirty --tags)
CXXFLAGS+=-MMD -Wall -Wextra -Wshadow -std=c++11 -I../lib/jsoncpp/

ifneq ($(GIT_VERSION), )
	CXXFLAGS+=-D _WW_VERSION=\"$(GIT_VERSION)\"
endif

walker: $(OBJS) $(LIBOBJ)
	$(CXX) $(CXXFLAGS) -o walker $(LIBS) $(LIBOBJ) $(OBJS)

clean:
	rm -f $(OBJS) *.d
	rm -f walker

distclean: clean
	rm -f $(LIBOBJ)

lib: $(LIBOBJ)

debug: CXXFLAGS += -g
debug: walker

optimized: CXXFLAGS += -O3
optimized: walker

-include $(OBJS:%.o=%.d)

