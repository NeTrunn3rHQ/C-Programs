# C Foundations: Memory, Data Structures, and Standard-Library Reimplementations

A collection of small, focused C programs exploring manual memory management, pointers, linked data structures, and byte/string operations.

This repository is intended to demonstrate **foundational systems-programming skills** through readable implementations and reproducible builds. These are learning exercises, not drop-in replacements for the C standard library or production container libraries.

## Projects

| Program | Concepts demonstrated |
|---|---|
| `dynamic_array.c` | `malloc`, `realloc`, capacity management, bounds and overflow checks |
| `linked_list.c` | Singly linked lists, pointer-to-pointer updates, insertion, deletion, reversal |
| `stack.c` | LIFO behavior, linked allocation, push/pop/peek |
| `queue.c` | FIFO behavior, front/back pointers, enqueue/dequeue |
| `my_memcpy.c` | Byte-wise copying and non-overlap contract |
| `my_memmove.c` | Overlapping memory regions and copy direction |
| `my_memset.c` | Byte-level memory initialization |
| `my_strcmp.c` | Lexicographic string comparison and unsigned-character handling |
| `my_strcpy.c` | Null-terminated string copying and destination capacity contract |
| `my_strlen.c` | Pointer traversal and pointer-difference length calculation |

## Repository layout

```text
.
├── README.md
├── Makefile
└── src/
    ├── dynamic_array.c
    ├── linked_list.c
    ├── stack.c
    ├── queue.c
    ├── my_memcpy.c
    ├── my_memmove.c
    ├── my_memset.c
    ├── my_strcmp.c
    ├── my_strcpy.c
    └── my_strlen.c
```

## Requirements

- GCC or Clang
- C11-compatible compiler
- Make (optional, for the provided build commands)

## Build all programs

```bash
make
```

Run an individual program:

```bash
./build/dynamic_array
./build/linked_list
./build/stack
./build/queue
```

The memory/string exercises use the same pattern:

```bash
./build/my_memcpy
./build/my_memmove
./build/my_memset
./build/my_strcmp
./build/my_strcpy
./build/my_strlen
```

On Windows, run the corresponding `.exe` files from `build/`.

## Build without Make

Example:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow \
    -Wstrict-prototypes -O2 -g src/linked_list.c -o linked_list
```

## Design and correctness notes

- Allocation results are checked before use.
- Owned dynamic memory is released by explicit destroy functions.
- Container operations report success or failure through return values.
- The memory/string routines operate on `unsigned char` where byte semantics matter.
- `my_memcpy` requires non-overlapping regions, as does the standard `memcpy`.
- `my_memmove` is written for conventional flat-address systems; its address-order test uses `uintptr_t` and is implementation-oriented.
- `my_strcpy` requires the destination buffer to be large enough and does not support overlapping source/destination regions.
- The data structures store `int` values to keep the focus on pointer and memory behavior.

## Scope

The examples intentionally favor clarity over advanced generic APIs, iterators, custom allocators, concurrency, or benchmarking. Each source file is independently buildable and includes a small demonstration in `main`.

## Next engineering improvements

Potential follow-on work includes dedicated unit-test files, allocation-failure injection, fuzz testing, sanitizer runs in CI, API/header separation, and performance benchmarks.
