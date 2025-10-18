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

**Example:**

```c
#include "linked_list.h"
linked_list* list = create_linked_list();
int* number = malloc(sizeof(int));
*number = 42;
llappend(list, value); // Ownership transferred to list, it SHOULD NOT be freed via the outside 'number' pointer
int* retrieved = llget(list, 0); // the list still owns the memory, you should not free it using this pointer
printf("%d\n", *retrieved);
free_linked_list(list, free); // pass free function pointer to free the memory pointed by the inner pointers
```

```c
#include "linked_list.h"
linked_list* list = create_linked_list();
llappend(list, "text"); // since we appended non-user-owned memory (read-only memory), neither the user nor the list should free it
int* retrieved = llget(list, 0);
printf("%s\n", *retrieved);
free_linked_list(list, NULL); // pass NULL for the free function pointer to avoid freeing non-owned memory.
```

### Array List

- Dynamic array-based list.  
- Supports append, insert, delete, and element retrieval.  
- Resizable on demand (doubles when the list is full (length == max_size) and shrinks to half when the length <= 1/4 max_size).  
- Generic type support with flexible `void*` element storage.

**Example:**

```c
#include "array_list.h"
array_list* list = create_array_list();
int* number = malloc(sizeof(int));
*number = 42;
alappend(list, value); // Ownership transferred to list, it SHOULD NOT be freed via the outside 'number' pointer
int* retrieved = alget(list, 0); // the list still owns the memory, you should not free it using this pointer
printf("%d\n", *retrieved);
free_array_list(list, free); // pass free function pointer to free the memory pointed by the inner pointers
```

```c
#include "array_list.h"
array_list* list = create_array_list();
int number = 9;
alappend(list, &number); // since we appended non-user-owned memory (stack memory), neither the user nor the list should free it
int* retrieved = alget(list, 0);
printf("%d\n", *retrieved);
free_array_list(list, NULL); // pass NULL for the free function pointer to avoid freeing non-owned memory.
```

### Stack

- Built on top of `array_list`.  
- LIFO (Last-In-First-Out) operations: `push`, `pop`, `peek`.  
- `pop` transfers ownership of the element to the caller.  

**Example:**

```c
stack* stck = create_stack();
int* val = malloc(sizeof(int));
*val = 99;
stack_push(stck, val); // ownership transfer to the stack
int* top = stack_peek(stck); // No ownership transfer (the user should not free it outside the stack)
printf("%d\n", *top);
int* popped = stack_pop(stck); // Ownership transferred to caller (the user should free it (if possible))
free(popped); // the user should free the popped element (if freeable)
stack_free(stck, free); // pass free function to free the element (the same as array_list)
```

### Queue

- Built on top of `linked_list`.  
- FIFO (First-In-First-Out) operations: `enqueue`, `dequeue`, `front`.  
- `dequeue` transfers ownership of the element to the caller.

**Example:**

```c
queue* qu = queue_create();
int* num = malloc(sizeof(int));
*num = 7;
queue_enqueue(qu, num); // ownership transfer to the queue
int* front = queue_front(queue); // No ownership transfer
printf("%d\n", *front);
int* dequeued = queue_dequeue(queue); // Ownership transferred to the caller (the user should free it)
free(dequeued);
queue_free(queue, free);
```

---

## Memory Management

- All data structures are ***NOT*** null-able, i.e. if we tried to insert a NULL element, it will refuse.
- When inserting an element, a pointer for that element (regardless of the type, whether it is simple int or a complex struct) is passed.
- Memory ownership rules (the following apply to all data structures in this repo):
1. When inserting a pointer of a user-owned memory (like heap allocated memory), the data structure will take the ownership of that memory, which mean that the data structure is responsible of freeing that memory when deleting the memory pointer from the data structure and when freeing the whole data structure (which also means that once the user inserted any element in the data structure, they ***SHOULD NOT*** free it outside the data structure).
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

replace `test_name` by any name you want for the output executable, and replace `file_name` with the name of the C file that contain the implementation of the data structure (e.g. linked_list.c), and replace `test_file_name` with the name of the test file for that specific data structure (e.g. linked_list_test.c).

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

