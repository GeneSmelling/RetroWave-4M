# Graphics System Specification -- RetroWave 4M

> This file contains the detailed graphics architecture.
> For the system-wide architecture see [ARCHITECTURE.md](./ARCHITECTURE.md).

---

## Display Modes and Resolutions

| Mode | Resolution | Pixel mapping | Notes |
|------|-----------|---------------|-------|
| Full HD | 1920 x 1080 | 1 logical pixel = 1 physical pixel | Native resolution |
| Half HD | 960 x 540 | 1 logical pixel = 2x2 physical pixels | Integer-scaled |
| Quarter HD | 640 x 360 | 1 logical pixel = 3x3 physical pixels | Integer-scaled |

All lower resolutions use integer pixel-scaling: logical pixels are scaled up to exact
whole-number multiples of the physical pixel grid, producing sharp, artefact-free rendering
consistent with the retro aesthetic.

**Logical coordinate space** is always **X: 0-1919, Y: 0-1079** regardless of the active mode.
Coordinates outside this range are clipped. Visuals keep their proportions across modes.

Switch to graphics mode with the `GRAPHIC` command. Return to text mode with `TEXT`.

---

## Colour Model

- **RGBA** -- Red, Green, Blue, Alpha (transparency).
- **32-bit colour:** 8 bits per channel; over 16 million colours with a full alpha channel.
- Colours are specified as RGBA tuples, e.g. `(255, 128, 0, 255)` for opaque orange.

---

## 5-Layer Rendering Stack

Layers are composited front-to-back. Layer 1 is closest to the viewer.

| Layer | Name | Contents |
|-------|------|----------|
| 1 | **Text** | Unicode character grid (48x27 cells at 40x40 px); optional TrueType overlay |
| 2 | **2D Graphics** | Primitives, images, video frames, UI widgets |
| 3 | **2D Sprites** | Animated sprite sheets; sublayer ordering; collision detection |
| 4 | **3D** | Wire / flat / flatwire 3D scene; FOV 90 degrees default |
| 5 | **Background** | Solid colour, gradient, scrolling image, or tile map |

**Double buffering:** a back buffer is rendered off-screen and swapped to the front buffer at each
frame, eliminating flicker.

---

## 3D Rendering Modes

All 3D geometry is rendered using one of three modes, selected per scene or per object:

| Mode | Description |
|------|-------------|
| **Wireframe** | Edges only; single RGBA edge colour |
| **Flat** | Filled faces with no shading; colour defined per face (RGBA matrix) |
| **FlatWire** | Filled faces + wireframe edges; both face colours and edge colour apply |

### Camera and Field of View

- **FOV (Field of View):** horizontal angle in degrees. Default: **90 degrees**.
- Can be set per camera or per scene.
- Two camera types: Static (fixed position) and Dynamic (follows a target).

---

## 3D Primitives

### Basic Shapes

- **Cuboid (BOX):** width, height, depth.
- **Sphere:** centre point, radius, segment count.
- **Cylinder:** centre, radius, height, segment count.

### TORUS

A torus (donut shape) defined by:

- `majorRadius` -- distance from the centre of the tube to the centre of the torus.
- `minorRadius` -- radius of the tube itself.
- `majorSegments` -- number of segments around the main ring (integer, minimum 3).
- `minorSegments` -- number of segments around the tube cross-section (integer, minimum 3).

Coloring per rendering mode:

| Mode | Colour definition |
|------|------------------|
| Wireframe | Single RGBA edge colour |
| Flat | Face colour matrix of `majorSegments x minorSegments` RGBA values |
| FlatWire | Edge RGBA colour + face colour matrix |

### WORMHOLE / TUNNEL

A tunnel/pipe shape defined by:

- **Cross-section polygon:** arbitrary polygon (minimum 3 vertices) given as a list of 2D points
  `[(x1,y1), (x2,y2), ...]` defining the tunnel's cross-section shape.
- **Path spline:** a Catmull-Rom or Bezier spline defining the 3D centreline path.
- **Subdivisions:** integer number of sections along the spline (minimum 1); higher values
  produce smoother curves.

Coloring per rendering mode:

| Mode | Colour definition |
|------|------------------|
| Wireframe | Single RGBA edge colour |
| Flat | Face colour matrix of size `(polygon vertices) x (subdivisions - 1)` RGBA values |
| FlatWire | Edge RGBA colour + face colour matrix |

### POLYHEDRON

An arbitrary polyhedron defined by:

- **Vertices:** array of 3D points `(x, y, z)` (floating-point). Minimum 4 vertices.
- **Faces:** array of faces; each face is a list of vertex indices (integers, 0-based, minimum 3
  per face). Faces must be planar.

Coloring per rendering mode:

| Mode | Colour definition |
|------|------------------|
| Wireframe | Single RGBA edge colour |
| Flat | One RGBA colour per face (array of N RGBA values, N = number of faces) |
| FlatWire | Edge RGBA colour + per-face colour array |

### Regular Polyhedra (Platonic Solids)

The five Platonic solids are supported as built-in primitives. Each is defined by:

- Centre point `(x, y, z)`.
- Radius (circumradius: distance from centre to vertex).
- Optional orientation: rotation angles `rotX`, `rotY`, `rotZ` in degrees (all default 0).

| Solid | Faces | Face shape | BASIC command |
|-------|-------|-----------|---------------|
| Tetrahedron | 4 | Equilateral triangle | `TETRAHEDRON` |
| Cube (Hexahedron) | 6 | Square | `BOX` |
| Octahedron | 8 | Equilateral triangle | `OCTAHEDRON` |
| Dodecahedron | 12 | Regular pentagon | `DODECAHEDRON` |
| Icosahedron | 20 | Equilateral triangle | `ICOSAHEDRON` |

Coloring follows the same Wireframe / Flat / FlatWire rules as POLYHEDRON.

---

## Heightfield Rendering

A heightfield is a surface defined by a 2D grid of height samples laid over the X-Y plane, with
elevation applied along the Z axis.

### Height Data

- Heights provided as an `X x Y` matrix `H` (`X >= 2`, `Y >= 2`), where `H[x][y]` is the
  Z elevation at grid coordinate `(x, y)`.
- Grid coordinates run from `(0, 0)` to `(X-1, Y-1)`.

### Face Colours

- Face colours defined by a `(X-1) x (Y-1)` matrix `FC`, where `FC[x][y]` is the RGBA colour
  of the quad cell bounded by samples `(x, y)`, `(x+1, y)`, `(x, y+1)`, and `(x+1, y+1)`.

### Rendering Modes

| Mode | Description |
|------|-------------|
| **Wireframe** | Grid edges only; single RGBA `lineColor`; integer `lineThickness` (>= 1 logical pixel) |
| **Flat** | Filled quad faces using the face-colour matrix; no edges drawn |
| **FlatWire** | Filled faces using face-colour matrix + grid edges using `lineColor` / `lineThickness` |

### Geometry Notes

- Adjacent samples form quad cells; the renderer may internally triangulate each quad for
  rasterisation.
- World placement (origin) and grid spacing (distance between adjacent samples in world units)
  are specified by the `HEIGHTFIELD` command.

**BASIC command:**

```
HEIGHTFIELD heights, faceColors, x, y, z, dx, dy, lineColor, lineThickness
```

---

## 2D Sprites

- Animated sprite sheets for characters and objects.
- **Sublayer ordering:** sprites can be assigned to sublayers within layer 3.
- **Collision detection:** bounding-box and pixel-perfect collision checks.
- **Rotation:** sprites may be rotated by a specified angle in degrees around a configurable
  anchor point.
- **Anchor point:** relative offset from the sprite's top-left corner (0.0--1.0 per axis).
  Default: `(0.5, 0.5)` (centre).

**BASIC commands:**

```
SPRITE_ANCHOR sprite, x_offset, y_offset   ' e.g. SPRITE_ANCHOR player, 0.5, 0.5
SPRITE_ANGLE  sprite, angle                ' e.g. SPRITE_ANGLE  player, 45
```

---

## 2D Graphics Primitives

All 2D primitives support an optional **rotation angle** (degrees, default 0), applied around
the primitive's centre point.

| Primitive | Definition |
|-----------|-----------|
| **Rectangle** | Position, width, height; axis-aligned or rotated |
| **Circle** | Centre point, radius |
| **Polygon** | List of 2D vertices `[(x1,y1), ...]` (minimum 3) |
| **Regular Polygon** | Centre `(x, y)`, number of sides (>= 3), radius, optional rotation |

**BASIC commands:**

```
LINE       x1, y1, x2, y2 [, ANGLE=degrees]
RECTANGLE  x, y, width, height [, ANGLE=degrees]
CIRCLE     x, y, radius
POLYGON    [(x1,y1), (x2,y2), ...] [, ANGLE=degrees]
REGULAR_POLYGON  x, y, sides, radius [, ANGLE=degrees]
```

Example -- regular hexagon:

```basic
' Hexagon centred at (320, 240), radius 50, rotated 30 degrees
REGULAR_POLYGON 320, 240, 6, 50, ANGLE=30
```

---

## Text Layer

- Dynamic Unicode text rendered in the text layer (layer 1).
- Character cells: 40 x 40 px; grid 48 x 27.
- Optional TrueType font overlay for proportional or decorative text in graphics mode.

---

## Media Formats

| Type | Formats |
|------|---------|
| Still image | JPEG, PNG, BMP |
| Video | MP4 (H.264) |
| Audio | WAV, MP3, OGG |

Images and video frames can be displayed on layer 2 (2D graphics). Video playback is
single-channel (monotasking constraint: only audio plays in background).

---

## Widget System

- Interactive UI elements rendered on top of the 5-layer stack.
- Available in both graphics windows and text-mode windows.
- Widget types: buttons, labels, text input fields, checkboxes, sliders, dropdown menus,
  scrollable panels.
- Full keyboard navigation (per accessibility goals); no mouse required.
- Visibility and enabled/disabled state are manageable at runtime.

---

## Lazy Loading

- Graphics assets (sprites, textures, heightmaps, 3D meshes) are loaded on demand when first
  accessed, not at startup.
- A placeholder (blank sprite or colour fill) is shown while the asset loads.
- Loaded assets are cached in memory and not reloaded unless explicitly flushed.
- Assets may be pre-loaded with `PRELOAD` for performance-critical scenes.
- Under memory pressure, least-recently-used cached assets are evicted.

---

## Retrowave Aesthetic

- Neon RGBA colour palettes.
- High-contrast, pixel-sharp character cells.
- Integer pixel-scaling for artefact-free retro look.
- Synthwave visual motifs available through the Synthwave preset bank (see [SOUND.md](./SOUND.md)).
