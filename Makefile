# Compiler
CXX = g++

# Environment variable for Eigen include path
EIGEN_PATH = C:\\msys64\\mingw64\\include

# Compiler flags
CXXFLAGS = -g -I$(EIGEN_PATH) -I.

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
