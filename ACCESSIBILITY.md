# Accessibility -- RetroWave 4M

RetroWave 4M is built on an **accessibility-by-default** philosophy: every core feature must be
usable even when no monitor is attached. Accessibility is not an add-on; it is part of the
fundamental design.

---

## Core Principles

1. **Works without a monitor.** All utilities and the BASIC environment are fully usable in
   headless (monitor-free) mode.
2. **Automatic voice mode.** If no monitor is detected at boot, or if the monitor is disconnected
   while the system is running, TTS (Text-to-Speech) is automatically enabled.
3. **Keyboard-only navigation.** Every function can be reached and operated using only the
   keyboard. No mouse or touchpad is ever required.
4. **High-contrast colour schemes.** At startup (or via F2 Options), users choose from: Classic
   C64, Classic C16, Green Phosphor, Night Mode, High Contrast, or Custom.
5. **Affordable and energy-efficient.** Raspberry Pi 4/400 hardware keeps costs and power
   consumption low.

---

## VOICE ON Behaviour

- **`VOICE ON`** -- enables system-wide speech feedback.
- **`VOICE OFF`** -- disables speech feedback.
- Also configurable via `F2` Options (language, voice gender: male/female, enable/disable).

### What is spoken

| Context | Spoken content |
|---------|---------------|
| `PRINT` output | User string in the selected language |
| `INPUT` prompt | Prompt text in the selected language |
| `LIST` output | BASIC keywords in English; string literals in the selected language |
| Menus | All menu items and selections |
| Utilities (F1--F12) | All text, prompts, and selectable items |
| Keypress feedback | Key name or character (when enabled in Options) |
| Error messages | Error text in the selected language |

### Auto-voice (headless mode)

When the system boots without a monitor, or when a monitor is disconnected:

1. TTS is automatically activated (equivalent to `VOICE ON`).
2. The selected language from F2 Options is used.
3. The system reads the boot screen, the READY prompt, and all subsequent output.
4. The user can fully navigate menus, run BASIC programs, and use all utilities by ear.

---

## Accessibility Shortcuts

| Key | Function |
|-----|---------|
| `F9` | Reads aloud the current cursor coordinates and the content of the current cell (text mode), or the current graphics mode (graphics mode) |
| `F10` | Reads aloud the entire content of the current line |
| `Ctrl+.` | `SAYSTOP` -- stop speech immediately and clear the TTS queue |
| `Ctrl+Shift+.` | `SAYFLUSH` -- clear the pending TTS queue without interrupting the current utterance |

---

## Accessible Utilities (F1--F12)

All function-key utilities run in text/accessible mode. None require a mouse or complex graphics.

### F1 -- Help (Instruction Manual)

- Fully voice-driven; works without a monitor.
- When no monitor is connected, Help reads out screen content and all selectable items.
- Enables complete self-learning of BASIC and the system entirely by ear.
- Contents: keyboard map, function-key descriptions, BASIC command reference, beginner and
  advanced BASIC courses, FAQ, release notes, online support.

### F2 -- Options and Preferences

- Set language, speech voice, gender, and enable/disable voice at startup.
- Choose colour scheme (for when a monitor is available).
- Configure backup, printer, login settings.

### F3 -- File Manager

- Midnight-Commander style; full keyboard navigation.
- All file operations (copy, move, delete to Trashcan) accessible by keyboard + TTS.

### F9 / F10 -- Screen Reader Shortcuts

- Instant TTS feedback at any time, in any mode.
- Designed to work like screen-reader commands on standard accessibility tools.

---

## Text-Based Web Browsing

- The text browser companion app (Lynx-like) renders web pages as plain text.
- No graphical browser is provided (no Chrome-like browser).
- TTS reads page content aloud; all navigation is keyboard-based.
- Safe even without a monitor: web content is accessible entirely by audio.

---

## Additional Accessibility Notes

- **Web radio and streaming** work without a display (audio-only operation).
- **Monotasking** reduces cognitive load: there is always exactly one active context, no
  overlapping windows, no hidden background apps.
- **Minimal widget set** (buttons, labels, text inputs, checkboxes, sliders, dropdowns,
  scrollable panels) ensures all UI elements have clear keyboard focus and TTS labels.
- Any future utility or companion app must follow the same accessibility guidelines: full
  keyboard navigation, TTS-compatible labels, no mouse-only interactions.

---

## WCAG and Standards Alignment

- Colour contrast targets are aligned with WCAG 2.1 AA (minimum 4.5:1 for text).
- High-contrast mode achieves AAA contrast ratios.
- All images and sprites exposed to the user interface carry alternative text descriptions
  accessible via TTS.
- Keyboard focus order follows a logical, predictable sequence.

---

## Cross-References

- [BASIC_LANGUAGE.md](./BASIC_LANGUAGE.md) -- `TTS`, `SAYSTOP`, `SAYFLUSH`, `SAYSTATUS`,
  `SAYQUEUE` commands; `VOICE ON/OFF`.
- [SOUND.md](./SOUND.md) -- TTS engine details, queue semantics, Piper and eSpeak NG.
- [KEYBOARD.md](./KEYBOARD.md) -- F9/F10 shortcuts; `Ctrl+.` / `Ctrl+Shift+.`; all key bindings.
- [UTILITIES.md](./UTILITIES.md) -- accessible behaviour of each F-key utility.
- [VISION.md](./VISION.md) -- accessibility goals; Neoism; non-goals.
