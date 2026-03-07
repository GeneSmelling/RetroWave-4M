# Graphics System Specification

## Resolutions
- `1920×1080` — Full HD, native physical resolution
- `960×540` — Logical mode; each logical pixel = 2×2 physical pixels
- `640×360` — Logical mode; each logical pixel = 3×3 physical pixels

All three resolutions are part of a deliberate integer pixel-scaling system anchored to the 1920×1080 physical canvas.

## Color Model
- **RGBA** (Red, Green, Blue, Alpha) model used for defining colors.
- **32-bit color depth** — 8 bits per channel (R, G, B, A), ensuring vibrant visuals with full transparency support.

## 3D Rendering Modes
1. **Wireframe**  – Basic rendering mode for visualizing 3D structures through wireframes, useful for debugging.
2. **Flat**  – Simplistic shading where surfaces are colored uniformly, reflecting the retro aesthetic.
3. **FlatWire**  – Combines wireframe outlines with solid surfaces for a unique visual effect.

## Camera System
- **Static Camera**   – Fixed position for classic retro gameplay.
- **Dynamic Camera**  – Follows the player, providing a more immersive experience.
- **Field of View (FOV)** – Defines the camera's horizontal angle of view in degrees. Default value is **90 degrees** if not explicitly set. Can be configured per camera or per scene. Applies to both Static and Dynamic camera types.

## 3D Primitives
- **Cuboids**, **Spheres**, **Cylinders** – Basic geometric shapes for building worlds and objects.

### TORUS
A toroidal (donut) shape defined by the following parameters:
- `majorRadius` — distance from the centre of the tube to the centre of the torus.
- `minorRadius` — radius of the tube itself.
- `majorSegments` — integer number of segments around the main ring (minimum 3). Higher values produce a smoother approximation.
- `minorSegments` — integer number of segments around the tube cross-section (minimum 3). Higher values produce a smoother approximation.

Coloring per rendering mode:
- **Wireframe**: edge color defined by a single RGBA value.
- **Flat**: face color defined by a `majorSegments × minorSegments` matrix of RGBA values.
- **FlatWire**: both edge color (single RGBA) and face color matrix (`majorSegments × minorSegments` RGBA values) apply.

### WORMHOLE-TUNNEL
A tunnel/pipe shape defined by:
- **Cross-section polygon** — an arbitrary polygon (minimum 3 vertices) that defines the shape of the tunnel's cross-section. Specified as a list of 2D vertices `[(x1,y1), (x2,y2), ...]`.
- **Path spline** — a spline (e.g. Catmull-Rom or Bézier) that defines the 3D path/centreline the tunnel follows through space.
- **Subdivisions** — integer number of sections into which the tunnel is divided along the spline path (minimum 1). Higher values produce a smoother curve-following tunnel.

Coloring per rendering mode:
- **Wireframe**: edge color defined by a single RGBA value.
- **Flat**: face color defined by a matrix of RGBA values. Dimensions: **rows = number of polygon vertices**, **columns = subdivisions − 1** (each column covers one segment between adjacent cross-sections, so N subdivisions yield N−1 inter-section faces; one RGBA per face).
- **FlatWire**: both edge color (single RGBA) and face color matrix apply.

## Heightfield Rendering
- Supports rendering terrains based on heightmaps to create varied landscapes.

## 2D Sprites
- Animated sprites for characters and objects, implemented for both movement and interactions.

## 2D Graphics Primitives
- Supports shapes like rectangles, circles, and polygons for UI elements.

## Text Layer
- Dynamic text rendering for displaying scores, messages, and other information in the game.

## Double Buffering
- Using double buffering to prevent flickering during rendering, enhancing visual quality.

## Retrowave Aesthetic Integration
- Includes neon color palettes, synthwave motifs, and retro futurism design elements to achieve an immersive retro vibe.

## Widget System
Widgets are interactive UI elements rendered on top of the 5-layer graphics stack (above the Text layer).

- Supported in both **graphics windows** (rendered via Pygame surface overlay) and **text-mode windows** (rendered as text-based UI elements).
- Supported widget types:
  - **Button** — clickable control that triggers an action.
  - **Label** — static or dynamic text display.
  - **Text Input Field** — single-line or multi-line editable text entry.
  - **Checkbox** — boolean toggle control.
  - **Slider** — numeric value selection along a range.
  - **Dropdown Menu** — single-selection list that expands on interaction.
  - **Scrollable Panel** — container with scrollable content area.
- Widgets support **keyboard navigation** (consistent with the project's accessibility goals).
- Widget state (visible/hidden, enabled/disabled) is manageable at runtime.

## Lazy Loading
Graphics assets (sprites, textures, heightmaps, 3D meshes) are loaded on demand rather than at application startup.

- When an asset is first accessed, a **loading placeholder** (e.g. a blank sprite or a colour fill) is displayed while the asset loads asynchronously.
- Once loaded, assets are **cached in memory** and are not reloaded unless explicitly flushed.
- Assets can be **preloaded explicitly** with a `PRELOAD` command for performance-critical scenes.
- Under memory pressure, the least-recently-used cached assets are automatically evicted.