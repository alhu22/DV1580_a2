# Compiler and Linking Variables
CC = gcc
CFLAGS = -Wall -fPIC
LIB_NAME = libmemory_manager.so

# Source and Object Files
SRC = memory_manager.c linked_list.c
OBJ = $(SRC:.c=.o)

# Default target
all: mmanager list test_mmanager test_list test_listCG

# Rule to create the dynamic library
$(LIB_NAME): $(OBJ)
	$(CC) -shared -o $@ $(OBJ) -lm   # Add -lm to link the math library

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build the memory manager
mmanager: $(LIB_NAME)

# Build the linked list
list: linked_list.o

# Test target to run the memory manager test program
test_mmanager: $(LIB_NAME)
	$(CC) -o test_memory_manager test_memory_manager.c -L. -lmemory_manager -lm 
	
# Test target to run the linked list test program
test_list: $(LIB_NAME) linked_list.o
	$(CC) -o test_linked_list linked_list.o test_linked_list.c -L. -lmemory_manager -lm
	
# Additional target for test_linked_listCG
test_listCG: $(LIB_NAME) linked_list.o
	$(CC) -o test_linked_listCG linked_list.c test_linked_list.c -L. -lmemory_manager -Wl,-rpath=. -lpthread -lm
	

# Run tests
run_tests: run_test_mmanager run_test_list
	
# Run test cases for the memory manager
run_test_mmanager:
	./test_memory_manager

# Run test cases for the linked list
run_test_list:
	./test_linked_list

# Run test cases for the custom target test_listCG
run_test_listCG:
	./test_linked_listCG

# Clean target to clean up build files
clean:
	rm -f $(OBJ) $(LIB_NAME) test_memory_manager test_linked_list test_linked_listCG linked_list.o

