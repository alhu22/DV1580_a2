# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Files
MEM_MANAGER_SRC = memory_manager.c linked_list.c
MEM_MANAGER_HDR = memory_manager.h linked_list.h

# Executable for testing memory manager
TEST_MEMORY_MANAGER_EXEC = test_memory_manager
TEST_MEMORY_MANAGER_SRC = test_memory_manager.c

# Executable for testing linked list
TEST_LINKED_LIST_EXEC = test_linked_list
TEST_LINKED_LIST_SRC = test_linked_list.c

# Default target (will build both test executables)
all: $(TEST_MEMORY_MANAGER_EXEC) $(TEST_LINKED_LIST_EXEC)

# Rule for building test_memory_manager binary
$(TEST_MEMORY_MANAGER_EXEC): $(MEM_MANAGER_SRC) $(TEST_MEMORY_MANAGER_SRC) $(MEM_MANAGER_HDR)
	$(CC) $(CFLAGS) -o $(TEST_MEMORY_MANAGER_EXEC) $(MEM_MANAGER_SRC) $(TEST_MEMORY_MANAGER_SRC)

# Rule for building test_linked_list binary
$(TEST_LINKED_LIST_EXEC): $(MEM_MANAGER_SRC) $(TEST_LINKED_LIST_SRC) $(MEM_MANAGER_HDR)
	$(CC) $(CFLAGS) -o $(TEST_LINKED_LIST_EXEC) $(MEM_MANAGER_SRC) $(TEST_LINKED_LIST_SRC)

# Clean rule to remove compiled binaries
clean:
	rm -f $(TEST_MEMORY_MANAGER_EXEC) $(TEST_LINKED_LIST_EXEC)
