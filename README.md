# Data Structure

A collection of fundamental **data structures** implemented in C, designed for learning, experimentation, and lightweight usage.  

Currently implemented:

- **Linked List** (`linked_list.h`)  
- **Array List** (`array_list.h`)  
- **Stack** (`stack.h`)  
- **Queue** (`queue.h`)  

Additional data structures are planned for future releases.

---

## Table of Contents

- [Features](#features)  
- [Usage](#usage)  
- [Memory Management](#memory-management)  
- [Tests](#tests)  
- [Notes](#notes)  
- [Future Plans](#future-plans)  

---

## Features

### Linked List

- Singly linked list implementation.  
- Supports basic operations (insertion, deletion, reversal, and element retrieval).  
- Generic type support with flexible `void*` element storage.  
- Memory management via optional `free_element` function.

### Array List

- Dynamic array-based list.  
- Supports append, insert, delete, and element retrieval.  
- Resizable on demand.  
- Generic type support with flexible `void*` element storage.

### Stack

- Built on top of `array_list`.  
- LIFO (Last-In-First-Out) operations: `push`, `pop`, `peek`.  
- `pop` transfers ownership of the element to the caller.  

### Queue

- Built on top of `linked_list`.  
- FIFO (First-In-First-Out) operations: `enqueue`, `dequeue`, `front`.  
- `dequeue` transfers ownership of the element to the caller.

---

## memory-management

- All data structures are ***NOT*** null-able, i.e. if we tried to insert a NULL element, it will refuse.
- When inserting an element, a pointer for that element (regardless of the type, weather it is simple int or a complex struct) is passed.
- Memory ownership rules (the following apply to all data structures in this repo):
1. When inserting a pointer of a user-owned memory (like heap allocated memory), the data structure will take the ownership of that memory, which mean that the data structure is  responsible of freeing that memory when deleting the memory pointer from the data structure and when freeing the whole data structure.
2. If the inserted pointer wasn't owned by the user (like a pointer to the stack or constant segment, read-only memory), the data structure shouldn't take ownership of that memory and thus shouldn't free that memory.
3. The user should know when the memory is owned by the data structure so they can pass a free_element function pointer that takes void* and returns void as an argument for deleting functions and freeing functions, the free_element function is responsible of freeing each element since the data structure has no idea about what the pointer points to, and if the memory pointed by the inner void* pointers isn't owned by the data structure, then when calling deleting functions, the user should pass NULL for free_element parameter to avoid freeing that memory.
4. for queue/stack: stack_pop and enqueue functions transfer memory ownership of that pointer to the caller, so the user should manage that memory and free it (if needed), while peek/front functions don't transfer memory ownership and just return void*.

---

## tests

- There is a unit test for each data structure that covers both normal and edge cases.
- to compile a test, move to test/ and run the following command:

```bash
gcc -std=c11 -Wall -Wextra -I.. -o ./test_name ../file_name ./test_file_name
./test_name
```

replace `test_name` by any name you want for the output executable, and replace `file_name` with the name of the C file that contain the implantation of the data structure (e.g. ./linked_list.c), and replace `test_file_name` with the name of the test file for that specific data structure (e.g. linked_list_test.c).

*Note:* for data structures that depend on other data structure like queue/stack, we should compile the depended files.
- Example for compiling stack test.
```bash
gcc -std=c11 -Wall -Wextra -I.. -o ./stack_test ../array_list.c ../stack.c ./stack_test.c
./stack_test
```

---

## notes

- specific details about the APIs are documented in the header file of each data structure.

## future_plans

- After implementing these linear data structures, I will start working on the non-linear ones like tree (specifically BST), hashmap, set, and graph.

