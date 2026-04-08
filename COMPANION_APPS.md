# Companion Applications -- RetroWave 4M

Companion applications are standalone programs bundled with RetroWave 4M. They are NOT loaded
as resident utilities (function-key utilities) to avoid excessive memory usage. Launch them from
the BASIC `RUN` command or from the F3 File Manager.

> For function-key utilities (F1--F12), see [UTILITIES.md](./UTILITIES.md).

---

## How to Launch a Companion App

From the BASIC prompt:

```basic
RUN "apps/texteditor/main.bas"
RUN "apps/spreadsheet/main.bas"
RUN "apps/browser/main.bas"
```

Or press `F3` (File Manager), navigate to the `apps/` directory, and select an app.

---

## Application Catalogue

### Text Editor

A standalone accessible text editor for plain text and BASIC source files.

- Fully keyboard-navigable; TTS-compatible.
- Opens any plain-text file from the user data directory.
- Syntax highlighting for BASIC source (optional).
- Save, save-as, new file operations.
- Accessible find/replace.
- **Gap-analysis recommended:** high-priority inclusion for the 1.0 release.

---

### Spreadsheet

A lightweight spreadsheet for budgeting, data management, and small datasets.

- CSV import and export.
- Cell formulas: arithmetic, basic functions (SUM, AVERAGE, MIN, MAX, COUNT, IF, VLOOKUP).
- Column sorting (ascending/descending).
- Basic charting: bar chart, line chart, pie chart rendered from the current data range.
- Charts can be exported as PNG.
- Keyboard-navigable; TTS reads cell content and formulas aloud.
- **Scope note:** this is not a full Excel / LibreOffice Calc replacement. The goal is
  practical data management for the RetroWave 4M audience.
- **Gap-analysis recommended:** high-priority inclusion for the 1.0 release.

---

### Presentation (Linear Slides)

A lightweight slide-by-slide presentation tool.

- Linear navigation only (previous / next slide); no branching.
- Each slide contains text and optionally one image.
- Keyboard-navigated: left/right arrows to advance or go back.
- TTS reads slide content aloud (useful for accessible presentations).
- Export to a sequence of PNG images.
- **Gap-analysis recommended:** high-priority inclusion for the 1.0 release.

---

### Text Web Browser

A Lynx-inspired text-only web browser.

- Renders HTML as plain text; no JavaScript execution; no media auto-play.
- Keyboard navigation: arrow keys to select links; Enter to follow; Backspace to go back.
- TTS reads page content and link lists aloud.
- No cookie tracking; no form auto-fill.
- Conservative HTML parser: rejects or ignores unknown/complex structures.
- **This is not a Chrome-like browser.** It is intentionally limited to safe, text-based
  web access.

---

### Email Client

A text-based email client.

- Send and receive plain-text email via IMAP / SMTP.
- Keyboard-navigable folder and message list.
- TTS reads message subjects and body aloud.
- No HTML email rendering (plain text only); no inline images.
- Conservative MIME parser.

---

### Graphical DAW

A lightweight graphical Digital Audio Workstation, inspired by LMMS.

- Timeline-based arrangement view.
- Instrument and effects rack (uses the same synthesis engine as F4 tracker).
- Piano roll for note entry.
- Accessibility-minded: keyboard-navigable; TTS-compatible labels for all controls.
- Export: WAV and MP3.
- **Scope note:** this is not a full professional DAW. It is lighter and more focused than
  LMMS. For simple composition, the F4 text tracker is sufficient.

---

### Sprite Editor and Map Editor

A visual tool for creating and editing sprites and tile maps.

- Create and edit sprite sheets (PNG-based).
- Draw pixel-by-pixel or with primitive shapes.
- Define animation frames and frame timing.
- Map editor: place tiles/sprites on a 2D grid; export as BASIC array data.
- Keyboard and mouse support (mouse optional; keyboard-only mode available).

---

### 3D Editor

A 3D scene editor for composing and previewing 3D content.

- OBJ file import for external 3D models.
- Heightfield tools: create and edit terrain height matrices.
- Wormhole/tunnel tools: define cross-section polygon and spline path.
- Preview in wireframe / flat / flatwire modes.
- Export as BASIC data arrays (`heights`, `faceColors`, `WORMHOLE` parameters).

---

### Paint Editor

A lightweight raster paint application.

- Draw with brushes, fill, lines, shapes (rectangle, circle, polygon).
- RGBA colour picker.
- Layers (limited count to keep memory usage low).
- Export as PNG or BMP.
- **Scope note:** this is not a full GIMP replacement. It does not support advanced filters,
  non-destructive editing, or plug-ins.

---

### 3D Game Demo

A demonstration game showcasing the 3D rendering, sound, and BASIC capabilities.

- Uses the 3D layer (wireframe / flat / flatwire) and the sound engine.
- Serves as a reference implementation for 3D game programming in BASIC.
- Source code is fully readable and documented as a learning resource.

---

## Gap-Analysis Notes

The following companion apps are recommended for high-priority inclusion before 1.0:

| App | Priority | Notes |
|-----|----------|-------|
| Text Editor | High | Essential for working with BASIC source files |
| Spreadsheet | High | CSV + formulas; basic charting |
| Presentation | Medium | Lightweight; slide-by-slide; keyboard-navigated |

The following are non-goals for companion apps:

| Non-goal | Rationale |
|----------|-----------|
| Full GIMP parity | Memory, plug-in infrastructure, and complexity exceed the scope |
| Full Blender parity | Same as GIMP; 3D editor covers basic 3D composition needs |
| Chrome-like browser | Security, memory, and complexity reasons; text browser covers the use case |
| Untrusted third-party plug-ins | No plug-in loader; curated bundled apps only |

---

## Cross-References

- [UTILITIES.md](./UTILITIES.md) -- function-key utilities (F1--F12).
- [GRAPHICS.md](./GRAPHICS.md) -- graphics specification used by the Sprite Editor and 3D Editor.
- [SOUND.md](./SOUND.md) -- sound engine used by the DAW.
- [VISION.md](./VISION.md) -- gap-analysis, non-goals, scope boundaries.
