# Valgrind Analysis

---

## Program: stack_example.c

### Valgrind Output (simplified)

Invalid read of size 4  
Address 0x... is not stack'd, malloc'd or (recently) free'd

---

### Error Type

Invalid read → use of invalid memory  
More specifically: **dangling pointer dereference**

---

### Memory Object Involved

- Local variable `x` declared inside function `get_value()`
- Stored in the stack frame of that function

---

### Root Cause Analysis

The function `get_value()` returns the address of a local variable:
