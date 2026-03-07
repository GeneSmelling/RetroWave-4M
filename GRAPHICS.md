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

## Heightfield Rendering
- Supports rendering terrains based on heightmaps to create varied landscapes.

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