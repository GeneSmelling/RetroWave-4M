# Keyboard Shortcuts

This document defines the normative keyboard shortcuts for RetroWave-4M.

> Note: Shortcuts are specified at the project level even if current code does not yet implement them.

## Conventions

- **Ctrl** means Control key.
- **Shift** means Shift key.
- **Alt** means Alt/Option key.
- Keys are written in U.S. keyboard naming (e.g. `Esc`, `Enter`, `Backspace`).
- "MUST" / "SHOULD" are used in the RFC 2119 sense.

## Global shortcuts

These shortcuts MUST work regardless of the current screen (splash, BASIC editor, running program, etc.), unless stated otherwise.

| Action | Shortcut | Normative behavior |
|---|---|---|
| Quit / Exit | `Alt+F4` (window managers), or window close button | MUST exit cleanly (release audio/video resources). |
| Pause/Resume | `Pause` | SHOULD toggle pause if a pause system exists; otherwise MAY be unassigned. |
| Screenshot | `F9` | SHOULD save a screenshot to a timestamped file (implementation-defined path). |
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