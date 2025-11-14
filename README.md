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
- Make deep copies of the inserted elements.
- Memory management via custom `free_element` function.

**Example:**

```c
#include "linked_list.h"

void* cpy_func(void* element){ // copy function takes a pointer to the element, allocate a heap space, copy the element to the heap, then return the pointer to the copy, this is used to make a deep copies of the inserted elements.
    int* cpy_int = malloc(sizeof(int));
    *cpy_int = *(int*) element;
    return cpy_int;
}

void free_func(void* element){ // free function takes a pointer to the elements and free its memory
    free(element);
}

int compare_func(void* element1, void* element2){ // compare function takes pointers to two elements and return 0 if they match, -1 otherwise
    return (*(int*)element1 == *(int*)element2? 0 : -1);
}

int main(){
    linked_list* list = create_linked_list(cpy_func, free_func, compare_func);
    int number = 42;
    llappend(list, &number); // list would have a deep copy of number (note: pass a pointer to the data)
    int* retrieved = llget(list, 0); 
    printf("%d\n", *retrieved);
    free_linked_list(list);
}
```

### Array List

- Dynamic array-based list.  
- Supports append, insert, delete, and element retrieval.  
- Resizable on demand (doubles when the list is full (length == max_size) and shrinks to half when the length <= 1/4 max_size).  
- Make deep copies of the inserted elements. 
- Generic type support with flexible `void*` element storage.

**Example:**

```c
#include "array_list.h"

void* cpy_func(void* element){ // copy function takes a pointer to the element, allocate a heap space, copy the element to the heap, then return the pointer to the copy, this is used to make a deep copies of the inserted elements.
    int* cpy_int = malloc(sizeof(int));
    *cpy_int = *(int*) element;
    return cpy_int;
}

void free_func(void* element){ // free function takes a pointer to the elements and free its memory
    free(element);
}

int compare_func(void* element1, void* element2){ // compare function takes pointers to two elements and return 0 if they match, -1 otherwise
    return (*(int*)element1 == *(int*)element2? 0 : -1);
}

int main(){
    array_list* list = create_array_list(cpy_func, free_func, compare_func);
    number = 42;
    alappend(list, &number); // a deep copy of number would be stored
    int* retrieved = alget(list, 0); 
    printf("%d\n", *retrieved);
    free_array_list(list);
}
```

### Stack

- Built on top of `array_list`.  
- LIFO (Last-In-First-Out) operations: `push`, `pop`, `peek`.  
- Make deep copies of the inserted elements. 
- `pop` transfers ownership of the element to the caller.  

**Example:**

```c
#include "stack.h"

void* cpy_func(void* element){ // copy function takes a pointer to the element, allocate a heap space, copy the element to the heap, then return the pointer to the copy, this is used to make a deep copies of the inserted elements.
    int* cpy_int = malloc(sizeof(int));
    *cpy_int = *(int*) element;
    return cpy_int;
}

void free_func(void* element){ // free function takes a pointer to the elements and free its memory
    free(element);
}

int compare_func(void* element1, void* element2){ // compare function takes pointers to two elements and return 0 if they match, -1 otherwise
    return (*(int*)element1 == *(int*)element2? 0 : -1);
}

int main(){
    stack* stck = create_stack(cpy_func, free_func, compare_func);
    int number = 99;
    stack_push(stck, &number); // the stack would make a deep copy of number
    int* top = stack_peek(stck); // No ownership transfer (the user should not free it outside the stack).
    printf("%d\n", *top);
    int* popped = stack_pop(stck); // Ownership transferred to caller (the user should free it)
    free(popped); // the user should free the popped element
    stack_free(stck);
}
```

### Queue

- Built on top of `linked_list`.  
- FIFO (First-In-First-Out) operations: `enqueue`, `dequeue`, `front`. 
- Make deep copies of the inserted elements. 
- `dequeue` transfers ownership of the element to the caller.

**Example:**

```c
#include "queue.h"

void* cpy_func(void* element){ // copy function takes a pointer to the element, allocate a heap space, copy the element to the heap, then return the pointer to the copy, this is used to make a deep copies of the inserted elements.
    int* cpy_int = malloc(sizeof(int));
    *cpy_int = *(int*) element;
    return cpy_int;
}

void free_func(void* element){ // free function takes a pointer to the elements and free its memory
    free(element);
}

int compare_func(void* element1, void* element2){ // compare function takes pointers to two elements and return 0 if they match, -1 otherwise
    return (*(int*)element1 == *(int*)element2? 0 : -1);
}

int main(){
    queue* qu = queue_create(cpy_func, free_func, compare_func);
    int num = 7;
    queue_enqueue(qu, &num); 
    int* front = queue_front(qu); // No ownership transfer
    printf("%d\n", *front);
    int* dequeued = queue_dequeue(qu); // Ownership transferred to the caller (the user should free it)
    free(dequeued);
    queue_free(qu);
}
```

---

## Memory Management

- All data structures are ***NOT*** null-able, i.e. if we tried to insert a NULL element, it will refuse.
- When creating a data structure, the user should pass cpy_func function pointer that takes void* and do a deep copy in the heap for the data and then return a pointer to the copy.
- When inserting an element, a pointer to for that element is passed, and the data structure make deep copies using cpy_func.
- The data structure stores pointers to the deep copies created by cpy_func, so it has full ownership of the memory.
- free_func function pointer is also passed to the constructor, this is used by the data structure to free the elements.
- For queue/stack: stack_pop and dequeue functions transfer memory ownership of that pointer to the caller, so the user should manage that memory and free it, while peek/front functions don't transfer memory ownership (the user shouldn't free it directly) and just return void*.

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

