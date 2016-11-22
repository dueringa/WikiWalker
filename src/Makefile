.PHONY: clean

BIN_OUT_NAME=walker

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)

LIBSRC=../lib/jsoncpp/jsoncpp.cpp
LIBOBJ=$(LIBSRC:%.cpp=%.o)

LIBS+=-lcurl
GIT_VERSION:=$(shell git describe --always --dirty --tags)
CXXFLAGS+=-MMD -Wall -Wextra -Wshadow -std=c++11 -I../lib/jsoncpp/ -Werror=return-type

ifneq ($(GIT_VERSION), )
	CXXFLAGS+=-D _WW_VERSION=\"$(GIT_VERSION)\"
endif

$(BIN_OUT_NAME): $(OBJ) $(LIBOBJ)
	$(CXX) $(CXXFLAGS) -o $(BIN_OUT_NAME) $(LIBS) $(LIBOBJ) $(OBJ)

clean:
	rm -f $(OBJ) *.d
	rm -f $(BIN_OUT_NAME)

distclean: clean
	rm -f $(LIBOBJ)

debug: CXXFLAGS += -g
debug: $(BIN_OUT_NAME)

optimized: CXXFLAGS += -O3
optimized: $(BIN_OUT_NAME)

-include $(OBJ:%.o=%.d)
