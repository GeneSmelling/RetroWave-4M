# Keyboard Shortcuts

This document defines the normative keyboard shortcuts for RetroWave-4M.

> Note: Shortcuts are specified at the project level even if current code does not yet implement them.

## Conventions

- **Ctrl** means Control key.
- **Shift** means Shift key.
- **Alt** means Alt/Option key.
- Keys are written in U.S. keyboard naming (e.g. `Esc`, `Enter`, `Backspace`).
- "MUST" / "SHOULD" are used in the RFC 2119 sense.

## Function Keys — Utility Launcher

The function keys F1–F12 launch the built-in utilities described in `UTILITIES.md`. All utilities run in **accessible text mode** unless noted otherwise.

| Key | Utility | Notes |
|---|---|---|
| `F1` | **Help** — interactive manual | Accessible / voice-driven; works without a monitor. |
| `F2` | **Options & Startup Preferences** | Language, colours, voice, printer, login, backup, etc. |
| `F3` | **File Manager** | Midnight-Commander–style; includes text editor, image viewer, sprite viewer, video/audio player. |
| `F4` | **Music Utility (DAW)** | Tracker-style composer; export to WAV or BASIC commands. |
| `F5` | **Network** | USB tethering and mesh network configuration. |
| `F6` | **Crypto Wallet** | Wallet management and optional Litecoin mining. |
| `F7` | *(reserved / implementation-defined)* | |
| `F8` | *(reserved / implementation-defined)* | |
| `F9` | **Read cursor position** | Reads aloud (in the selected language) the cursor coordinates and the content of the current cell; in graphics mode, reads the current graphics mode. |
| `F10` | **Read current line** | Reads aloud the content of the current line. |
| `F11` | **Toggle fullscreen** | Toggle Fullscreen (development/testing only; see note below) |
| `F12` | **Restart / Shutdown** | Prompts for restart or shutdown. Equivalent to Hard Reset (see below). |
| `Print Screen` (`STAMP`) | **Screenshot** | Active in both text and graphics mode; automatically saves a timestamped PNG to the screenshots directory. |

> **Note:** The **Sprite Editor** and **3D Editor** are separate companion applications, not function-key utilities, so as not to burden system memory.

## Global shortcuts

These shortcuts MUST work regardless of the current screen (splash, BASIC editor, running program, etc.), unless stated otherwise.

| Action | Shortcut | Normative behavior |
|---|---|---|
| Quit / Exit | `Alt+F4` (window managers), or window close button | MUST exit cleanly (release audio/video resources). |
| Pause/Resume | `Pause` | SHOULD toggle pause if a pause system exists; otherwise MAY be unassigned. |
| Screenshot | `Print Screen` | MUST save a screenshot to a timestamped PNG file in the screenshots directory (text and graphics modes). |
| Toggle fullscreen | `F11` | SHOULD toggle between windowed and fullscreen. |

## Reset / Break shortcuts (BASIC runtime)

### Soft Reset

- **Shortcut:** `Ctrl+R`
- **Meaning:** reset the *current program execution context* while keeping the BASIC environment loaded.
- **Normative behavior:**
  - MUST stop the current running program.
  - SHOULD preserve loaded program text in memory.
  - MUST return to a ready prompt (e.g. `READY.`), and reset runtime variables unless an implementation provides a different model.

### Hard Reset

- **Shortcut:** `Ctrl+Shift+R` (recommended) OR `F12`
- **Meaning:** reset the virtual machine/session to a clean boot-like state.
- **Normative behavior:**
  - MUST stop the current running program.
  - MUST clear any queued audio/TTS utterances.
  - SHOULD clear runtime variables.
  - SHOULD reset graphics/audio subsystems to their initial state.
  - MAY clear program text depending on the intended "boot" semantics (must be documented by the implementation).

### Break (Interrupt)

- **Shortcut:** `Esc` (recommended)
- **Meaning:** interrupt program execution (like a classic `BREAK` key).
- **Normative behavior:**
  - MUST interrupt execution as soon as possible.
  - MUST return control to the user.
  - SHOULD show an interrupt message (e.g. `BREAK IN <line>`), if line context exists.

## Text-to-Speech (TTS) control shortcuts

These mirror the `SAYSTOP` / `SAYFLUSH` commands described in the specs.

| Action | Shortcut | Normative behavior |
|---|---|---|
| Stop speech immediately | `Ctrl+.` | MUST stop current utterance immediately and clear the queue (equivalent to `SAYSTOP`). |
| Flush queued speech | `Ctrl+Shift+.` | MUST NOT interrupt current utterance; MUST clear pending queue (equivalent to `SAYFLUSH`). |

## Notes for Raspberry Pi 400

- Some keyboards map `F12` or `Pause` differently; implementations SHOULD allow remapping via a config file.
- For kiosk-like usage, implementations MAY disable OS-level shortcuts (like `Alt+F4`) and provide an in-app alternative.
- `F9` and `F10` provide screen-reader–style accessibility on all supported keyboards.

- ### F11 — Toggle Fullscreen

- **Development/testing (desktop environment):**
  Pressing `F11` toggles between windowed and fullscreen mode. Use this shortcut only when running RetroWave-4M in a desktop OS during development or testing.

- **Standalone system (final release for Raspberry Pi):**
  When RetroWave-4M runs as a stand-alone system on Raspberry Pi 4, Pi 400, or newer, the environment is always fullscreen. In this case, the `F11` key has no function and is unassigned.

> **Note:**  
> F11 is reserved for desktop development only. End users on the stand-alone, appliance build should ignore this shortcut.
