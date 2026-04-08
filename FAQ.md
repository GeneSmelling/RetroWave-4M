# FAQ -- Frequently Asked Questions -- RetroWave 4M

---

## General

### What is RetroWave 4M?

RetroWave 4M is a modern open-source home computer system for Raspberry Pi 4 and Pi 400,
inspired by the Commodore C16/C64 tradition and reimagined through **Neoism**: an alternate
present in which 1980s home computers evolved without losing their simplicity and character.

You boot directly to a BASIC prompt. There is no desktop OS, no window manager, and no taskbar.
Function keys F1--F12 are the shell. The system works fully without a monitor.

### What does "4M" mean?

"4M" stands for 4 million kilobytes (4 GB of RAM), echoing the spirit of the "64 kilobytes"
era while using today's hardware to deliver full-HD graphics, Unicode, offline voice, and safe
internet access.

### What is Neoism?

Neoism is the philosophical concept behind RetroWave 4M. It asks: what if home computers like
the Commodore 64 never died, but instead grew up on their own terms -- gaining HD output,
Unicode, offline TTS, and internet access -- without becoming a general-purpose GUI desktop?

The result is a system that is direct, predictable, and cohesive: one thing running at a time,
one prompt waiting for input, function keys for everything.

---

## Hardware and Setup

### What hardware do I need?

- Raspberry Pi 4 (2 GB RAM or more recommended) or Raspberry Pi 400.
- HDMI display (optional -- the system works fully without a monitor in TTS/headless mode).
- USB keyboard.
- SD card (16 GB or more recommended).

### Can I run it on a desktop PC or laptop?

Yes, for development and testing. The system runs on any Linux desktop with Python 3.10+.
GPIO features are only available on Raspberry Pi hardware.

### How do I install it?

See [SETUP.md](./SETUP.md).

---

## Features and Scope

### Is there a web browser?

Yes, a **text-based browser** (Lynx-like) is provided as a companion app. It renders HTML as
plain text; no JavaScript, no media auto-play, no tracking.

There is **no Chrome-like graphical browser**. A full graphical browser is a separate OS
subsystem and is explicitly out of scope.

### Is there a word processor or spreadsheet?

A **standalone text editor** (accessible, TTS-compatible) and a **lightweight spreadsheet**
(CSV + formulas + basic charting) are planned as companion apps and are recommended for
high-priority inclusion. See [COMPANION_APPS.md](./COMPANION_APPS.md).

### Can I run multiple programs at the same time?

No. RetroWave 4M is **monotasking**: one program runs at a time. Background audio playback and
network I/O are the only permitted parallel activities. This is a deliberate design choice: it
reduces complexity, cognitive load, and the security attack surface.

### Is there a graphical desktop?

No. The interface is a character-cell text grid (48 x 27 cells at 40 x 40 px). There is no
window manager. Function keys F1--F12 are the application launcher.

### Does it support 3D graphics?

Yes. The 3D layer supports three rendering modes (wireframe / flat / flatwire), a camera with
FOV of 90 degrees by default, and the following primitives:

- Cuboid, sphere, cylinder.
- Torus (with per-face RGBA colour matrix).
- Polyhedron and Platonic solids (tetrahedron, cube, octahedron, dodecahedron, icosahedron).
- Heightfield terrain (2D height grid with per-face colours).
- Wormhole/tunnel (polygon cross-section + spline path + subdivisions; per-face colour matrix).

See [GRAPHICS.md](./GRAPHICS.md) for the full specification.

### What is the text grid resolution?

48 columns x 27 rows of character cells. Each cell is 40 x 40 px, designed for a 1920 x 1080
display. Unicode glyph rendering supports worldwide alphabets.

---

## Accessibility

### Can I use RetroWave 4M without a monitor?

Yes. If no monitor is connected, TTS (Text-to-Speech) is automatically enabled. The system
reads all output, menus, and prompts aloud. You can learn BASIC and use all utilities entirely
by audio.

### How do I enable voice output?

Use `VOICE ON` in BASIC, or configure it in F2 Options (language, voice gender, enable/disable).
Voice activates automatically when no monitor is detected.

### What TTS engine is used?

**Piper** (offline, high-quality neural voices) is the primary engine. **eSpeak NG** is the
fallback for languages not covered by Piper. No cloud connection is required.

### Are screen-reader shortcuts available?

Yes:
- `F9` reads the cursor position and current cell content aloud.
- `F10` reads the current line aloud.
- `Ctrl+.` stops speech immediately (`SAYSTOP`).
- `Ctrl+Shift+.` clears queued speech without interrupting the current utterance (`SAYFLUSH`).

See [ACCESSIBILITY.md](./ACCESSIBILITY.md) and [KEYBOARD.md](./KEYBOARD.md).

---

## Security and Safety

### Is file access sandboxed?

Yes. BASIC can only access files in the user data directory. System files, hidden files, and
executable files are inaccessible. The maximum file size is 200 MB.

### Can I accidentally delete important files?

All delete operations move files to a **Trashcan** directory. Permanent deletion requires a
second explicit step.

### Is SQLite safe to use from BASIC?

Yes, with the built-in security posture: all SQL parameters MUST be passed as bind arguments
(the `?` placeholder syntax). String interpolation into SQL statements is not permitted,
preventing SQL injection.

### Can I run native code or shell commands?

No. Only BASIC statements can execute. There is no native code loader, no shell command
execution, and no binary blob loader.

---

## Crypto and Ecology

### Is crypto mining supported?

Optional Litecoin mining is available via the F6 Crypto Wallet utility. It is **disabled by
default** and requires explicit user consent. A CPU share cap (default 25%) limits resource
use. Mining auto-pauses when running on battery. The ecology impact is prominently disclosed.

### Why is the ecology impact disclosed?

CPU mining consumes electricity. RetroWave 4M targets energy-efficient hardware (Raspberry Pi),
but transparency about mining's energy use is a design principle.

---

## Development and Contributing

### How do I contribute?

See [CONTRIBUTING.md](./CONTRIBUTING.md).

### What are the non-goals?

See [VISION.md -- Non-Goals](./VISION.md#non-goals). Key non-goals:
- Full GIMP / Blender parity.
- Modern shader pipelines (GLSL, Vulkan, Metal).
- Chrome-like browser.
- Untrusted third-party plug-ins.
- Real multitasking.

### Where is the full documentation?

| Document | Contents |
|----------|---------|
| [README.md](./README.md) | Project overview |
| [VISION.md](./VISION.md) | Vision, Neoism, scope, non-goals, gap-analysis, risks |
| [ARCHITECTURE.md](./ARCHITECTURE.md) | System architecture, security model, threat model |
| [BASIC_LANGUAGE.md](./BASIC_LANGUAGE.md) | BASIC language reference |
| [GRAPHICS.md](./GRAPHICS.md) | Graphics specification |
| [SOUND.md](./SOUND.md) | Sound and TTS specification |
| [ACCESSIBILITY.md](./ACCESSIBILITY.md) | Accessibility features |
| [UTILITIES.md](./UTILITIES.md) | Function-key utilities |
| [KEYBOARD.md](./KEYBOARD.md) | Keyboard shortcuts |
| [COMPANION_APPS.md](./COMPANION_APPS.md) | Companion application catalogue |
| [DATA_STRUCTURES.md](./DATA_STRUCTURES.md) | Data types and structures |
| [SETUP.md](./SETUP.md) | Installation and setup |
| [CONTRIBUTING.md](./CONTRIBUTING.md) | How to contribute |
