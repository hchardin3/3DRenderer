# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -g -I/usr/include/eigen3 -I.

# Source files
SRC = $(wildcard *.cpp)
# Header files (not used directly but ensures they are listed)
HEADERS = $(wildcard *.hpp)

# Object files
OBJ = $(SRC:.cpp=.o)
# Output executable
TARGET = main

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)

# Phony targets
.PHONY: all clean
