# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

# Directories
SRCDIR = src
INCDIR = header
BUILDDIR = build
TARGET = airline_reservation

# Source and Object Files
SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRC))

# Default target
all: $(TARGET)

# Linking the final executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Create build directory if not exists
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Clean build files
clean:
	rm -rf $(BUILDDIR) $(TARGET)
