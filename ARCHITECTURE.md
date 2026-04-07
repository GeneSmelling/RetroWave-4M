# System Architecture -- RetroWave 4M

> **Note:** This document describes the overall system architecture.
> Detailed graphics specifications have been moved to [GRAPHICS.md](./GRAPHICS.md).

---

## Overview

RetroWave 4M is a **monotasking** home computer system targeting Raspberry Pi 4 and Pi 400.
The user interacts with one program at a time. Limited background parallelism is permitted only
for audio playback and network I/O; everything else is strictly sequential.

The system boots directly to a BASIC prompt (or the last-used app). There is no login screen,
no desktop environment, and no window manager. Function keys F1--F12 serve as the application
launcher and navigation layer.

---

## Subsystems

### 1. Text UI

- **Grid:** 48 columns x 27 rows of character cells.
- **Cell size:** 40 x 40 px, optimised for 1920 x 1080 display.
- **Rendering:** Unicode glyph rendering (supports worldwide alphabets). Optional TrueType
  overlay in graphics mode.
- **Colour schemes:** Classic C64, Classic C16, Green Phosphor, Night Mode, High Contrast, Custom.
- **Input:** keyboard-only; no mouse required for any core function.

### 2. BASIC Runtime

- Line-numbered, non-object-oriented BASIC.
- Structured extensions: `DEF PROC`, `DEF FN`, `FUNCTION/SUB`, `INCLUDE`, `SELECT CASE`.
- Records (`TYPE ... END TYPE`), multi-dimensional arrays, `REDIM`.
- First-class data structures: List, Tree, Graph, Stack, Queue, Set, Map, Priority Queue, Deque.
- Math: standard library + precalculated SINF/COSF tables (36 000 entries at 0.01-degree
  resolution); matrix operations (`MAT DIM/ADD/MUL/TRANS/ROT/DET/INV`); calculus helpers
  (`INTEGRAL`, `DERIVATIVE`); financial functions (NPV, IRR, PMT, FV, PV).
- Execution is **single-threaded**; no coroutines or threads visible to the programmer.

See [BASIC_LANGUAGE.md](./BASIC_LANGUAGE.md).

### 3. Graphics Subsystem

- Three display modes: 1920x1080, 960x540 (2x2 pixel blocks), 640x360 (3x3 pixel blocks).
- Logical coordinates always 0-1919 (X) / 0-1079 (Y), regardless of active mode.
- 5-layer rendering stack (see [GRAPHICS.md](./GRAPHICS.md) for full specification):
  1. Text layer
  2. 2D graphics
  3. 2D sprites
  4. 3D layer (wire / flat / flatwire; FOV 90 degrees default)
  5. Background layer
- Double buffering (front and back buffers) to eliminate flicker.
- Asset lazy loading with LRU eviction.

### 4. Audio Subsystem

- **TTS:** Piper (offline, primary); eSpeak NG (fallback for uncovered languages). Utterances
  queued in a FIFO; no overlap; `SAYSTOP` / `SAYFLUSH` for queue control.
- **Sampled audio:** WAV, MP3, OGG; record or import.
- **Synthesis:** virtual analog, FM, and granular oscillators; ADSR envelopes; filters; FX
  (reverb, delay, EQ).
- **SoundFonts (SF2):** one file per instrument; lazy-loaded; LRU cache with configurable budget.
- **Tracker (F4):** text/accessible, no mouse; export to WAV or BASIC TRACK commands.
- **DAW companion app:** graphical (LMMS-like, lightweight); exports MP3.
- Background audio playback is the only background activity permitted alongside BASIC execution.

See [SOUND.md](./SOUND.md).

### 5. Accessibility Engine

- `VOICE ON` / `VOICE OFF` command; also set via `F2` Options.
- **Auto-voice:** if no monitor is detected at boot, or if the monitor is disconnected, the system
  automatically enables TTS.
- Speech coverage: `PRINT`, `INPUT`, `LIST` (BASIC keywords in English; user strings in the
  selected language), menus, utilities, keypress feedback.
- `F9` -- reads cursor position and cell content aloud.
- `F10` -- reads the current line aloud.
- All utilities (F1--F12) are designed for full keyboard + TTS operation.

See [ACCESSIBILITY.md](./ACCESSIBILITY.md).

### 6. Storage and Sandbox

- **Allowed paths:** only the user data directory; no system directories.
- **Forbidden file types:** executables, batch files, autorun scripts, hidden files.
- **Hidden files** are surfaced as visible (no hidden-file bypass).
- **Max file size:** 200 MB per file.
- **Trashcan:** all delete operations move files to a Trashcan directory; permanent deletion
  requires an explicit second step.
- **SQLite:** `DB OPEN`, `DB EXEC`, `DB QUERY` from BASIC; all SQL parameters MUST use bind
  arguments; string interpolation into SQL is not permitted.
- **Static analysis:** BASIC listings are scanned at load time; suspicious patterns (abnormal
  PEEK/POKE, suspicious string constructions) are flagged; user may cancel execution.
- **INCLUDE sandboxing:** included files may only define functions, procedures, data, and
  track/map resources; they cannot execute statements or include further files.

### 7. Network Subsystem

- USB tethering and mesh network (encryption on by default); configured via `F5`.
- **Text-only browser** (Lynx-like companion app): HTML rendered as text; no JavaScript;
  no media auto-play; no cookie tracking.
- **Email client** (text-based companion app): send and receive plain-text email.
- **FTP/SFTP** support in the F3 file manager or a dedicated companion app.
- All parsers operate in a **conservative mode**: reject unknown structures; no auto-redirect
  chains; no inline script execution.

### 8. Crypto Subsystem

- Managed via `F6` Crypto Wallet utility.
- Wallet keys stored in an encrypted local store; no key export over network.
- **Mining:** optional Litecoin CPU mining; disabled by default; requires explicit user consent.
  - CPU share cap: configurable, default 25 %.
  - Real-time energy-use indicator shown in F6.
  - Mining auto-pauses on battery power.
  - Ecology impact disclosed prominently to the user.

### 9. GPIO

- Available on Raspberry Pi 4 and Pi 400 hardware.
- Accessed from BASIC via sandboxed GPIO commands.
- No direct memory access; pin access is mediated through a safe API.

### 10. Lazy Loading and Memory Management

- Graphics assets (sprites, textures, heightmaps, 3D meshes) are loaded on demand.
- Audio assets (samples, soundfonts, presets) are loaded on first play.
- Piper voice models are loaded lazily with an LRU policy and configurable memory budget
  (`TTS_CACHE_MB`).
- Cached assets may be pre-loaded explicitly with `PRELOAD` for performance-critical scenes.
- Under memory pressure, least-recently-used assets are evicted from cache.

---

## Threat Model and Security Model

### Principles

1. **No machine code.** Only BASIC statements execute. No native code loaders, no shell commands,
   no binary blobs.
2. **Restricted file I/O.** See Storage and Sandbox section above.
3. **SQL injection prevention.** Mandatory bind parameters for all SQLite queries.
4. **Conservative network parsers.** Whitelist-based; no script execution; no auto-redirect.
5. **Crypto key safety.** Local encrypted storage; no network key export; explicit mining consent.
6. **Minimal attack surface.** Monotasking eliminates whole classes of inter-process attacks.

### Risk Register

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|-----------|
| Unicode/TTS edge cases | Medium | Low | Piper + eSpeak NG fallback; regression tests for edge-case glyphs and language tags |
| Sandbox path traversal | Low | High | Strict path normalisation; symlink resolution + check; automated regression tests |
| Network parser exploitation | Medium | Medium | Conservative, whitelist-based parsers; no JavaScript; no auto-redirect |
| Crypto key theft | Low | High | Local encrypted store; no network export; user consent required |
| Mining CPU/battery abuse | Medium | Medium | CPU cap (default 25 %); auto-pause on battery; energy indicator |
| BASIC static analysis bypass | Low | Medium | Warn-and-cancel model; no silent execution; user always informed |

---

## Scope Boundaries (Non-Goals)

| Non-goal | Why |
|----------|-----|
| Full GIMP / Blender parity | Memory and complexity exceed the system's scope |
| Modern shader pipelines (GLSL, Vulkan, Metal) | Intentional retro rendering model |
| Chrome-like browser | Full browser is a separate OS subsystem; text browser is provided |
| Untrusted third-party plugins | No plugin loader; curated companion apps only |
| Real multitasking | Background audio + network I/O are the only exceptions |
| Cloud-dependent core features | All core features must work fully offline |

---

## Cross-References

- [README.md](./README.md) -- project overview and quick-start
- [VISION.md](./VISION.md) -- vision, Neoism philosophy, gap-analysis, non-goals
- [GRAPHICS.md](./GRAPHICS.md) -- full graphics specification (layers, 3D, 2D, sprites)
- [BASIC_LANGUAGE.md](./BASIC_LANGUAGE.md) -- BASIC language reference
- [SOUND.md](./SOUND.md) -- sound engine specification
- [ACCESSIBILITY.md](./ACCESSIBILITY.md) -- accessibility features
- [UTILITIES.md](./UTILITIES.md) -- function-key utilities
- [KEYBOARD.md](./KEYBOARD.md) -- keyboard shortcuts
- [COMPANION_APPS.md](./COMPANION_APPS.md) -- companion application catalogue
- [DATA_STRUCTURES.md](./DATA_STRUCTURES.md) -- data structures reference
- [SETUP.md](./SETUP.md) -- installation and setup
