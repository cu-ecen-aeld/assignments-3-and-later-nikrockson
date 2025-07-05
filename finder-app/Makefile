# Default compiler
CC = $(CROSS_COMPILE)gcc

# Target binary
TARGET = writer

# Source files
SRC = writer.c
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Build the target from object file
$(TARGET): $(OBJ)
	$(CC) -o $@ $^

# Clean target
clean:
	rm -f $(TARGET) *.o
