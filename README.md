# RetroWave 4M

**RetroWave 4M** is a modern, open-source home computer for Raspberry Pi 4/400, inspired by the Commodore C16/C64 experience, reimagined through **Neoism**: an alternate present where 1980s home computers evolved without losing their character, simplicity, and playful creativity.

“4M” stands for **4 million kilobytes** (4 GB), echoing the spirit of the “64 kilobytes” era—while using today’s technology to deliver speed, multimedia, accessibility, and safety.

RetroWave 4M is designed to be:
- **Monotasking** (focus and control; no window chaos)
- **BASIC-first** (line numbers, structured extensions, no object orientation)
- **Accessibility-first** (usable without a monitor)
- **Secure-by-design** (sandboxed I/O, no executable scripts)
- **A world to discover** (utilities, apps, games, learning paths)

---

## What RetroWave 4M Is (and Isn’t)

### It *is*
- A cohesive retro-modern system where BASIC, graphics, sound, and utilities feel like one “home computer universe”.
- A platform where accessibility is not an add-on: it is core functionality.

### It *is not*
- A full replacement for professional desktop software like **GIMP**, **Blender**, or a modern web browser.
- A multitasking desktop OS with overlapping windows, background apps, plugins, and heavy third‑party extensions.

For pragmatic scope and a “gap analysis” of what should be included, see:  
- [VISION.md](./VISION.md)  
- [ARCHITECTURE.md](./ARCHITECTURE.md)  
- [FAQ.md](./FAQ.md)

---

## Core Experience

### Modern BASIC (Monotasking)
- Line-numbered, non-object-oriented BASIC
- `DEF PROC`, `DEF FN`, `INCLUDE`, `CASE`
- Records + fundamental data structures (lists, trees, graphs)
- Unicode everywhere (code and output)
- SQLite support from BASIC (with security posture)

### Full HD Retro Text UI (Unicode)
- Pixel-perfect character grid: **48×27**
- Each character: **40×40 px**, optimized for clarity
- Unicode glyph rendering to support worldwide alphabets

### Voice & Accessibility (Monitor optional)
- `VOICE ON` enables system-wide speech feedback
- Speech affects: `PRINT`, `INPUT`, `LIST` (keywords in English; strings in selected language), menus, utilities, keypress feedback
- System auto-enables voice mode when no monitor is detected

---

## Graphics
Switch to graphics with `GRAPHIC`.

Supported modes:
- 1920×1080 (Full HD)
- 960×540 (2×2 logical pixels)
- 640×360 (3×3 logical pixels)

Coordinates remain **logical 0–1919 / 0–1079** (with clipping), so visuals keep their proportions across modes.

5 layers:
1. Background (solid/gradient/image/scroll/tile)
2. 3D (wireframe / flat / flatwire)
3. 2D sprites (sublayers, collisions, animation)
4. 2D graphics (primitives, images/video, UI widgets)
5. Text (Unicode + optional TrueType overlay)

See [GRAPHICS.md](./GRAPHICS.md).

---

## Sound & Music
- Built-in TTS (multi-language, voice gender)
- Sampled audio (record/import WAV/MP3)
- Runtime synth instruments (oscillators, ADSR, filters, FX)
- SoundFonts (lazy-loaded, garbage-collected)
- Tracker utility (text/accessible) and a graphical DAW companion app
- Export to WAV; DAW export to MP3

See [SOUND.md](./SOUND.md).

---

## System Utilities (Function Keys)
Utilities are part of the core system and must remain accessible.

- STAMP: Screenshot to PNG
- F1: Help / manual (multilingual + accessible)
- F2: Options / preferences
- F3: File manager (safe delete to Trashcan)
- F4: Music tracker utility
- F5: Network utility
- F6: Crypto wallet utility
- F9: Speak cursor position + cell content
- F10: Speak current line
- F12: Reboot/shutdown

See [UTILITIES.md](./UTILITIES.md).

---

## Companion Applications
In addition to utilities, RetroWave 4M provides standalone companion apps (separate directories; launched via BASIC commands or the file manager), such as:
- Text web browser (Lynx-like)
- Text email client
- Graphical DAW (LMMS-like, “light”, accessibility-minded)
- Sprite editor + map editor
- 3D editor (OBJ import, heightfield, wormhole/tunnel tools)
- Paint editor (GIMP-like “light”)
- 3D game demo

See [COMPANION_APPS.md](./COMPANION_APPS.md).

---

## Security & Safety (Short)
- Sandboxed file I/O: no system files, no hidden files, no executable/batch/autorun
- Max file size target: ~200 MB
- Trashcan directory for safe delete
- SQLite support with a security-first posture (reduce injection risk)
- Optional static analysis warnings on suspicious BASIC scripts

See [ARCHITECTURE.md](./ARCHITECTURE.md).

---

## Getting Started
See [SETUP.md](./SETUP.md).

---

## Contributing
See [CONTRIBUTING.md](./CONTRIBUTING.md).

---

## License
See [LICENSE](./LICENSE).