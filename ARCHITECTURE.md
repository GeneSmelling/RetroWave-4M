# Detailed Graphics Architecture

## Resolutions
- Standard resolution: 1920x1080 (Full HD)
- Support for lower resolutions: 1280x720 (HD)

## Color Model
- Used color model: RGBA (Red, Green, Blue, Alpha)
- Bit depth: 8 bits per channel

## Primitives
- Supported graphics primitives:
  - Points
  - Lines
  - Triangles
  - Quads

## Double Buffering
- Description: Implemented to reduce flickering and provide smoother visuals.
- Buffers in use:
  - Front buffer
  - Back buffer

## 5-layer System
1. **Background**: Static and animated background layers.
2. **3D Graphics**: Rendered using a dedicated rendering engine.
3. **2D Sprites**: Animated sprite handling for characters and objects.
4. **2D Graphics**: Static and dynamic graphical elements.
5. **Text**: Rendering of UI elements and in-game text.

---

This detailed architecture outlines the graphics system for RetroWave-4M, providing a comprehensive overview of how graphics are rendered and managed within the engine.