# Keyboard Shortcuts -- RetroWave 4M

This document defines the normative keyboard shortcuts for RetroWave 4M.

> Shortcuts are specified at the project level even if a given implementation has not yet
> fully applied them.

## Conventions

- **Ctrl** means the Control key.
- **Shift** means the Shift key.
- **Alt** means the Alt (or Option) key.
- Keys are written in U.S. keyboard naming (e.g. `Esc`, `Enter`, `Backspace`).
- "MUST" / "SHOULD" / "MAY" are used in the RFC 2119 sense.

---

## Function Keys -- Utility Launcher

The function keys F1--F12 launch the built-in utilities described in [UTILITIES.md](./UTILITIES.md).
All utilities run in accessible text mode unless otherwise noted.

| Key | Utility | Notes |
|-----|---------|-------|
| `F1` | **Help** -- interactive manual | Accessible/voice-driven; works without a monitor |
| `F2` | **Options and Startup Preferences** | Language, colour scheme, voice, printer, login, backup |
| `F3` | **File Manager** | Midnight-Commander style; text editor, image/sprite viewer, audio/video player, FTP |
| `F4` | **Music Utility (Tracker)** | Tracker-style composer; export to WAV or BASIC commands |
| `F5` | **Network** | USB tethering and mesh network configuration |
| `F6` | **Crypto Wallet** | Wallet management and optional Litecoin mining |
| `F7` | *(reserved / implementation-defined)* | |
| `F8` | *(reserved / implementation-defined)* | |
| `F9` | **Read cursor position and cell content** | Reads aloud cursor coordinates and the current cell; in graphics mode reads the current graphics mode |
| `F10` | **Read current line** | Reads aloud the content of the current line |
| `F11` | **Toggle fullscreen** | Development/testing in desktop OS only (see note below) |
| `F12` | **Restart / Shutdown** | Prompts for restart or shutdown; equivalent to Hard Reset |
| `Print Screen` (STAMP) | **Screenshot** | Saves a timestamped PNG to the `screenshots/` directory; active in text and graphics modes |

> The Sprite Editor, 3D Editor, Paint Editor, DAW, and other companion apps are separate
> applications and are not assigned to function keys. See [COMPANION_APPS.md](./COMPANION_APPS.md).

---

## Global Shortcuts

These shortcuts MUST work regardless of the current screen (splash, BASIC editor, running
program, etc.), unless stated otherwise.

| Action | Shortcut | Normative behaviour |
|--------|---------|---------------------|
| Quit / Exit | `Alt+F4` (window managers) or window close button | MUST exit cleanly (release audio/video resources) |
| Pause / Resume | `Pause` | SHOULD toggle pause if a pause system exists; otherwise MAY be unassigned |
| Screenshot | `Print Screen` | MUST save a timestamped PNG to the screenshots directory (text and graphics modes) |
| Toggle fullscreen | `F11` | SHOULD toggle between windowed and fullscreen (desktop mode only) |

---

## Reset / Break Shortcuts (BASIC Runtime)

### Soft Reset

- **Shortcut:** `Ctrl+R`
- **Meaning:** reset the current program execution context while keeping the BASIC environment
  loaded.
- **Normative behaviour:**
  - MUST stop the current running program.
  - SHOULD preserve loaded program text in memory.
  - MUST return to a ready prompt (e.g. `READY.`).

### Hard Reset

- **Shortcut:** `Ctrl+Shift+R` (recommended) or `F12`
- **Meaning:** reset the virtual machine/session to a clean boot-like state.
- **Normative behaviour:**
  - MUST stop the current running program.
  - MUST clear any queued audio/TTS utterances.
  - SHOULD clear runtime variables.
  - SHOULD reset graphics/audio subsystems to their initial state.
  - MAY clear program text (must be documented by the implementation).

### Break (Interrupt)

- **Shortcut:** `Esc` (recommended)
- **Meaning:** interrupt program execution (like a classic `BREAK` key).
- **Normative behaviour:**
  - MUST interrupt execution as soon as possible.
  - MUST return control to the user.
  - SHOULD show an interrupt message (e.g. `BREAK IN 150`), if line context exists.

---

## Text-to-Speech (TTS) Control Shortcuts

These mirror the `SAYSTOP` / `SAYFLUSH` BASIC commands.

| Action | Shortcut | Normative behaviour |
|--------|---------|---------------------|
| Stop speech immediately | `Ctrl+.` | MUST stop the current utterance immediately and clear the TTS queue (equivalent to `SAYSTOP`) |
| Flush queued speech | `Ctrl+Shift+.` | MUST NOT interrupt the current utterance; MUST clear the pending queue (equivalent to `SAYFLUSH`) |

---

## F11 -- Toggle Fullscreen (Details)

**Development / testing (desktop OS):**
Pressing `F11` toggles between windowed and fullscreen mode. Use only when running RetroWave 4M
inside a desktop environment during development or testing.

**Standalone system (Raspberry Pi final release):**
RetroWave 4M always runs fullscreen on Raspberry Pi 4, Pi 400, or newer. In this mode, `F11`
has no function and is unassigned.

> End users on the standalone/appliance build should ignore this shortcut.

---

## Notes for Raspberry Pi 400

- Some keyboards map `F12` or `Pause` differently; implementations SHOULD allow remapping via a
  configuration file.
- For kiosk-like usage, implementations MAY disable OS-level shortcuts (like `Alt+F4`) and
  provide an in-app alternative.
- `F9` and `F10` provide screen-reader-style accessibility on all supported keyboards.

---

## Cross-References

- [UTILITIES.md](./UTILITIES.md) -- full description of each F-key utility.
- [ACCESSIBILITY.md](./ACCESSIBILITY.md) -- accessibility shortcuts, VOICE ON behaviour.
- [BASIC_LANGUAGE.md](./BASIC_LANGUAGE.md) -- `SAYSTOP`, `SAYFLUSH`, `SAYSTATUS`, `SAYQUEUE`.
