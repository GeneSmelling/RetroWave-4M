# Utilities

Utilities are programs invoked via function keys. They are designed to be feature-rich, as they are a central part of the RetroWave-4M experience — a world to discover.

Utilities run in **text/accessible mode** unless otherwise noted, and are designed to work without a monitor (screen reader / TTS mode).

> **Note on bundled applications:** Graphics-intensive tools such as the Sprite Editor and the 3D Editor are provided as **separate bundled applications** (not function-key utilities). Loading them as resident utilities would be too costly in terms of memory.

---

## Screenshot — `Print Screen` key

Active in both text mode and graphics mode. Captures the current screen and automatically saves it as a PNG file in a dedicated directory.

---

## F1 — Help (Instruction Manual)

An accessible, multilingual instruction manual. Works in **text mode only**.

When opened, Help loads in the language set in Preferences. The same language is used for the voice, both when voice support is enabled and when no monitor is connected. In the latter case, the voice will read out the screen content and any selectable items. This makes it possible to learn BASIC — and RetroWave-4M in general — entirely without a monitor.

### Contents

- Map and description of function keys and keyboard shortcuts
- Detailed description of all function-key utilities
- Alphabetical list of all BASIC commands and functions (selectable)
- Beginner BASIC course with explanations and examples
- Advanced BASIC course with explanations and examples
- RetroWave-4M release notes and version updates
- Frequently Asked Questions (origin of the project, goals, etc.)
- Online support

> The manual may also be published in print (e.g. via Amazon) for convenient offline reference.

---

## F2 — Options and Startup Preferences

General system preferences, configured in text/accessible mode:

- Default language for Help, utilities, and TTS at startup
- Voice at startup: enable/disable, language, gender (male/female) — only if a monitor is connected and the option is desired
- Text mode colour scheme at startup: Classic C64, Classic C16, Green phosphor, Night mode, High contrast, Custom
- Printer configuration
- Login settings (if required)
- Backup configuration
- Other

---

## F3 — File Manager

Similar to Midnight Commander. Includes a text viewer and text editor. Capabilities:

- Basic file operations: copy, move, delete (to Trash) one or more selected files
- Create, copy, move, delete directories (to Trash)
- Read text files
- Create and/or edit text files
- Display JPEG, PNG, BMP images
- Display sprites
- Play MP4 video (H.264)
- Play MP3 and WAV audio
- Load BASIC files (as an alternative to the `LOAD` command)
- Edit BASIC files (as an alternative to the text editor mode); the editor can also be launched from within, but saves the current listing first
- Backup system (e.g. to USB pen drive)

---

## F4 — Music Utility

A simplified text/accessible DAW, operable without a mouse. Allows composing, loading, and saving music, with export either as a WAV file or as BASIC commands (track definition) that can be integrated into a BASIC listing and played with `PLAY`, paused with `PAUSE`, and stopped with `STOP`.

### Sections

#### Instrument
Starting from the waveform. Some instruments may be too complex to synthesise from scratch; the waveform can therefore be:
- A soundfont from the available library
- A complex synthesis sound, available as a preset or built by editing the waveform via oscillators

Parameters: ADSR envelope, additional filters, effects (echo, reverb). Multiple instruments can be defined for the same track.

#### Tracker
Music is defined in tabular form, like a classic tracker. Each channel has its own column listing:
- Sound, selected instrument, note, duration, and optional rests
- Channel volume (Mixer) and BPM

Chords can be selected directly in addition to individual notes (more oscillator-intensive, but lighter on filters). Optional features: portamento, fade in/out, microtones. Playback available at any time.

External WAV and MP3 files can be included alongside or in place of synthesised tracks, up to covering the full length of the piece.

#### Preferences
- Master volume control
- Tuning (default A=440 Hz; configurable)
- Other options

#### Export
The piece can be saved at any stage. Export options:
- As a series of BASIC commands defining tempo, pitch, instruments, music, and mixer
- As a WAV file

---

## F5 — Network

Configuration and management of internet connectivity, in text/accessible mode:

- USB Tethering: activation, configuration, login, password, connect/disconnect
- Mesh network: activation and configuration (encryption enabled by default)

---

## F6 — Crypto Wallet

Management of cryptocurrency wallets and their security levels, with additional features specific to RetroWave-4M:

- Wallet management and security configuration
- Litecoin mining (requires configuration and active internet connection)

---

## F9 — Accessibility: Cursor Position and Cell Content

Reads aloud (in the selected language) the current cursor coordinates and the content of the cell at that position. In graphics mode, reads the current graphics mode instead.

---

## F10 — Accessibility: Read Line

Reads aloud (in the selected language) the content of the current line.

---

## F12 — Restart / Shutdown

Initiates a system restart or shutdown.

---

## Bundled Applications (not function-key utilities)

The following tools are provided as **separate applications** distributed alongside RetroWave-4M. They are not loaded as resident utilities in order to avoid excessive memory usage.

| Application | Description |
|---|---|
| **Sprite Editor** | Visual editor for creating and editing sprites used in BASIC programs. |
| **3D Editor** | Editor for composing and previewing 3D scenes and objects.