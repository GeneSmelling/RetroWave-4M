# Graphics System Specification

## Resolutions
- `1920x1080` – Full HD, native physical resolution
- `960x540` – Logical mode: each logical pixel maps to a 2×2 block of physical pixels
- `640x360` – Logical mode: each logical pixel maps to a 3×3 block of physical pixels

All lower resolutions use integer pixel-scaling: logical pixels are scaled up to exact whole-number multiples of the physical pixel grid, ensuring sharp, artefact-free rendering consistent with the retro aesthetic.

## Color Model
- **RGBA** (Red, Green, Blue, Alpha) model used for defining colors, ensuring vibrant visuals with full transparency support.
- **32-bit color (8 bits per channel)** – 8 bits each for R, G, B, and A, providing over 16 million colors with a full alpha channel.

## 3D Rendering Modes
1. **Wireframe**  – Basic rendering mode for visualizing 3D structures through wireframes, useful for debugging.
2. **Flat**  – Simplistic shading where surfaces are colored uniformly, reflecting the retro aesthetic.
3. **FlatWire**  – Combines wireframe outlines with solid surfaces for a unique visual effect.

## Camera System
- **Static Camera**   – Fixed position for classic retro gameplay.
- **Dynamic Camera**  – Follows the player, providing a more immersive experience.

### Field of View (FOV)
- Defines the camera's horizontal angle of view in degrees.
- **Default: 90 degrees** if not explicitly specified.
- Applies to both Static and Dynamic camera types.
- Can be set per camera or per scene.

## 3D Primitives
- **Cuboids**, **Spheres**, **Cylinders** – Basic geometric shapes for building worlds and objects.

### TORUS
A torus (donut shape) defined by:
- `majorRadius` – distance from the centre of the tube to the centre of the torus
- `minorRadius` – radius of the tube itself
- `majorSegments` – number of segments around the main ring (integer, minimum 3); controls approximation quality
- `minorSegments` – number of segments around the tube cross-section (integer, minimum 3); controls approximation quality

Coloring per rendering mode:
- **Wireframe**: single RGBA edge color
- **Flat**: face color matrix of `majorSegments x minorSegments` RGBA values
- **FlatWire**: both edge RGBA color and face color matrix apply

### WORMHOLE-TUNNEL
A tunnel/pipe shape defined by:
- **Cross-section polygon**: arbitrary polygon (minimum 3 vertices) given as a list of 2D points `[(x1,y1), (x2,y2), ...]` defining the tunnel's cross-section shape
- **Path spline**: a spline (Catmull-Rom or Bezier) defining the 3D centreline path the tunnel follows
- **Subdivisions**: integer number of sections dividing the tunnel along the spline (minimum 1); higher values produce smoother curves

Coloring per rendering mode:
- **Wireframe**: single RGBA edge color
- **FlatWire**: edge RGBA color and face color matrix
- **Flat**: face color matrix of size `(number of polygon vertices) x (subdivisions - 1)` RGBA values — rows = polygon vertices, columns = subdivisions - 1

### POLYHEDRON
An arbitrary polyhedron defined by:
- **Vertices**: an array of 3D points, each a record with fields `x`, `y`, `z` (floating-point). Minimum 4 vertices.
- **Faces**: an array of faces, where each face is a list of vertex indices (integers, 0-based) referencing the vertices array. Each face must have a minimum of 3 indices. Faces must be planar.

Coloring per rendering mode:
- **Wireframe**: single RGBA edge color
- **Flat**: one RGBA color per face (array of N RGBA values, where N = number of faces)
- **FlatWire**: both edge RGBA color and per-face color array apply

### Regular Polyhedra (Platonic Solids)
The five Platonic solids are supported as built-in primitives. Each is defined by:
- Centre point `(x, y, z)`
- Radius — the circumradius (distance from centre to vertex)
- Optional orientation — three rotation angles in degrees: `rotX`, `rotY`, `rotZ` (all default 0)

| Solid | Faces | Face shape | Command |
|---|---|---|---|
| Tetrahedron | 4 | Equilateral triangle | `TETRAHEDRON` |
| Cube (Hexahedron) | 6 | Square | `BOX` (existing) |
| Octahedron | 8 | Equilateral triangle | `OCTAHEDRON` |
| Dodecahedron | 12 | Regular pentagon | `DODECAHEDRON` |
| Icosahedron | 20 | Equilateral triangle | `ICOSAHEDRON` |

Coloring per rendering mode (applies to all Platonic solid commands):
- **Wireframe**: single RGBA edge color
- **Flat**: one RGBA color per face (array of N RGBA values matching the face count of the solid)
- **FlatWire**: both edge RGBA color and per-face color array apply

## Heightfield Rendering

A heightfield is a surface defined by a 2D grid of height samples laid over the **X–Y plane**, with elevation applied along the **Z axis**.

### Height Data

- Heights are provided as an `X × Y` matrix `H` (`X >= 2`, `Y >= 2`), where `H[x][y]` is the height (Z elevation) at grid coordinate `(x, y)`.
- Grid coordinates run from `(0, 0)` to `(X-1, Y-1)`.

### Face Colors

- Face colors are defined by a `(X-1) × (Y-1)` matrix `FC`, where `FC[x][y]` is the RGBA color of the quad cell bounded by samples `(x, y)`, `(x+1, y)`, `(x, y+1)`, and `(x+1, y+1)`.

### Rendering Modes

| Mode | Description |
|---|---|
| **Wireframe** | Draws all grid edges (every segment between adjacent samples in both X and Y directions) using a single RGBA `lineColor` and integer `lineThickness` (logical pixels, >= 1). No filled faces. |
| **Flat** | Draws filled quad faces using the `(X-1) × (Y-1)` face-color matrix `FC`. No edges drawn. |
| **FlatWire** | Draws filled quad faces using the face-color matrix `FC` and also draws all grid edges using the specified `lineColor` and `lineThickness`. |

### Geometry Notes

- Adjacent samples form quad cells; the renderer may internally triangulate each quad (e.g. split along a diagonal) for rasterization.
- World placement (origin/translation) and grid spacing (distance between adjacent samples in world units) are specified by the command or API call that renders the heightfield.

## 2D Sprites
- Animated sprites for characters and objects, implemented for both movement and interactions.
- Sprites support **rotation** by a specified angle in degrees.
- Rotation is applied around a configurable **pivot/anchor point** (default: centre of the sprite).
- The anchor point is expressed as a relative offset from the sprite's top-left corner, e.g. `(0.5, 0.5)` for centre, `(0.0, 0.0)` for top-left.

## 2D Graphics Primitives
Supported 2D primitives:
- **Rectangle** – Axis-aligned or rotated rectangle defined by position, width, and height.
- **Circle** – Defined by centre point and radius.
- **Polygon** – Arbitrary polygon defined by a list of 2D vertices `[(x1,y1), (x2,y2), ...]` (minimum 3 vertices).
- **Regular Polygon** – A perfectly regular polygon (e.g. equilateral triangle, square, pentagon, hexagon) defined by:
  - Centre point `(x, y)`
  - Number of sides (integer, minimum 3)
  - Radius (distance from centre to vertex)
  - Optional rotation angle in degrees (default: 0)

All 2D primitives support an optional **rotation angle** (in degrees, default 0), applied around the primitive's centre point.

## Text Layer
- Dynamic text rendering for displaying scores, messages, and other information in the game.

## Double Buffering
- Using double buffering to prevent flickering during rendering, enhancing visual quality.

## Retrowave Aesthetic Integration
- Includes neon color palettes, synthwave motifs, and retro futurism design elements to achieve an immersive retro vibe.

## Widget System
- Widgets are interactive UI elements rendered on top of the 5-layer graphics stack.
- Available in both **graphics windows** and **text-mode windows**.
- Widget types include: buttons, labels, text input fields, checkboxes, sliders, dropdown menus, scrollable panels.
- Widgets support keyboard navigation (per accessibility goals).
- Widget visibility and enabled/disabled state are manageable at runtime.

## Lazy Loading
- Graphics assets (sprites, textures, heightmaps, 3D meshes) are loaded on demand when first accessed, not at startup.
- A placeholder (blank sprite or colour fill) is shown while the asset loads asynchronously.
- Loaded assets are cached in memory and not reloaded unless explicitly flushed.
- Assets may be preloaded explicitly using a `PRELOAD` command for performance-critical scenes.
- Memory pressure triggers eviction of least-recently-used cached assets.
