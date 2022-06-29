SRCMODULES = main.cpp curses.cpp game.cpp shape.cpp
OBJMODULES = $(SRCMODULES:.cpp=.o)
CXX = g++
CXXFLAGS = -Wall -O2
LDFLAGS = -lncurses
RM = rm

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

tetris: $(OBJMODULES)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCMODULES)
	$(CXX) -MM $^ > $@

clean:
	$(RM) -rf *.o tetris deps.mk
