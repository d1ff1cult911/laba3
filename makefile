CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I.
LDFLAGS =

SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = main.exe

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
