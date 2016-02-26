# Declaration of variables
CC = g++
CC_FLAGS = -g -std=c++11 -O2
LD_FLAGS = -g -pthread -lGL -lGLEW -lsfml-window -lsfml-system 

# File names
EXEC = NetroTD
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LD_FLAGS)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)
