# Compiler and flags
CXX := g++
CXXFLAGS := -g -Wall -Wextra -std=c++20

# Include directories
INCLUDE_DIRS := ./File_Creator ./Thread_Pool ./Sort

# Target and source files
TARGET := main
SRC := $(wildcard source/*.cpp)  # All .cpp files in the source directory
OBJ := $(SRC:source/%.cpp=obj/%.o)  # Corresponding object files

# Include paths
INCLUDES := $(addprefix -I, $(INCLUDE_DIRS))

# Targets
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

obj/%.o: source/%.cpp
	@mkdir -p obj  # Create obj directory if it doesn't exist
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf obj $(TARGET)

debug: $(TARGET)
	gdb ./$(TARGET)


