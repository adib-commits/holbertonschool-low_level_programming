# Memory Maps – AI Memory Visualizer & Valgrind Tracer

**Author:** Student Analysis  
**Project:** ai_memory_visualizer_valgrind_tracer  
**Directory:** analysis/

---

## Table of Contents

1. [stack\_example.c – Stack Memory Analysis](#1-stack_examplec--stack-memory-analysis)
2. [aliasing\_example.c – Pointer Aliasing Analysis](#2-aliasing_examplec--pointer-aliasing-analysis)
3. [heap\_example.c – Heap Memory Analysis](#3-heap_examplec--heap-memory-analysis)
4. [AI Error Documentation](#4-ai-error-documentation)
5. [Terminology Reference](#5-terminology-reference)

---

## 1. `stack_example.c` – Stack Memory Analysis

### Program Overview

```c
#include <stdio.h>

int add(int a, int b)
{
    int result = a + b;
    return result;
}

int main(void)
{
    int x = 3;
    int y = 5;
    int sum = add(x, y);
    printf("Sum: %d\n", sum);
    return 0;
}
```

*(Note: The actual file contents may vary. This analysis reflects the canonical stack_example.c pattern used in this module.)*

---

### Execution Point 1 – Entry into `main()`

When `main()` is called, the OS/runtime creates a **stack frame** for it. The stack grows downward in memory on x86/x86-64 architectures.

```
STACK (grows downward)
┌─────────────────────────────────┐  ← Higher addresses
│         ...caller data...       │
├─────────────────────────────────┤
│  [main frame]                   │
│  x   = 3         (int, 4 bytes) │  ← local variable
│  y   = 5         (int, 4 bytes) │  ← local variable
│  sum = ?         (int, 4 bytes) │  ← uninitialized until add() returns
│  return address                 │  ← saved by call instruction
├─────────────────────────────────┤  ← Stack pointer (SP)
│         (unused stack space)    │
└─────────────────────────────────┘  ← Lower addresses

HEAP
┌─────────────────────────────────┐
│         (empty – no malloc)     │
└─────────────────────────────────┘
```

**Variable lifetimes at this point:**
- `x`, `y`: **alive**, allocated on `main`'s stack frame.
- `sum`: allocated but **not yet initialized** (contains garbage value until `add()` returns).

---

### Execution Point 2 – Call to `add(x, y)`

When `add()` is called, a **new stack frame** is pushed. The arguments `a` and `b` are **copies** of `x` and `y` — this is pass-by-value.

```
STACK (grows downward)
┌─────────────────────────────────┐  ← Higher addresses
│  [main frame]                   │
│  x   = 3                        │
│  y   = 5                        │
│  sum = ? (not yet written)      │
│  return address to caller       │
├─────────────────────────────────┤
│  [add frame]                    │
│  a      = 3      (copy of x)    │  ← parameter (copy, independent)
│  b      = 5      (copy of y)    │  ← parameter (copy, independent)
│  result = 8      (int, 4 bytes) │  ← local variable
│  return address → main          │
├─────────────────────────────────┤  ← Stack pointer (SP)
│         (unused)                │
└─────────────────────────────────┘
```

**Key observations:**
- `a` and `b` are **not** `x` and `y`. They are independent copies. Modifying `a` inside `add()` has **no effect** on `x` in `main`.
- `result` is a **local variable** of `add`. Its lifetime begins here and ends when `add` returns.
- There is **no heap activity**. All data lives exclusively on the stack.

---

### Execution Point 3 – Return from `add()`

When `add()` returns:

1. The return value (8) is passed back via a CPU register (typically `eax`/`rax` on x86).
2. The stack frame of `add` is **popped** (SP is moved back up).
3. `result` no longer exists — its memory is no longer valid to access.
4. In `main`, `sum` is now written with the return value.

```
STACK (grows downward)
┌─────────────────────────────────┐
│  [main frame]                   │
│  x   = 3                        │
│  y   = 5                        │
│  sum = 8    ← now written       │
│  return address                 │
├─────────────────────────────────┤  ← Stack pointer (SP)
│  [add frame DESTROYED]          │
│  (memory still physically       │
│   present but INVALID to read)  │
└─────────────────────────────────┘
```

> ⚠️ **Critical point:** The memory where `add`'s frame lived is not erased. It is simply no longer owned. If another function is called, it will overwrite those bytes. Reading from a pointer to `result` after `add()` returns is **use-after-free** on the stack (dangling pointer to a dead frame), and constitutes **undefined behavior** in C.

---

### Execution Point 4 – Return from `main()`

When `main` returns, its frame is also popped. All local variables (`x`, `y`, `sum`) cease to exist. No heap memory was allocated, so no `free()` is needed.

**No memory leaks. No heap activity.**

---

## 2. `aliasing_example.c` – Pointer Aliasing Analysis

### Program Overview

```c
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *p = (int *)malloc(sizeof(int));
    int *q = p;  /* alias: q and p point to the same heap block */

    *p = 42;
    printf("*q = %d\n", *q);  /* prints 42 */

    free(p);
    /* q is now a dangling pointer — the memory it points to is freed */
    /* accessing *q here is undefined behavior */

    return 0;
}
```

*(Adapted to reflect the typical aliasing_example.c pattern for this module.)*

---

### Execution Point 1 – `malloc()` call and first assignment

```
STACK
┌──────────────────────────────────────┐
│  [main frame]                        │
│  p = 0x601010  (pointer, 8 bytes)   │  ← holds heap address
│  q = ???       (not yet assigned)    │
└──────────────────────────────────────┘

HEAP
┌──────────────────────────────────────┐
│  [block at 0x601010]                 │
│  size: sizeof(int) = 4 bytes         │
│  content: ??? (uninitialized)        │
│  status: ALLOCATED                   │
└──────────────────────────────────────┘
```

- `malloc` allocates 4 bytes on the heap and returns the **address** of that block.
- `p` stores that address. `p` **owns** the allocation — it is responsible for calling `free`.

---

### Execution Point 2 – Alias created: `q = p`

```
STACK
┌──────────────────────────────────────┐
│  p = 0x601010   ──────────────┐      │
│  q = 0x601010   ──────────────┤      │
└───────────────────────────────┼──────┘
                                │
HEAP                            ▼
┌──────────────────────────────────────┐
│  [block at 0x601010]                 │
│  content: 42 (after *p = 42)         │
│  status: ALLOCATED                   │
└──────────────────────────────────────┘
```

**Aliasing:** Both `p` and `q` contain the value `0x601010`. They are **two pointers to the same memory location**. Writing through `p` is immediately visible through `q` and vice versa. They are **not independent copies** of the data.

**Ownership:** Only `p` is the "owner" by convention — it was the one that received the return value of `malloc`. However, C does not enforce this. Either pointer could be passed to `free`. What is **illegal** is calling `free` twice on the same address (double-free).

---

### Execution Point 3 – `free(p)` called

```
STACK
┌──────────────────────────────────────┐
│  p = 0x601010  (value unchanged!)    │  ← NOW a dangling pointer
│  q = 0x601010  (value unchanged!)    │  ← NOW a dangling pointer
└──────────────────────────────────────┘

HEAP
┌──────────────────────────────────────┐
│  [block at 0x601010]                 │
│  status: FREED (returned to allocator│
│  content: undefined (may be changed) │
└──────────────────────────────────────┘
```

> ⚠️ **Critical:** After `free(p)`, the memory at `0x601010` is returned to the allocator. However, `p` and `q` **still contain the address `0x601010`**. They are now **dangling pointers**. Dereferencing either (`*p` or `*q`) is **undefined behavior**. The program may crash, return garbage, or appear to work (which is the most dangerous outcome, as it masks the bug).

**What `free()` does NOT do:**
- It does **not** set `p` to `NULL`.
- It does **not** erase the memory content.
- It does **not** notify `q` that the memory is freed.

This is why aliasing is dangerous: freeing through one alias leaves all other aliases dangling without any indication.

**Best practice:** After `free(p)`, set `p = NULL` and `q = NULL` immediately to prevent accidental use.

---

### Valgrind Output for Aliasing Bug

If `*q` were accessed after `free(p)`, Valgrind would report:

```
==PID== Invalid read of size 4
==PID==    at 0x...: main (aliasing_example.c:12)
==PID==  Address 0x601010 is 0 bytes inside a block of size 4 free'd
==PID==    at 0x...: free (vg_replace_malloc.c:...)
==PID==    at 0x...: main (aliasing_example.c:10)
```

This is a **use-after-free** error: reading from memory that has already been freed.

---

## 3. `heap_example.c` – Heap Memory Analysis

### Program Overview

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *create_string(const char *src)
{
    char *buf = (char *)malloc(strlen(src) + 1);
    if (buf == NULL)
        return NULL;
    strcpy(buf, src);
    return buf;
}

int main(void)
{
    char *s1 = create_string("hello");
    char *s2 = create_string("world");

    printf("%s %s\n", s1, s2);

    free(s1);
    free(s2);
    return 0;
}
```

---

### Execution Point 1 – First call: `create_string("hello")`

```
STACK
┌──────────────────────────────────────────┐
│  [main frame]                            │
│  s1 = ??? (not yet assigned)             │
│  s2 = ??? (not yet assigned)             │
├──────────────────────────────────────────┤
│  [create_string frame]                   │
│  src = 0x400600  → "hello" (read-only)  │
│  buf = 0x601010  → heap block           │
└──────────────────────────────────────────┘

HEAP
┌──────────────────────────────────────────┐
│  [block at 0x601010] – 6 bytes           │
│  content: "hello\0"                      │
│  status: ALLOCATED                       │
│  owner:  buf (in create_string frame)    │
└──────────────────────────────────────────┘
```

**Ownership transfer:** When `create_string` returns `buf`, it returns the **heap address**. The local variable `buf` (on the stack) is destroyed, but the **heap block persists**. Ownership transfers to the caller through `s1`. The heap block outlives the function that created it — this is the fundamental purpose of heap allocation.

---

### Execution Point 2 – After both calls, before `free()`

```
STACK
┌──────────────────────────────────────────┐
│  [main frame]                            │
│  s1 = 0x601010  ────────────────┐        │
│  s2 = 0x601050  ──────────┐     │        │
└───────────────────────────┼─────┼────────┘
                            │     │
HEAP                        │     │
┌─────────────────────────────────┐
│  [block at 0x601010] – 6 bytes  │◄───── s1
│  content: "hello\0"             │
│  status: ALLOCATED              │
├─────────────────────────────────┤
│  [block at 0x601050] – 6 bytes  │◄───── s2
│  content: "world\0"             │
│  status: ALLOCATED              │
└─────────────────────────────────┘
```

The two blocks are **independent**. `s1` and `s2` do not alias each other.

---

### Execution Point 3 – After `free(s1)` and `free(s2)`

```
HEAP
┌─────────────────────────────────┐
│  [block at 0x601010] – FREED    │
├─────────────────────────────────┤
│  [block at 0x601050] – FREED    │
└─────────────────────────────────┘
```

**No memory leak.** Both blocks were properly freed. Valgrind would report:
```
==PID== LEAK SUMMARY:
==PID==    definitely lost: 0 bytes in 0 blocks
==PID==    All heap blocks were freed -- no leaks are possible
```

### Memory Leak Variant

If `free(s1)` is **omitted**, Valgrind reports:

```
==PID== LEAK SUMMARY:
==PID==    definitely lost: 6 bytes in 1 blocks
==PID==    at 0x...: malloc (vg_replace_malloc.c:...)
==PID==    at 0x...: create_string (heap_example.c:7)
==PID==    at 0x...: main (heap_example.c:16)
```

The heap block for `"hello"` was allocated but never freed. It is a **definite leak**: no pointer in scope can ever reach it again after `main` returns (since `s1` is destroyed). The memory is lost for the duration of the process.

---

## 4. AI Error Documentation

This section documents a specific case where AI-generated output was **incorrect** and required manual correction.

---

### Case: AI Explanation of `free()` Behavior with Aliases

**Prompt given to AI:**

> "After calling `free(p)` where `q = p`, what happens to `q`?"

**AI-generated explanation (paraphrased):**

> "After `free(p)` is called, the memory is released. The pointer `q` becomes NULL automatically since it pointed to the same memory. You should still set `q = NULL` manually as good practice, but the system handles cleanup."

---

### Why This Is Wrong

The AI made **two factual errors**:

**Error 1: `q` does not become `NULL` automatically.**

In C, `free()` has the following signature:
```c
void free(void *ptr);
```

It receives a **copy** of the pointer value. It cannot modify `p` itself, let alone `q`. After `free(p)`:
- `p` still holds the old address (e.g., `0x601010`).
- `q` still holds the old address (`0x601010`).
- Neither is set to `NULL`.

This can be verified empirically:
```c
int *p = malloc(sizeof(int));
int *q = p;
free(p);
printf("p = %p\n", (void *)p);  /* prints 0x601010, not (nil) */
printf("q = %p\n", (void *)q);  /* prints 0x601010, not (nil) */
```

**Error 2: "The system handles cleanup" is misleading.**

The C runtime does **not** track all aliases to a freed block. It only manages the allocator's internal free-list. No notification is sent to `q`. The programmer is solely responsible for ensuring no dangling pointer is accessed.

---

### Corrected Explanation

After `free(p)`:
- The heap block at `0x601010` is returned to the allocator's free pool.
- `p` remains equal to `0x601010` — it is now a **dangling pointer**.
- `q` remains equal to `0x601010` — it is also a **dangling pointer**.
- Dereferencing either pointer is **undefined behavior**.
- The correct practice is to set **both** `p = NULL` and `q = NULL` immediately after `free`.
- This must be done **manually**; nothing in C does it automatically.

```
Corrected memory state after free(p):

STACK
  p = 0x601010  ──►  [FREED block – DO NOT ACCESS]
  q = 0x601010  ──►  [FREED block – DO NOT ACCESS]

The value of p and q is unchanged.
The block they point to is invalid.
```

---

### Why This Matters

The AI's answer was plausible but wrong. If a student had trusted it, they would believe that:
1. One `free()` is sufficient to "clean up" all aliases automatically.
2. Dangling pointers are not a concern if "the system handles it."

In reality, this exact misunderstanding causes **use-after-free vulnerabilities** — a major class of security and stability bugs in C programs. The mistake is undetectable by the compiler; only tools like Valgrind expose it at runtime.

---

## 5. Terminology Reference

| Term | Definition |
|------|-----------|
| **Stack frame** | A region of the stack allocated for a single function call, containing local variables, parameters, and the return address. Destroyed when the function returns. |
| **Heap** | Memory region managed explicitly via `malloc`/`free`. Persists beyond the function that allocated it, until `free()` is called. |
| **Dangling pointer** | A pointer that holds an address to memory that has already been freed or gone out of scope. Dereferencing it is undefined behavior. |
| **Aliasing** | Two or more pointers pointing to the same memory location. Writes through one are visible through the other. |
| **Memory leak** | A heap allocation that is never freed, and no live pointer can reach it to free it later. |
| **Use-after-free** | Accessing memory after it has been freed. A subset of undefined behavior. |
| **Double-free** | Calling `free()` twice on the same pointer. Corrupts the allocator's internal state and causes undefined behavior. |
| **Undefined behavior (UB)** | A program action whose result is not specified by the C standard. The compiler may assume UB never occurs, leading to surprising optimizations and crashes. |
| **Valgrind** | A dynamic analysis tool that instruments program execution to detect memory errors: leaks, invalid accesses, use-after-free, and uninitialized reads. |
| **Ownership** | The convention (not enforced by C) that one pointer is responsible for freeing a given allocation. |

---

*End of analysis — `analysis/memory_maps.md`*
