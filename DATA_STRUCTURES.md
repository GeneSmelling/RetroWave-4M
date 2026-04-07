# Data Structures -- RetroWave 4M

This document describes the data types and data structures available in the RetroWave 4M BASIC
runtime. For the full language syntax, including array declarations and built-in functions, see
[BASIC_LANGUAGE.md](./BASIC_LANGUAGE.md).

---

## Primitive Variable Types

| Type | Suffix | Description | Example |
|------|--------|-------------|---------|
| Integer | `%` | Whole numbers (32-bit signed) | `DIM score% = 0` |
| Float | `#` | Decimal numbers (64-bit double) | `DIM pi# = 3.14159` |
| String | `$` | Sequence of Unicode characters | `DIM name$ = "RetroWave"` |
| Boolean | `?` | Logical true or false | `DIM active? = TRUE` |

Type suffixes are used to distinguish variable types in BASIC. Implicit typing is not supported.

---

## Arrays

- **Single-dimensional:** `DIM scores%(10)` -- indices 0 to 10.
- **Multi-dimensional:** `DIM grid%(8, 8)` -- rows x columns.
- **Dynamic resize:** `REDIM arr%(newSize)` -- resizes at runtime (existing values preserved up
  to the smaller of old/new size).

---

## Record Structures

Records group multiple fields into a single named type.

```basic
TYPE Address
    Street AS STRING
    City   AS STRING
    Zip    AS STRING
END TYPE

TYPE Person
    Name    AS STRING
    Age     AS INTEGER
    Address AS Address
END TYPE

DIM p AS Person
p.Name       = "Jane"
p.Age        = 30
p.Address.City = "Rome"
```

- Nested records are supported (a field may itself be a record type).
- Arrays of records: `DIM people(100) AS Person`.

---

## First-Class Data Structures

All abstract data structures below are declared with `DIM ... AS` and operated on with
structure-prefixed commands. They are generic: replace `INTEGER` / `STRING` with any type.

| Structure | Declaration example | Notes |
|-----------|--------------------|----|
| **List** | `DIM myList AS LIST OF INTEGER` | Ordered; allows duplicates; index access |
| **Tree** | `DIM myTree AS TREE OF STRING` | Binary search tree; in-order traversal |
| **Graph** | `DIM myGraph AS GRAPH` | Directed or undirected; weighted edges supported |
| **Stack** | `DIM myStack AS STACK OF FLOAT` | LIFO; push/pop |
| **Queue** | `DIM myQueue AS QUEUE OF STRING` | FIFO; enqueue/dequeue |
| **Set** | `DIM mySet AS SET OF INTEGER` | Unique values; union/intersection/difference |
| **Map** | `DIM myMap AS MAP OF STRING TO INTEGER` | Key-value pairs; O(1) average lookup |
| **Priority Queue** | `DIM myPQ AS PRIORITY QUEUE OF INTEGER` | Min-heap by default |
| **Deque** | `DIM myDeque AS DEQUE OF STRING` | Double-ended queue; push/pop front and back |

### Operation naming convention

Operations use the structure name as a prefix:

```basic
' List
DIM items AS LIST OF INTEGER
LIST APPEND items, 42
LIST APPEND items, 17
PRINT LIST SIZE(items)        ' 2
PRINT LIST GET(items, 0)      ' 42

' Map
DIM scores AS MAP OF STRING TO INTEGER
MAP SET scores, "Alice", 95
MAP SET scores, "Bob",   88
PRINT MAP GET(scores, "Alice")   ' 95
PRINT MAP SIZE(scores)           ' 2

' Stack
DIM stk AS STACK OF INTEGER
STACK PUSH stk, 10
STACK PUSH stk, 20
PRINT STACK POP(stk)   ' 20

' Queue
DIM q AS QUEUE OF STRING
QUEUE ENQUEUE q, "first"
QUEUE ENQUEUE q, "second"
PRINT QUEUE DEQUEUE(q)   ' "first"

' Set
DIM s AS SET OF INTEGER
SET ADD s, 1 : SET ADD s, 2 : SET ADD s, 1
PRINT SET SIZE(s)   ' 2 (duplicates ignored)

' Priority Queue (min-heap)
DIM pq AS PRIORITY QUEUE OF INTEGER
PQ INSERT pq, 5
PQ INSERT pq, 1
PQ INSERT pq, 3
PRINT PQ POP(pq)   ' 1 (minimum value)
```

---

## Matrix Type

A dedicated mathematical matrix type (not a plain 2D array):

```basic
MAT DIM a(3, 3)
MAT DIM b(3, 3)
MAT MUL a, b           ' matrix multiplication; result in a
MAT TRANS a            ' transpose a in place
MAT ROT a, "Z", 45.0   ' apply 45-degree rotation matrix around Z axis
PRINT MAT DET(a)       ' determinant
MAT INV a              ' invert a in place (must be square and non-singular)
```

| Command | Description |
|---------|-------------|
| `MAT DIM m(r, c)` | Declare an r x c matrix |
| `MAT ADD m1, m2` | Element-wise addition (result in m1) |
| `MAT MUL m1, m2` | Matrix multiplication (result in m1) |
| `MAT TRANS m` | Transpose m in place |
| `MAT ROT m, axis$, degrees#` | Rotation matrix around "X", "Y", or "Z" |
| `MAT DET(m)` | Determinant (square matrices only) |
| `MAT INV m` | Invert m in place (square, non-singular only) |

---

## SQLite Integration

SQLite databases are a supported storage format in RetroWave 4M BASIC. All SQL parameters MUST
use bind arguments; string interpolation into SQL statements is not permitted.

```basic
10 DB OPEN "mydata.db" AS #1
20 DB EXEC  #1, "CREATE TABLE IF NOT EXISTS items (id INTEGER PRIMARY KEY, name TEXT)"
30 DB EXEC  #1, "INSERT INTO items (name) VALUES (?)", itemName$
40 DB QUERY #1, rs, "SELECT id, name FROM items WHERE id = ?", itemId%
50 WHILE NOT DB EOF(rs)
60   PRINT DB FIELD(rs, "id"), DB FIELD(rs, "name")
70   DB NEXT rs
80 WEND
90 DB CLOSE #1
```

| Command | Description |
|---------|-------------|
| `DB OPEN file$ AS #n` | Open (or create) a SQLite database file |
| `DB EXEC #n, sql$ [, params...]` | Execute a non-SELECT statement with bind parameters |
| `DB QUERY #n, rs, sql$ [, params...]` | Execute a SELECT; `rs` is the result-set handle |
| `DB NEXT rs` | Advance the result-set cursor |
| `DB EOF(rs)` | Returns TRUE when no more rows |
| `DB FIELD(rs, col$)` | Returns the value of the named column in the current row |
| `DB BEGIN #n` | Begin a transaction |
| `DB COMMIT #n` | Commit a transaction |
| `DB ROLLBACK #n` | Roll back a transaction |
| `DB CLOSE #n` | Close the database |

---

## File I/O and Storage Safety

| Rule | Detail |
|------|--------|
| Max file size | 200 MB per file |
| Forbidden paths | System directories; hidden directories; files outside user data dir |
| Forbidden types | Executables, batch files, autorun scripts |
| Hidden files | Surfaced as visible (no hidden-file bypass) |
| Delete | All deletions go to Trashcan; permanent delete requires a second step |

Standard file commands: `OPEN`, `CLOSE`, `READ`, `WRITE`, `INPUT #`, `PRINT #`, `EOF()`.

---

## Cross-References

- [BASIC_LANGUAGE.md](./BASIC_LANGUAGE.md) -- full syntax, control flow, functions, TTS,
  graphics, and sound commands.
- [ARCHITECTURE.md](./ARCHITECTURE.md) -- storage sandbox, SQLite security posture.
