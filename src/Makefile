CXXFLAGS+=-MMD -Wall -Wextra -std=c++11
SRC=$(wildcard *.cpp)
OBJS=$(SRC:%.cpp=%.o)
LIBS+=-L/usr/lib/i386-linux-gnu -lcurl

walker: $(OBJS)
	$(CXX) -o walker $(LIBS) $(OBJS)

clean:
	rm -f *.o *.d
	rm -f walker

debug: CXXFLAGS += -g
debug: walker

-include $(OBJS:%.o=%.d)

