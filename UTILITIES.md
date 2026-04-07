# Utilities -- RetroWave 4M

Utilities are programs launched via function keys. They form the core interactive shell of
RetroWave 4M: the function keys F1--F12 are the application launcher and navigation layer.

All utilities run in **text/accessible mode** unless otherwise noted. All are designed to work
fully without a monitor (screen-reader / TTS mode), with keyboard-only navigation.

> **Companion apps vs utilities:** Graphics-intensive tools (Sprite Editor, 3D Editor, Paint
> Editor, DAW) are provided as **separate companion applications** launched from BASIC or the
> file manager. Loading them as resident utilities would be too costly in memory. See
> [COMPANION_APPS.md](./COMPANION_APPS.md).

---

## Print Screen (STAMP) -- Screenshot

Active in both text mode and graphics mode. Captures the current screen and saves it as a
timestamped PNG file in the `screenshots/` directory. No menu required; triggers instantly.

---

## F1 -- Help (Instruction Manual)

An accessible, multilingual instruction manual. Text mode only.

When opened, Help loads in the language configured in F2 Preferences. The same language is used
for TTS voice output. When no monitor is connected, the voice reads screen content and all
selectable items, making it possible to learn BASIC -- and RetroWave 4M in general -- entirely
without a display.

### Contents

- Map and description of all function keys and keyboard shortcuts.
- Detailed description of all function-key utilities.
- Alphabetical list of all BASIC commands and functions (selectable; read aloud by TTS).
- Beginner BASIC course with explanations and examples.
- Advanced BASIC course with explanations and examples.
- RetroWave 4M release notes and version history.
- Frequently Asked Questions (origin, goals, scope, non-goals).
- Online support link.

> The manual may also be published in print (e.g. via Amazon) for offline reference.

---

## F2 -- Options and Startup Preferences

General system preferences. Text/accessible mode.

- Default language for Help, utilities, and TTS at startup.
- Voice at startup: enable/disable; language; gender (male/female). Voice is only enabled by
  default if no monitor is connected; users with monitors can enable it explicitly.
- Text-mode colour scheme: Classic C64, Classic C16, Green Phosphor, Night Mode, High Contrast,
  Custom.
- Printer configuration.
- Login settings (if required).
- Backup configuration (destination, schedule, integrity checks).
- Other system preferences.

---

## F3 -- File Manager

Midnight-Commander style file manager. Text/accessible mode. Full keyboard navigation.

### Capabilities

- **File operations:** copy, move, delete (to Trashcan) one or more selected files.
- **Directory operations:** create, copy, move, delete (to Trashcan).
- **Viewers:** text files, JPEG/PNG/BMP images, sprites.
- **Media playback:** MP4 video (H.264), MP3 and WAV audio.
- **Text editor:** create and edit plain text files (accessible, TTS-compatible).
- **BASIC files:** load or edit BASIC source files (alternative to the `LOAD` command).
- **Backup:** backup to USB pen drive or network share with integrity verification.
- **FTP/SFTP:** upload and download files via FTP or SFTP (passive mode supported).
- **Trashcan:** deleted files are moved to Trashcan; permanent deletion requires a second step.

---

## F4 -- Music Utility (Tracker)

A simplified text/accessible DAW. No mouse required. Compose, load, save, and export music.

### Instrument panel

Select a soundfont from the bundled library, or build a synth sound from oscillators (virtual
analog, FM, granular) with ADSR envelope, filters, and effects. Multiple instruments per track.

### Tracker grid

Tabular layout (classic tracker style). Each channel column lists: note, instrument, duration,
optional rest; channel volume and global BPM.

Optional features: chords, portamento, fade in/out, microtones.
External WAV and MP3 files can be mixed alongside synthesised tracks.

### Export

- BASIC `TRACK` commands for embedding in a BASIC listing (played with `TRACK PLAY id`).
- WAV audio file.

### Preferences

- Master volume; tuning (default A = 440 Hz); other options.

---

## F5 -- Network

Configuration and management of internet connectivity. Text/accessible mode.

- **USB Tethering:** activation, configuration (login, password), connect/disconnect.
- **Mesh network:** activation and configuration; encryption enabled by default.
- Network status display.

---

## F6 -- Crypto Wallet

Management of cryptocurrency wallets and security. Text/accessible mode.

- Wallet creation, import, and management.
- Key storage: local encrypted store; no key export over network.
- Security level configuration.
- **Litecoin mining** (optional):
  - Disabled by default; requires explicit user consent.
  - CPU share cap: configurable, default 25%.
  - Real-time energy-use indicator.
  - Mining auto-pauses on battery power.
  - Ecology impact prominently disclosed to the user.

---

## F7, F8 -- Reserved

These function keys are reserved for future utilities or implementation-defined use.

---

## F9 -- Accessibility: Cursor Position and Cell Content

Reads aloud (in the selected language) the current cursor coordinates and the content of the
cell at that position. In graphics mode, reads the current graphics mode and coordinates instead.

Available at any time; in any mode. Functions as a screen-reader shortcut.

---

## F10 -- Accessibility: Read Line

Reads aloud (in the selected language) the content of the current line.

Available at any time; in any mode. Functions as a screen-reader shortcut.

---

## F11 -- Toggle Fullscreen (Development / Testing Only)

In a desktop OS (development/testing), toggles between windowed and fullscreen mode.

On the standalone Raspberry Pi release, the system is always fullscreen; F11 is unassigned.

---

## F12 -- Restart / Shutdown

Prompts the user to restart or shut down the system. Equivalent to a hard reset.

Pressing `F12` confirms the action after a prompt; accidental single keypresses are not acted on
immediately.

---

## Bundled Companion Applications (not F-key utilities)

The following tools are bundled with RetroWave 4M but are not loaded as resident utilities.
Launch them from BASIC or from the F3 file manager.

| Application | Description |
|-------------|-------------|
| **Text Editor** | Standalone accessible plain-text editor; TTS-compatible; gap-analysis recommended. |
| **Spreadsheet** | Spreadsheet-like tables with CSV import/export and formulas; basic charting (bar, line, pie). |
| **Presentation** | Lightweight linear slide-by-slide presentation mode; keyboard-navigated. |
| **Text Web Browser** | Lynx-like; no JavaScript; no media auto-play; TTS-compatible. |
| **Email Client** | Text-based send/receive; plain text only. |
| **Graphical DAW** | LMMS-like, lightweight; timeline, piano roll, instrument rack; exports WAV and MP3. |
| **Sprite Editor** | Visual sprite creation and editing; map editor. |
| **3D Editor** | 3D scene composition; OBJ import; heightfield and wormhole/tunnel tools. |
| **Paint Editor** | Lightweight raster paint app; GIMP-like but limited scope; not full GIMP parity. |
| **3D Game Demo** | Demonstration game showcasing the 3D and sound capabilities. |

See [COMPANION_APPS.md](./COMPANION_APPS.md) for full descriptions.
