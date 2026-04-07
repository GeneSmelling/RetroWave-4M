# Vision — RetroWave 4M

## Vision Statement

RetroWave 4M aims to create a cohesive home-computer experience for Raspberry Pi that blends the
simplicity and character of 1980s personal computers with modern capability: full-HD graphics,
accessible text UI, offline TTS, networked connectivity, and practical productivity tools.

The guiding philosophy is **Neoism**: an alternate present in which 8-bit and 16-bit home
computers never died but instead grew up gracefully, keeping their direct relationship with the
user while gaining Unicode, HD output, offline voice, and safe internet access -- all without
becoming a general-purpose desktop OS.

---

## Neoism

Neoism is the conceptual foundation of RetroWave 4M. It asks: what if the Commodore 64, the ZX
Spectrum, and the MSX line had continued to evolve on their own terms, without being displaced by
GUI desktops and Windows?

Key Neoism principles:

- **Direct access:** the user boots into a BASIC prompt or the last-used app, with no login
  screen, no desktop icons, no taskbar.
- **Monotasking:** one program runs at a time (limited background work is permitted for audio
  playback and network I/O only). This is a feature, not a limitation: it reduces cognitive load,
  simplifies security, and makes the machine predictable.
- **Character-cell heritage:** the text UI is a grid of character cells (48 x 27 at 40 x 40 px),
  rendered with Unicode glyphs, high-contrast colour schemes, and optional TrueType overlay.
- **Function keys as the UI shell:** F1--F12 are the application launcher and navigation layer.
  No mouse is required for any core function.
- **Accessible by default:** voice output (TTS) is a first-class citizen, not an add-on. The
  system is fully usable without a monitor.

---

## Scope and Goals

### In scope

| Area | Key elements |
|------|-------------|
| BASIC runtime | Line-numbered BASIC; monotasking; records; first-class data structures; SQLite with bind params |
| Text UI | 48 x 27 grid; 40 x 40 px cells; Unicode; high-contrast colour schemes |
| Accessibility | VOICE ON / auto-voice; TTS queue (Piper); F9/F10 screen-reader shortcuts; headless operation |
| Graphics | 3 display modes (1920x1080, 960x540, 640x360); logical coords 0-1919/0-1079; 5 layers; 3D wire/flat/flatwire; FOV 90; per-face RGBA matrices |
| 3D primitives | Cuboid, sphere, cylinder, torus, polyhedron, Platonic solids, heightfield, wormhole/tunnel |
| 2D primitives | Rectangle, circle, polygon, regular polygon; all with optional rotation |
| Sound | Piper TTS; sampled WAV/MP3/OGG; synthesiser (analog/FM/granular); SF2 soundfonts; Synthwave presets; F4 tracker; DAW companion app |
| Utilities (F1-F12) | Help, Options, File manager, Music, Network, Crypto wallet, Accessibility shortcuts |
| Companion apps | Text browser (Lynx-like), email client, DAW, sprite editor, 3D editor, paint editor, text editor, 3D game demo |
| Productivity | Accessible text editor; spreadsheet-like tables (CSV + formulas); basic charting; linear presentation mode; FTP; backup/restore + integrity checks |
| Storage safety | Sandbox; Trashcan; 200 MB file cap; no hidden/system/executable files |
| Networking | USB tethering; mesh; conservative parsers; text-only browser |
| Crypto | Wallet management; optional Litecoin mining with CPU cap and ecology disclosure |
| GPIO | Safe, sandboxed access on Raspberry Pi hardware |
| Ecology | CPU-cap on mining; energy-efficient hardware target |

### Gap-analysis: high-priority inclusions

The following items were identified as absent from earlier documentation but highly valuable for
the target audience. They are recommended for inclusion before the 1.0 release:

1. **Accessible text editor** -- a standalone companion app for editing plain text and BASIC
   source files, keyboard-navigable, TTS-compatible.
2. **Spreadsheet-like tables** -- CSV import/export, cell formulas (arithmetic + basic functions),
   column sorting; intended for budgeting, data management, and small datasets, not full Excel
   parity.
3. **Basic charting** -- bar, line, and pie charts rendered from BASIC arrays or CSV data;
   keyboard-navigable; printable as PNG.
4. **Lightweight linear presentation mode** -- slide-by-slide display of text + graphics;
   keyboard-navigated; no embedded video or animations.
5. **Minimal FTP workflow** -- upload and download files over FTP/SFTP through the F3 file
   manager or a dedicated companion app; passive mode support.
6. **Backup/restore + integrity checks** -- scheduled or manual backup to USB or network share;
   SHA-256 checksum verification; simple restore wizard accessible via F2 or F3.
7. **Conservative parsers** -- all parsers (HTML, email, media, network protocols) must operate in
   a strict, minimal-surface mode: reject unknown structures rather than guess; no script
   execution; no auto-redirect chains.

---

## Non-Goals

The following are explicitly out of scope and will not be pursued:

| Non-goal | Rationale |
|----------|-----------|
| Full GIMP / Blender parity | Memory, complexity, and UX cost far exceed the system's scope. Lightweight paint and 3D editors are provided instead. |
| Modern shader pipelines (GLSL, Vulkan, Metal) | The 3-mode rendering engine (wire/flat/flatwire) is intentional. Shader support would break the retro aesthetic and balloon complexity. |
| Chrome-like browser | A general-purpose graphical browser is a full OS subsystem. A text browser (Lynx-like) is provided for safe, accessible web access. |
| Untrusted third-party plugins | Plugins from unknown sources are not supported. The companion app model (curated, bundled apps) is the extension mechanism. |
| Real multitasking | Background audio and network I/O are the only permitted parallel activities. No window manager, no overlapping apps. |
| Cloud-dependent features | All core features must work offline. Network features (FTP, wallet, mesh) are optional extras. |

---

## Risks and Mitigations

| Risk | Mitigation |
|------|-----------|
| Unicode/TTS complexity | Piper engine with eSpeak NG fallback; language packs installed at setup; LRU cache for voices; test suite covering edge-case glyphs and language tags |
| Sandbox edge cases (path traversal, symlinks) | Strict path normalisation; symlinks resolved and checked; no access outside the user data directory; automated regression tests |
| Network parsing surface | Conservative, whitelist-based parsers; no JavaScript; no auto-redirect; sandboxed network process |
| Crypto key safety | Keys stored in an encrypted local store; no key export over network; mining disabled by default; explicit user consent required to enable |
| Mining vs ecology | CPU share cap (configurable, default 25 %); real-time energy-use indicator in F6 wallet utility; mining auto-pauses on battery |
| Growing scope creep | Maintain this non-goals list; require explicit community RFC for any addition that touches the BASIC runtime or security model |

---

## Cross-References

- [README.md](./README.md) -- project overview
- [ARCHITECTURE.md](./ARCHITECTURE.md) -- system architecture, threat model, security model
- [GRAPHICS.md](./GRAPHICS.md) -- full graphics specification
- [SOUND.md](./SOUND.md) -- sound engine specification
- [ACCESSIBILITY.md](./ACCESSIBILITY.md) -- accessibility features
- [COMPANION_APPS.md](./COMPANION_APPS.md) -- companion application catalogue
- [FAQ.md](./FAQ.md) -- frequently asked questions
