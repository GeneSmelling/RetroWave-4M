# RetroWave 4M

**RetroWave 4M** is a modern, open-source home computer for Raspberry Pi 4/400, inspired by the
Commodore C16/C64 experience and reimagined through **Neoism**: an alternate present in which
1980s home computers evolved without losing their character, simplicity, and playful creativity.

"4M" stands for **4 million kilobytes** (4 GB), echoing the spirit of the "64 kilobytes" era while
using today's technology to deliver speed, multimedia, accessibility, and safety.

RetroWave 4M is designed to be:

- **Monotasking** -- one task at a time; no window chaos; predictable, focused control.
- **BASIC-first** -- line numbers, structured extensions, no object orientation.
- **Accessibility-first** -- fully usable without a monitor.
- **Secure-by-design** -- sandboxed I/O, no executable scripts, conservative parsers.
- **A world to discover** -- built-in utilities, companion apps, games, and learning paths.

---

## What RetroWave 4M Is (and Isn't)

### It *is*

- A cohesive retro-modern system where BASIC, graphics, sound, and utilities feel like one unified
  home-computer universe.
- A platform where accessibility is not an add-on: it is core functionality, enabled automatically
  when no monitor is detected.
- A home for practical productivity: text editing, spreadsheet-like tables with CSV and formulas,
  basic charting, a linear presentation mode, FTP file transfer, and backup/restore.

### It *is not*

- A full replacement for professional desktop tools such as **GIMP**, **Blender**, or a modern
  web browser -- there is no Chrome-like browser.
- A multitasking desktop OS with overlapping windows, background apps, or untrusted third-party
  plugins.
- A modern shader pipeline (no GLSL/Vulkan/Metal; rendering uses the built-in 3-mode engine:
  wireframe / flat / flatwire).

See [VISION.md](./VISION.md) and [FAQ.md](./FAQ.md) for full scope and non-goals.

---

## Core Experience

### Modern BASIC -- Monotasking

- Line-numbered, non-object-oriented BASIC.
- `DEF PROC`, `DEF FN`, `FUNCTION/SUB`, `INCLUDE`, `SELECT CASE`.
- Records (`TYPE ... END TYPE`) and first-class data structures: List, Tree, Graph, Stack, Queue,
  Set, Map, Priority Queue, Deque.
- Unicode everywhere -- code identifiers and string output.
- SQLite from BASIC (`DB OPEN`, `DB EXEC`, `DB QUERY`) with mandatory bind parameters to prevent
  SQL injection.

See [BASIC_LANGUAGE.md](./BASIC_LANGUAGE.md) and [DATA_STRUCTURES.md](./DATA_STRUCTURES.md).

### Full HD Retro Text UI -- Unicode

- Character grid: **48 columns x 27 rows**.
- Each character cell: **40 x 40 px**, designed for clarity at 1920 x 1080.
- Unicode glyph rendering supports worldwide alphabets.

### Voice and Accessibility -- Monitor Optional

- `VOICE ON` enables system-wide speech feedback.
- Speech is applied to `PRINT`, `INPUT`, `LIST` (BASIC keywords spoken in English; user strings
  spoken in the selected language), menus, utilities, and keypress feedback.
- The system automatically enables voice mode when no monitor is detected.
- `F9` reads the cursor position and cell content aloud; `F10` reads the current line aloud.

See [ACCESSIBILITY.md](./ACCESSIBILITY.md).

---

## Graphics

Switch to graphics mode with the `GRAPHIC` command.

**Supported display modes:**

| Mode | Resolution | Pixel mapping |
|------|-----------|---------------|
| Full HD | 1920 x 1080 | 1 logical pixel = 1 physical pixel |
| Half HD | 960 x 540 | 1 logical pixel = 2x2 physical pixels |
| Quarter HD | 640 x 360 | 1 logical pixel = 3x3 physical pixels |

Logical coordinates are always **0-1919 (X) / 0-1079 (Y)** regardless of the active mode, so
artwork keeps its proportions across all modes.

**5-layer system (front-to-back):**

1. Text -- Unicode characters + optional TrueType overlay.
2. 2D graphics -- primitives, images, video, UI widgets.
3. 2D sprites -- sublayers, collisions, animation, rotation.
4. 3D -- wireframe / flat / flatwire rendering.
5. Background -- solid colour, gradient, scrolling image, or tile map.

**3D features:** primitives (cuboid, sphere, cylinder, torus, polyhedron, Platonic solids),
heightfield terrain, wormhole/tunnel (polygon cross-section + spline path + subdivisions),
per-face RGBA colour matrices, FOV default 90 degrees, three rendering modes (wire / flat / flatwire).

**2D primitives:** rectangle, circle, arbitrary polygon, regular polygon (with optional rotation).

**Media formats:** JPEG, PNG, BMP (images); MP4/H.264 (video); WAV, MP3, OGG (audio).

**Sprites:** rotation with configurable anchor point; sublayer ordering; collision detection.

See [GRAPHICS.md](./GRAPHICS.md).

---

## Sound and Music

- Built-in TTS using the **Piper** offline engine (multi-language, voice gender selection).
- Sampled audio: record or import WAV, MP3, OGG.
- Runtime synthesis: oscillators (virtual analog, FM, granular), ADSR envelopes, filters, FX.
- SoundFonts (SF2): lazy-loaded, LRU-cached. Bundled library covers ethnic/acoustic and
  synth-based instruments; Synthwave preset bank included.
- **F4 Music Tracker** (text/accessible, no mouse required): compose, load, save, and export to
  WAV or BASIC `TRACK` commands.
- **Graphical DAW** companion app (LMMS-like, lightweight, accessibility-minded); exports MP3.

See [SOUND.md](./SOUND.md).

---

## System Utilities -- Function Keys

All utilities run in accessible text mode unless otherwise noted.

| Key | Utility |
|-----|---------|
| `Print Screen` | Screenshot -- timestamped PNG |
| `F1` | Help / manual (multilingual, voice-driven, works without monitor) |
| `F2` | Options and startup preferences |
| `F3` | File manager (Midnight-Commander style; Trashcan for safe delete) |
| `F4` | Music tracker / DAW |
| `F5` | Network (USB tethering, mesh network) |
| `F6` | Crypto wallet (management + optional Litecoin mining with CPU cap) |
| `F9` | Read cursor position and cell content aloud |
| `F10` | Read current line aloud |
| `F12` | Restart / Shutdown |

See [UTILITIES.md](./UTILITIES.md) and [KEYBOARD.md](./KEYBOARD.md).

---

## Companion Applications

Standalone apps launched from BASIC or the file manager (not resident utilities, to save memory):

- Text web browser (Lynx-like; no Chrome-like browser).
- Text email client.
- Graphical DAW (LMMS-like, lightweight).
- Sprite editor and map editor.
- 3D scene editor (OBJ import, heightfield, wormhole/tunnel tools).
- Paint editor (lightweight; not full GIMP parity).
- 3D game demo.

See [COMPANION_APPS.md](./COMPANION_APPS.md).

---

## Security and Safety

- **Sandboxed file I/O:** no system files, no hidden files, no executable/batch/autorun files.
- **Max file size:** ~200 MB per file (e.g. MP4 video).
- **Trashcan:** all deletions go to a recoverable Trashcan directory.
- **SQLite:** mandatory bind parameters; no string interpolation into SQL.
- **Static analysis:** suspicious BASIC listings are flagged at load time; user may cancel.
- **No machine code:** only BASIC statements execute; no native loaders.
- **Conservative parsers:** all network and media parsers use strict, minimal attack surfaces.
- **Crypto mining CPU cap:** mining is limited in CPU share; ecology impact is disclosed to users.
- **GPIO:** safe, sandboxed access on Raspberry Pi hardware.

See [ARCHITECTURE.md](./ARCHITECTURE.md) for the full security model and risk analysis.

---

## Gap-Analysis Highlights

The following practical productivity tools are recommended for high-priority inclusion:

- Accessible text editor (standalone companion app).
- Spreadsheet-like tables with CSV import/export and formulas.
- Basic charting (bar, line, pie) driven from BASIC data structures.
- Lightweight linear presentation mode (slide-like, keyboard-navigated).
- Minimal FTP workflow for file transfer.
- Backup/restore with integrity checks.

See [VISION.md](./VISION.md) for the full gap-analysis discussion and non-goals.

---

## Getting Started

See [SETUP.md](./SETUP.md).

---

## Contributing

See [CONTRIBUTING.md](./CONTRIBUTING.md).

---

## License

See [LICENSE](./LICENSE).
