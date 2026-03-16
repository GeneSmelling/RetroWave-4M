# Accessibility in RetroWave-4M

RetroWave-4M is designed with an “accessibility by default” philosophy: all core features are usable even when no monitor is attached. The goal is to provide an inclusive experience for everyone, with special care given to blind, visually impaired, and motor-impaired users.

---

## General Principles

- **Works without a monitor:** All main utilities and the system itself can be used even if no display is connected.
- **Automatic audio support:** If a monitor is not detected or gets unplugged during use, speech output (TTS) is automatically enabled, reading screens, lists, menus, and other important content.
- **Keyboard accessibility:** Complete navigation and all operations are possible via keyboard only, with no mouse or touchpad required.
- **High-contrast color schemes:** On startup, users can choose between various high-contrast modes (Classic C64, Night Mode, Green Phosphor, High Contrast, or Custom).
- **Affordable and sustainable:** RetroWave-4M is designed to be low-cost and energy-efficient, accessible for everyone.

---

## Accessible Features & Utilities

- **Help (F1): Accessible Manual**  
  - Works in text mode and even without a monitor.
  - Is read aloud (TTS) in the preferred language.
  - Covers keyboard mapping, BASIC commands, utility index, FAQs, BASIC tutorials—everything can be navigated by keyboard and spoken aloud.
  - Enables full self-learning, even without a screen.

- **Utilities & Interface**  
  - All primary utilities (F1 to F12) are designed for full use with TTS and keyboard.
  - No essential utility relies on complex graphics or requires windowing/multitasking.

- **Screen reader functions**  
  - `F9`: Reads out the current cursor position and the cell’s content (text mode) or the current graphics mode (graphics mode).
  - `F10`: Reads aloud the entire current line.
  - These features are always available for instant feedback.

- **Text-based web browsing**  
  - The built-in browser is text-only (Lynx-inspired), ensuring maximum compatibility with screen readers and safe navigation.
  - Even without a monitor, web navigation is possible using speech output.
  - No graphical/multitasking browser is provided.

- **Accessibility preferences**  
  - Via the Options menu (`F2`), users can set language, speech settings, color theme, and further accessibility parameters.

---

## Cross-References — Where to Find Accessibility Topics

- `VISION.md`: *Accessibility Goals* section (overview and principles)
- `UTILITIES.md`: See Help, Browser, Options, F9/F10, and speech output features in each utility.
- `KEYBOARD.md`: Table of F9/F10 shortcuts (“screen reader style”), keyboard navigation notes.
- `BASIC_LANGUAGE.md`: Sections listing TTS and accessibility commands.
- Every main utility description highlights “no mouse, no windows” for compatibility and focus.

---

## Additional Notes

- **Web radio** and streaming features work even without a display, with audio-only operation.
- Monotasking and absence of windows/complex widgets reduce cognitive load and make the interface ideal for blind and low-vision users.
- Any future integrated utility must follow these same accessibility guidelines.

---

> **Note:** For suggestions, feedback, or requests regarding accessibility, a dedicated support channel is available (see “Support” section in the documentation).
