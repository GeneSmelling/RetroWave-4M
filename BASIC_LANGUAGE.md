# BASIC Language Reference -- RetroWave 4M

RetroWave 4M BASIC is a line-numbered, non-object-oriented dialect extended with structured
programming constructs, Unicode support, first-class data structures, SQLite integration,
graphics, and sound commands. The runtime is strictly monotasking: one program runs at a time
(background audio and network I/O are the only permitted parallel activities).

> For data structures see [DATA_STRUCTURES.md](./DATA_STRUCTURES.md).
> For graphics commands see [GRAPHICS.md](./GRAPHICS.md).
> For sound and TTS see [SOUND.md](./SOUND.md).
> For accessibility commands (`VOICE ON`, `SAYSTOP`, etc.) see [ACCESSIBILITY.md](./ACCESSIBILITY.md).

---

## Data Types
- **Integer `%`**: Represents whole numbers.
- **Float `#`**: Represents decimal numbers.
- **String `$`**: Represents sequences of characters.
- **Boolean `?`**: Represents truth values (`True` or `False`).

## Arrays
- **Single-Dimensional**: An array with a single index.
- **Multi-Dimensional**: Arrays with multiple indices. Can be declared like `DIM arr(10, 10)`. 
- **Dynamic REDIM**: Allows resizing of arrays at runtime using `REDIM` keyword.

## Record Structures
- **RECORD...END RECORD**: Used to define complex data types.
- Supports nested records and record arrays.
- Example:
  ```
  TYPE Person
      Name AS STRING
      Age AS INTEGER
      Address AS RECORD
          Street AS STRING
          City AS STRING
      END RECORD
  END TYPE
  ```

## Control Flow
- **IF/THEN/ELSE**: Conditional execution.
- **FOR/NEXT**: Looping a fixed number of times.
  ```
  FOR i = 1 TO 10
      PRINT i
  NEXT i
  ```
- **WHILE/WEND**: Executes while a condition is true.
- **DO/LOOP**: Executes indefinitely until a condition is met.
- **SELECT CASE / END SELECT**: Multi-branch conditional.
  ```
  SELECT CASE x
    CASE 1:        PRINT "one"
    CASE 2, 3:     PRINT "two or three"
    CASE ELSE:     PRINT "other"
  END SELECT
  ```

## GOTO and GOSUB

### Labels
A label marks a target line for `GOTO` and `GOSUB`. Labels are identifiers followed by a colon, placed at the start of a line:
```
MyLabel:
    PRINT "arrived here"
```

### GOTO
Unconditionally transfers execution to the specified label.

**Syntax:** `GOTO label`

```
GOTO SkipSection
PRINT "this line is skipped"
SkipSection:
PRINT "execution continues here"
```

### GOSUB / RETURN
`GOSUB` calls a subroutine at the specified label, saving the return address on the call stack. `RETURN` transfers execution back to the statement immediately after the calling `GOSUB`.

**Syntax:**
- `GOSUB label`
- `RETURN`

```
GOSUB DrawBorder
PRINT "border drawn"
END

DrawBorder:
    RECTANGLE 0, 0, 320, 240
    RETURN
```

- Subroutines defined with `GOSUB`/`RETURN` do not accept parameters; use `DEF PROC` or `FUNCTION`/`SUB` for parameterised routines. 
- `GOSUB` calls may be nested; each `RETURN` unwinds one level of the call stack.
- Jumping into a `GOSUB` subroutine body with `GOTO` (bypassing the `GOSUB`) is permitted but the call stack will not contain a return address; a subsequent `RETURN` will cause a runtime error.

## Functions and Subroutines

### DEF FN
User-defined single-expression functions, classic BASIC style:
```
DEF FN square(x) = x * x
PRINT FN square(5)
```

### DEF PROC
Named procedures with parameters, no return value:
```
DEF PROC DrawBox(x, y, w, h)
    RECTANGLE x, y, w, h
END PROC

CALL DrawBox(10, 10, 100, 50)
```

### FUNCTION / END FUNCTION
Named functions with parameters and a return value:
```
FUNCTION Add(a AS INTEGER, b AS INTEGER) AS INTEGER
    Add = a + b
END FUNCTION
```

### SUB / END SUB
Named subroutines (no return value), similar to `DEF PROC` but using structured syntax:
```
SUB Greet(name AS STRING)
    PRINT "Hello, " + name
END SUB
```

## INCLUDE
External listing files can be included at load time using `INCLUDE`. Included files may define:
- `DEF FN` and `DEF PROC` libraries
- `FUNCTION` and `SUB` definitions
- `TRACK` / `NOTE` / `BPM` musical compositions (activated by the main listing with `TRACK PLAY id`)
- `DIM` arrays and `DATA` blocks for HEIGHTFIELD and WORMHOLE maps

**Syntax:** `INCLUDE "filename.bas"`

Restrictions:
- Included files MUST NOT contain `RUN` statements.
- Nested `INCLUDE` (an included file including another file) is NOT permitted.
- Included files are loaded and parsed once at startup before the main listing executes.

**Example — modular program with two display modes:**
```
10 INCLUDE "db_logic.bas"       ' database queries and analysis routines
20 INCLUDE "ui_graphic.bas"     ' graphical display DEF PROCs (or swap with ui_text.bas)
30 INCLUDE "soundtrack.bas"     ' musical track definitions
40 INCLUDE "map_level1.bas"     ' heights() and faceColors() arrays for HEIGHTFIELD

50 GOSUB LoadData
60 TRACK PLAY 1
70 HEIGHTFIELD heights, faceColors, 0, 0, 0, 1, 1, WHITE, 1
```

## Built-in Functions

### String Functions
`LEN()`, `MID()`, `LEFT()`, `RIGHT()`

### Math Functions

#### Standard
`SIN()`, `COS()`, `TAN()`, `RND()`, `ABS()`, `SQR()`, `LOG()`, `EXP()`

#### Precalculated Sine/Cosine Tables
For performance-critical use (oscillators, 3D rotations), two read-only lookup tables cover 36,000 entries at 0.01-degree resolution (0 to 359.99 degrees):

- **`SINF(d)`** — returns the sine of `d` hundredths of a degree (integer 0–35999).
- **`COSF(d)`** — returns the cosine of `d` hundredths of a degree (integer 0–35999).

```basic
10 angle% = 9000          ' 90.00 degrees
20 s# = SINF(angle%)      ' s# = 1.0
30 c# = COSF(angle%)      ' c# = 0.0
```

The tables are loaded into memory at startup and remain resident. Index values outside 0–35999 wrap modulo 36000.

#### Matrices
Mathematical matrix type (not a plain 2D array) with the following operations:

- `MAT DIM m(rows, cols)` — declare a matrix.
- `MAT ADD m1, m2` — element-wise addition (result in m1).
- `MAT MUL m1, m2` — matrix multiplication (result in m1).
- `MAT TRANS m` — transpose m in place.
- `MAT ROT m, axis$, degrees#` — apply a rotation matrix around "X", "Y", or "Z".
- `MAT DET(m)` — returns the determinant (square matrices only).
- `MAT INV m` — invert m in place (square, non-singular matrices only).

```basic
10 MAT DIM a(3,3)
20 MAT DIM b(3,3)
30 MAT MUL a, b
40 PRINT MAT DET(a)
```

#### Calculus on DEF FN
Numerical integration and differentiation are available on user-defined functions declared with `DEF FN`:

- **`INTEGRAL(FN f, a, b, n)`** — approximates the definite integral of `f` from `a` to `b` using `n` steps (trapezoidal rule).
- **`DERIVATIVE(FN f, x)`** — approximates the first derivative of `f` at `x` (central difference).

```basic
10 DEF FN f(x) = x * x
20 PRINT INTEGRAL(FN f, 0, 10, 1000)   ' ≈ 333.33
30 PRINT DERIVATIVE(FN f, 5)            ' ≈ 10.0
```

#### Financial Functions
Standard financial calculations available as built-in functions:

| Function | Description |
|---|---|
| `NPV(rate#, flows())` | Net Present Value of a cash-flow array |
| `IRR(flows())` | Internal Rate of Return |
| `PMT(rate#, nper%, pv#)` | Periodic payment for a loan |
| `FV(rate#, nper%, pmt#, pv#)` | Future value of an investment |
| `PV(rate#, nper%, pmt#, fv#)` | Present value of a cash flow |

```basic
10 DIM flows#(5)
20 flows#(0) = -1000 : flows#(1) = 200 : flows#(2) = 300
30 flows#(3) = 400   : flows#(4) = 500 : flows#(5) = 100
40 PRINT NPV(0.05, flows#())
50 PRINT IRR(flows#())
60 PRINT PMT(0.05/12, 360, 200000)   ' monthly mortgage payment
```

### Type Functions
`TYPEOF()`, `CAST()`

## Data Structures
The following abstract data structures are available as first-class types. All are declared with `DIM ... AS` and operated on with structure-prefixed commands. The full set of operations for each structure is defined in the implementation phase.

| Structure | Declaration example |
|---|---|
| **List** | `DIM myList AS LIST OF INTEGER` |
| **Tree** | `DIM myTree AS TREE OF STRING` |
| **Graph** | `DIM myGraph AS GRAPH` |
| **Stack** | `DIM myStack AS STACK OF FLOAT` |
| **Queue** | `DIM myQueue AS QUEUE OF STRING` |
| **Set** | `DIM mySet AS SET OF INTEGER` |
| **Map** | `DIM myMap AS MAP OF STRING TO INTEGER` |
| **Priority Queue** | `DIM myPQ AS PRIORITY QUEUE OF INTEGER` |
| **Deque** | `DIM myDeque AS DEQUE OF STRING` |

Operations use a structure-name prefix:
```basic
10 DIM myList AS LIST OF INTEGER
20 LIST APPEND myList, 42
30 LIST APPEND myList, 17
40 PRINT LIST SIZE(myList)

50 DIM myMap AS MAP OF STRING TO INTEGER
60 MAP SET myMap, "score", 100
70 PRINT MAP GET(myMap, "score")
```

## Input/Output
- **PRINT**: Displays output to screen.
- **INPUT**: Reads input from user.
- **VOICE commands**: Commands for text-to-speech functionalities.

### File I/O
File access is intentionally restricted for safety:
- System files and hidden/invisible files are not accessible; hidden files are surfaced as visible.
- Executable, batch, and auto-run files cannot be opened or written.
- Maximum individual file size: **200 MB** (e.g. MP4 video).
- Only BASIC commands can be executed; no machine code can be loaded or run.

Standard file commands: `OPEN`, `CLOSE`, `READ`, `WRITE`, `INPUT #`, `PRINT #`, `EOF()`.

### SQLite Database
SQLite databases can be opened and queried directly from BASIC. SQL injection is prevented by mandatory use of **bind parameters** (`?`); string interpolation into SQL statements is not permitted.

```basic
10 DB OPEN "mydata.db" AS #1
20 DB EXEC #1, "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT)"
30 DB EXEC #1, "INSERT INTO users (name) VALUES (?)", userName$
40 DB QUERY #1, rs, "SELECT id, name FROM users WHERE id = ?", userId%
50 WHILE NOT DB EOF(rs)
60   PRINT DB FIELD(rs, "name")
70   DB NEXT rs
80 WEND
90 DB CLOSE #1
```

Key rules:
- All SQL parameters MUST be passed as bind arguments, never concatenated into the query string.
- `DB QUERY` returns a result-set handle (`rs`) iterated with `DB NEXT` / `DB EOF`.
- `DB EXEC` is used for non-SELECT statements (INSERT, UPDATE, DELETE, CREATE, etc.).
- Transactions: `DB BEGIN #n`, `DB COMMIT #n`, `DB ROLLBACK #n`.

## Graphics Commands
- **GRAPHIC**: Initiates graphics mode.
- **LINE** `x1, y1, x2, y2 [, ANGLE=degrees]`: Draws a line between two points. Optional `ANGLE` rotates around its midpoint (default 0).
- **CIRCLE** `x, y, radius [, ANGLE=degrees]`: Draws a circle. Optional `ANGLE` parameter accepted for consistency (default 0).
- **RECTANGLE** `x, y, width, height [, ANGLE=degrees]`: Draws a rectangle. Optional `ANGLE` rotates around its centre (default 0).
- **POLYGON** `[(x1,y1), (x2,y2), ...] [, ANGLE=degrees]`: Draws an arbitrary polygon from a list of 2D vertices (minimum 3). Optional `ANGLE` rotates the polygon around its centroid (default 0).
- **REGULAR_POLYGON** `x, y, sides, radius [, ANGLE=degrees]`: Draws a regular polygon (e.g. triangle, hexagon) centred at `(x, y)` with the given number of sides (integer >= 3) and radius. Optional `ANGLE` rotates around the centre (default 0).
- **SPHERE, BOX, TORUS, WORMHOLE**: 3D shapes rendering commands.
- **HEIGHTFIELD** `heights, faceColors, x, y, z, dx, dy, lineColor, lineThickness`: Renders a heightfield surface (terrain) defined over the X-Y plane with elevation along Z.
  - `heights`: `X x Y` matrix of height samples (`X >= 2`, `Y >= 2`); `heights[x][y]` is the height at grid coordinate `(x, y)`.
  - `faceColors`: `(X-1) x (Y-1)` matrix of RGBA colors, one per quad cell.
  - `(x, y, z)`: world-space origin for sample `(0, 0)`.
  - `dx`, `dy`: spacing between adjacent samples in world units along X and Y.
  - `lineColor`, `lineThickness`: grid edge style used in Wireframe and FlatWire modes; ignored in Flat mode.

### REGULAR_POLYGON Example
```
' Draw a regular hexagon centred at (320, 240), radius 50, rotated 30 degrees
REGULAR_POLYGON 320, 240, 6, 50, ANGLE=30
```

## 3D Primitives
- Use to define basic 3D shapes and objects in the environment.

## Camera Setup
- Setup view with **FOV** (Field of View) settings.

## Sprite Commands
- **SPRITE_ANGLE** `sprite, angle`: Sets the rotation angle (in degrees) of a sprite. Rotation is applied around the sprite's anchor point.
- **SPRITE_ANCHOR** `sprite, x_offset, y_offset`: Sets the pivot/anchor point for sprite rotation as a relative offset (0.0-1.0). Default is `(0.5, 0.5)` (centre of the sprite).

### Sprite Rotation Example
```
' Rotate a sprite 45 degrees around its centre
SPRITE_ANCHOR player_sprite, 0.5, 0.5
SPRITE_ANGLE player_sprite, 45
```

## Sound Commands
- **SOUND**: Initiates sound.
- **NOTE**: Specifies musical notes.
- **REST**: Indicates periods of silence.
- **BPM**: Beats per minute control.
- **TUNING**: Set the instrument tuning.
- **TRACK** operations manage musical compositions.
- **TRACK PLAY id**: Activates a track defined in an included file (see `INCLUDE`).

### Text-to-Speech (TTS)

The system provides offline Text-to-Speech using the **Piper** engine only. No cloud connection is required. Language availability depends on installed Piper voice packs.

TTS is designed with:
- **Engine:** Piper (offline, high-quality neural voices).
- **No cloud requirement.**
- **No fallback engine.** If no Piper voice is available for the requested language, the system uses the configured default voice.

#### Queue semantics (normative)

TTS output is **queued**.

- Every call to `TTS` creates one *utterance* and pushes it to a **FIFO queue**.
- The system MUST play **only one utterance at a time** (**no overlap**).
- If `TTS` is called while speech is already playing, the new utterance MUST be **queued**, and MUST NOT implicitly interrupt the current utterance (**no implicit suppression**).

Use `SAYSTOP` / `SAYFLUSH` to explicitly control playback and the queue.

#### TTS `text$ [, lang$ [, mode$]]`

Speaks the given text.

- `text$`: The text to speak (UTF-8).
- `lang$` (optional): BCP-47 language tag or short language code.
  - Examples: "it-IT", "en-US", "it", "en".
  - If omitted, the current default Piper voice/language is used.
  - If no Piper voice is available for the requested language, the default voice is used.
- `mode$` (optional): Controls pronunciation strategy.
  - "TEXT" (default): Natural reading for human text.
  - "CODE": Intended for reading BASIC listings and technical strings.

#### Speech control commands

##### SAYSTOP
Stops speech immediately and clears the queue.

- Stops the current utterance **immediately**
- Clears any queued utterances

##### SAYFLUSH
Flushes pending speech without interrupting the current utterance.

- Does **not** interrupt the current utterance
- Clears any queued utterances (so nothing will be spoken after the current finishes)

##### SAYSTATUS `()` -> status
Returns the current speech status:
- `0` = idle (not speaking, queue empty)
- `1` = speaking
- `2` = paused (only if pause/resume is implemented)

##### SAYQUEUE `()` -> n
Returns the number of queued utterances **excluding** the one currently playing.

#### Examples (always line-numbered)
**Queueing (FIFO, no overlap)**
```basic
10 TTS "Loading..."
20 TTS "Graphics ready."
30 TTS "Network ready."
40 ' Spoken in order. No overlap. No implicit interruption.
```

**SAYFLUSH (drop backlog, keep current)**
```basic
10 TTS "One"
20 TTS "Two"
30 TTS "Three"
40 SAYFLUSH
50 ' Finishes current utterance, then stops.
```

**SAYSTOP (immediate stop + clear queue)**
```basic
10 TTS "This is a long sentence that might take a while."
20 TTS "This will be queued unless we stop."
30 SAYSTOP
40 ' Speech stops immediately and the queue is cleared.
```

## Security

RetroWave-4M is single-tasking by design (with limited multitasking restricted to sound and network only). This architecture provides a reduced attack surface. Security principles:

- **No machine code execution.** Only BASIC statements can run; no native code, shell commands, or binary loaders are permitted.
- **Restricted file I/O.** See File I/O section for access restrictions (no system files, no executables, no hidden files, 200 MB limit).
- **SQL injection prevention.** All SQLite queries MUST use bind parameters. String interpolation into SQL is not permitted.
- **Static analysis at load time.** Listings flagged as potentially dangerous (e.g. abnormal PEEK/POKE patterns, suspicious string constructions) are analysed before execution. The user is warned and may cancel.
- **INCLUDE sandboxing.** Included files may only define functions, procedures, data, and track/map resources. They cannot execute statements directly or include further files.

## Example Program
``` BASIC
DIM people(10) AS Person

FOR i = 1 TO 10
    people(i).Name = INPUT("Enter name:")
    people(i).Age = INPUT("Enter age:")
NEXT i

FOR i = 1 TO 10
    PRINT "Name: " + people(i).Name + ", Age: " + people(i).Age
NEXT i
```
---

## Cross-References

- [DATA_STRUCTURES.md](./DATA_STRUCTURES.md) -- data types, arrays, records, first-class data
  structures (List, Tree, Graph, Stack, Queue, Set, Map, Priority Queue, Deque), SQLite.
- [GRAPHICS.md](./GRAPHICS.md) -- full graphics specification (display modes, 3D, 2D, sprites).
- [SOUND.md](./SOUND.md) -- TTS engine, queue semantics, synthesis, SoundFont instruments.
- [ACCESSIBILITY.md](./ACCESSIBILITY.md) -- VOICE ON/OFF, headless mode, F9/F10 shortcuts.
- [ARCHITECTURE.md](./ARCHITECTURE.md) -- security model, storage sandbox, monotasking design.
