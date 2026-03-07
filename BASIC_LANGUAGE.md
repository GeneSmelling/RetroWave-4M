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