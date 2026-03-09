# BASIC Language Specification

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

- Subroutines defined with `GOSUB`/`RETURN` do not accept parameters; use `FUNCTION` or `SUB` for parameterised routines.
- `GOSUB` calls may be nested; each `RETURN` unwinds one level of the call stack.
- Jumping into a `GOSUB` subroutine body with `GOTO` (bypassing the `GOSUB`) is permitted but the call stack will not contain a return address; a subsequent `RETURN` will cause a runtime error.

## Functions and Subroutines
- Define operations using `FUNCTION` and `SUB`. Supports parameters and return types.
- Example:
  ```
  FUNCTION Add(a AS INTEGER, b AS INTEGER) AS INTEGER
      Add = a + b
  END FUNCTION
  ```

## Built-in Functions
- **String Functions**: `LEN()`, `MID()`, `LEFT()`, `RIGHT()`. 
- **Math Functions**: `SIN()`, `COS()`, `TAN()`, `RND()`, `ABS()`. 
- **Type Functions**: `TYPEOF()`, `CAST()`. 

## Input/Output
- **PRINT**: Displays output to screen.
- **INPUT**: Reads input from user.
- **VOICE commands**: Commands for text-to-speech functionalities.

## Graphics Commands
- **GRAPHIC**: Initiates graphics mode.
- **LINE** `x1, y1, x2, y2 [, ANGLE=degrees]`: Draws a line between two points. Optional `ANGLE` rotates around its midpoint (default 0).
- **CIRCLE** `x, y, radius [, ANGLE=degrees]`: Draws a circle. Optional `ANGLE` parameter accepted for consistency (default 0).
- **RECTANGLE** `x, y, width, height [, ANGLE=degrees]`: Draws a rectangle. Optional `ANGLE` rotates around its centre (default 0).
- **POLYGON** `[(x1,y1), (x2,y2), ...] [, ANGLE=degrees]`: Draws an arbitrary polygon from a list of 2D vertices (minimum 3). Optional `ANGLE` rotates the polygon around its centroid (default 0).
- **REGULAR_POLYGON** `x, y, sides, radius [, ANGLE=degrees]`: Draws a regular polygon (e.g. triangle, hexagon) centred at `(x, y)` with the given number of sides (integer ≥ 3) and radius. Optional `ANGLE` rotates around the centre (default 0).
- **SPHERE, BOX, TORUS, WORMHOLE**: 3D shapes rendering commands.
- **HEIGHTFIELD** `heights, faceColors, x, y, z, dx, dy, lineColor, lineThickness`: Renders a heightfield surface (terrain) defined over the X–Y plane with elevation along Z.
  - `heights`: `X × Y` matrix of height samples (`X >= 2`, `Y >= 2`); `heights[x][y]` is the height at grid coordinate `(x, y)`.
  - `faceColors`: `(X-1) × (Y-1)` matrix of RGBA colors, one per quad cell; `faceColors[x][y]` colors the quad bounded by samples `(x, y)`, `(x+1, y)`, `(x, y+1)`, `(x+1, y+1)`.
  - `(x, y, z)`: world-space origin for sample `(0, 0)`.
  - `dx`, `dy`: spacing between adjacent samples in world units along X and Y.
  - `lineColor`, `lineThickness`: grid edge style used in Wireframe and FlatWire modes; ignored in Flat mode. All grid edges are drawn between adjacent samples in both X and Y directions.

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
- **SPRITE_ANCHOR** `sprite, x_offset, y_offset`: Sets the pivot/anchor point for sprite rotation as a relative offset (0.0–1.0). Default is `(0.5, 0.5)` (centre of the sprite).

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

### Text-to-Speech (TTS)

The system provides offline Text-to-Speech suitable for a real "home computer" workflow (including headless use on Raspberry Pi 400).

TTS is designed with:
- **Primary engine:** Piper (higher-quality voices).
- **Fallback engine:** eSpeak NG (broad language coverage).
- **No cloud requirement.**

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
  - Examples: "it-IT", "en-US", "zh-CN", "it", "en".
  - If omitted, the current default voice/language is used.
- `mode$` (optional): Controls pronunciation strategy.
  - "TEXT" (default): Natural reading for human text.
  - "CODE": Intended for reading BASIC listings and technical strings (symbols and tokens are read more explicitly; may use a different internal reading strategy and/or engine).

**Behavior**
- If a Piper voice matching `lang$` is available, it is used.
- If not, the system falls back to eSpeak NG for that language.
- If the requested language is not available, a final fallback voice is used (implementation-defined; typically the default language).

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
50 ' If already speaking, it will finish the current utterance, then the rest is dropped.
```

**SAYSTOP (immediate stop + clear queue)**
```basic
10 TTS "This is a long sentence that might take a while."
20 TTS "This will be queued unless we stop."
30 SAYSTOP
40 ' Speech stops immediately and the queue is cleared.
```

> Note: TTS runs offline. Language availability depends on installed voice packs.

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
