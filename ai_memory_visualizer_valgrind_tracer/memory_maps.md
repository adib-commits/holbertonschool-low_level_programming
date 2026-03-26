# Memory Maps Analysis

---

## Program: stack_example.c

### Assumed Code Behavior
A function returns a pointer to a local variable.

---

### Step 1: main() entry

Stack:
- main()
  - int *ptr (uninitialized)

Heap:
- (empty)

---

### Step 2: call to function get_value()

Stack:
- main()
  - ptr (uninitialized)
- get_value()
  - int x = 42

Heap:
- (empty)

---

### Step 3: returning address of x

Stack:
- main()
  - ptr → address of x (e.g. 0x7ffe1234)
- get_value()
  - x = 42

Heap:
- (empty)

---

### Step 4: function returns (CRITICAL POINT)

Stack:
- main()
  - ptr → 0x7ffe1234

The stack frame of get_value() is destroyed.
Variable x no longer exists.

Heap:
- (empty)

---

### Step 5: dereferencing ptr

Stack:
- main()
  - ptr → invalid address (dangling pointer)

Result:
Undefined behavior (may print garbage or crash)

---

### Analysis

The function returns the address of a local variable stored on the stack.

Once the function returns:
- The stack frame is destroyed
- The variable x is no longer valid
- The pointer becomes a dangling pointer

This leads to undefined behavior when dereferenced.

---

### AI Mistake Example

AI Claim:
"The pointer remains valid because the value was not modified."

Why this is incorrect:
- Validity depends on memory lifetime, not value
- The variable was allocated on the stack
- Its lifetime ended when the function returned

Correction:
The pointer is invalid after function return because it points to destroyed stack memory.

---

## Program: aliasing_example.c

### Assumed Code Behavior
Two pointers refer to the same heap memory.

---

### Step 1: malloc allocation

Stack:
- main()
  - int *a

Heap:
- 0x1000 → int value = 10

Pointer:
- a → 0x1000

---

### Step 2: alias creation

Stack:
- main()
  - int *a → 0x1000
  - int *b → 0x1000

Heap:
- 0x1000 → int value = 10

a and b point to the same memory

---

### Step 3: modifying value through b

Operation:
*b = 20

Stack:
- a → 0x1000
- b → 0x1000

Heap:
- 0x1000 → int value = 20

---

### Step 4: freeing memory through a

Operation:
free(a)

Stack:
- a → 0x1000 (invalid)
- b → 0x1000 (invalid)

Heap:
- memory at 0x1000 is freed

---

### Step 5: using b after free (CRITICAL)

Operation:
*b = 30

Result:
Use-after-free → undefined behavior

---

### Analysis

Both pointers a and b alias the same heap memory.

After free(a):
- The memory is deallocated
- Both a and b become invalid (dangling pointers)

Using b after free results in a use-after-free bug.

---

### AI Mistake Example

AI Claim:
"Only pointer a becomes invalid after free."

Why this is incorrect:
- free() releases the memory block, not the pointer
- All pointers referencing that memory become invalid

Correction:
Both a and b are dangling pointers after free().

---

## Key Takeaways

- Stack variables exist only during function execution
- Returning their address leads to dangling pointers
- Heap memory must be managed manually
- Multiple pointers can reference the same memory (aliasing)
- Freeing memory invalidates ALL aliases
- Using invalid pointers leads to undefined behavior
