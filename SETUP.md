# Setup and Installation -- RetroWave 4M

---

## Prerequisites

- **Hardware:** Raspberry Pi 4 (2 GB RAM or more recommended) or Raspberry Pi 400.
  GPIO features require Raspberry Pi hardware; the system also runs on any Linux desktop for
  development.
- **Python:** 3.10 or higher.
- **pip:** included with Python.
- **Recommended:** a Python virtual environment (`venv`).

---

## Installation Steps

### 1. Clone the repository

```bash
git clone https://github.com/GeneSmelling/RetroWave-4M.git
cd RetroWave-4M
```

### 2. Create and activate a virtual environment

```bash
python -m venv .venv
source .venv/bin/activate   # On Windows: .venv\Scripts\activate
```

### 3. Install dependencies

```bash
pip install -r requirements.txt
```

> **Note:** `RPi.GPIO` in `requirements.txt` is only needed on Raspberry Pi hardware.
> It is commented out by default. Uncomment it before installing on a Raspberry Pi.

### 4. (Optional) Install Piper TTS voice models

For offline TTS (Text-to-Speech), download one or more Piper voice models and place them in the
`voices/` directory. The system will fall back to eSpeak NG if no Piper model is found.

Example (English, medium quality):

```bash
mkdir -p voices
# Download model files from the Piper releases page and place them in voices/
```

### 5. (Optional) Install SoundFont instruments

Place SF2 files in the `soundfonts/` directory. Register them in `soundfonts/index.json`.
Custom user soundfonts can be placed in `soundfonts/user/` and are auto-discovered at startup.

---

## Running the Project

```bash
python main.py
```

On Raspberry Pi 4/400, the system boots to a full-screen BASIC prompt.
On a desktop OS, it opens in a window (use `F11` to toggle fullscreen during development).

---

## Running Tests

```bash
pytest
```

---

## GPIO Setup (Raspberry Pi Only)

1. Uncomment `RPi.GPIO` in `requirements.txt`.
2. Install:
   ```bash
   pip install RPi.GPIO
   ```
3. GPIO access is sandboxed: only the safe API exposed by RetroWave 4M's GPIO commands is
   available from BASIC. Direct memory access is not permitted.

---

## Standalone / Appliance Setup

For a standalone Raspberry Pi appliance (no desktop OS):

1. Flash Raspberry Pi OS Lite (64-bit) to an SD card.
2. Enable SSH and configure Wi-Fi if needed (`raspi-config`).
3. Follow the installation steps above.
4. Add a `systemd` service or `rc.local` entry to launch `python main.py` at boot.
5. Connect a HDMI display and a USB keyboard (or use in headless/TTS mode).

---

## Accessibility at First Boot

If no monitor is connected when the system first starts:

- Text-to-Speech is automatically enabled.
- The system reads the boot status and the READY prompt aloud.
- Use `F1` (Help) to navigate the manual by audio.
- Use `F2` (Options) to configure language, voice gender, and colour scheme for future use.

---

## Cross-References

- [ARCHITECTURE.md](./ARCHITECTURE.md) -- security model, storage sandbox, GPIO.
- [ACCESSIBILITY.md](./ACCESSIBILITY.md) -- VOICE ON behaviour, headless operation.
- [UTILITIES.md](./UTILITIES.md) -- F1 Help, F2 Options, F3 File Manager.
- [CONTRIBUTING.md](./CONTRIBUTING.md) -- how to contribute to the project.
