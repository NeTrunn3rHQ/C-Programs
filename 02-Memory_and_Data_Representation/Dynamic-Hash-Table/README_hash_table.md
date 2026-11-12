# Dynamic Hash Table in C

A dynamically resizing hash table that maps strings to integer values. It uses **separate chaining** to handle collisions and the **djb2** string hash function to select buckets.

## Features

- Insert, search, and delete key/value pairs
- Separate chaining with linked lists
- Automatic capacity doubling when the load factor reaches `0.75`
- Manual resizing with `ht_resize`
- Duplicate keys are rejected; existing values are not overwritten
- Each node owns a copy of its key
- `ht_destroy` frees all allocated memory and resets the table

## Requirements

- A C compiler with C11 support (GCC or Clang)

## Build

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow \
    -Wstrict-prototypes -O2 -g 05_dynamic_hash_table.c -o hash_table
```

## Run

```bash
./hash_table
```

On Windows:

```powershell
.\hash_table.exe
```

Expected output:

```text
ALL TESTS PASSED!
```

## API

| Function | Description | Return value |
|---|---|---|
| `ht_init` | Initialize a table with an initial capacity | `0` on success, `1` on failure |
| `ht_insert` | Insert a new string/integer pair | `0` on success, `1` for invalid input, duplicate key, or allocation failure |
| `ht_search` | Find a key and optionally retrieve its value | `0` if found, `1` otherwise |
| `ht_delete` | Remove a key and free its memory | `0` if deleted, `1` otherwise |
| `ht_resize` | Rebuild the bucket array with a new capacity | `0` on success, `1` on failure |
| `ht_destroy` | Free all table memory and reset its fields | No return value |

The `value` argument to `ht_search` may be `NULL` when only key existence needs to be checked.

## Design notes

### Collision handling

Different keys can produce the same bucket index. Each bucket therefore points to a linked list of nodes. This technique is called **separate chaining**.

### Resizing

The table doubles its capacity when:

```c
(size / capacity) >= 0.75
```

Resizing allocates a new bucket array and re-links the existing nodes using their new bucket indexes. The nodes and their key strings are not copied.

If an automatic resize allocation fails after an insertion, the insertion is still reported as successful and the table remains valid at its previous capacity.

### Ownership

The table makes its own copy of every inserted key. The caller can modify or release the original input string after a successful insertion. Call `ht_destroy` when the table is no longer needed.

## Current limitations

- Keys are case-sensitive.
- Values are `int`.
- Duplicate keys are rejected rather than updated.
- The implementation is not thread-safe.
- The simple API uses `0` for success and `1` for failure; it does not expose detailed error codes.

## Learning goals

This project demonstrates:

- Dynamic memory allocation (`malloc`, `calloc`, `free`)
- Structs and linked lists
- Pointer manipulation
- Hash functions and collision handling
- Load factors and dynamic resizing
- Basic API design and assertion-based testing
