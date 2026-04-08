# Contributing to RetroWave 4M

Thank you for your interest in contributing! This document explains how to get involved.

---

## Code of Conduct

Be respectful and constructive. All contributors are expected to follow basic community
standards: no harassment, no discriminatory language, and a genuine focus on making
RetroWave 4M better.

---

## How to Contribute

### Reporting Bugs

1. Check the existing [Issues](https://github.com/GeneSmelling/RetroWave-4M/issues) to see if
   the bug has already been reported.
2. Open a new issue with:
   - A clear title.
   - Steps to reproduce the problem.
   - Expected behaviour vs actual behaviour.
   - Hardware and OS version (e.g. Raspberry Pi 400 / Raspberry Pi OS 64-bit).
   - Any relevant log output or screenshot.

### Suggesting Features or Changes

1. Open an issue with the label `enhancement`.
2. Describe the feature, why it is needed, and how it fits the project scope (see
   [VISION.md](./VISION.md) for non-goals).
3. For changes that touch the **BASIC runtime** or **security model**, an explicit community
   RFC discussion is required before implementation.

### Submitting Code

1. Fork the repository and create a feature branch from `main`:
   ```bash
   git checkout -b feature/my-feature
   ```
2. Make your changes following the guidelines below.
3. Run the test suite:
   ```bash
   pytest
   ```
4. Open a Pull Request against `main`. The PR description must:
   - Explain what changes were made and why.
   - Reference any related issues (e.g. `Closes #42`).
   - Confirm that `changed_files > 0` (do not open empty PRs).

---

## Development Guidelines

### Language and Style

- All documentation and code comments must be written in **clear English**.
- Python code follows PEP 8.
- BASIC examples use **line numbers** and the syntax defined in
  [BASIC_LANGUAGE.md](./BASIC_LANGUAGE.md).

### Documentation

- When adding or changing a feature, update the relevant Markdown documentation file.
- Cross-link related documents where helpful (see the existing docs for examples).
- Do not add content that contradicts the non-goals listed in [VISION.md](./VISION.md)
  without opening a discussion first.

### Security

- Do not introduce machine code execution paths.
- All file I/O must remain sandboxed (no system directories, no executables, 200 MB cap).
- All SQLite queries must use bind parameters; no string interpolation into SQL.
- All network parsers must be conservative (whitelist-based; no script execution).
- Review [ARCHITECTURE.md](./ARCHITECTURE.md) (Threat Model section) before touching security-
  sensitive code.

### Accessibility

- Any new utility or companion app must be fully keyboard-navigable and TTS-compatible.
- Do not add UI elements that require a mouse.
- TTS labels must be provided for all interactive widgets.

---

## Scope Boundaries

Before submitting, verify that your contribution is within scope:

- See [VISION.md -- Non-Goals](./VISION.md#non-goals) for a list of things we will not build.
- Key non-goals: Chrome-like browser, full GIMP/Blender parity, real multitasking, untrusted
  third-party plug-ins, modern shader pipelines.

---

## Setting Up for Development

See [SETUP.md](./SETUP.md) for full installation instructions.

```bash
git clone https://github.com/GeneSmelling/RetroWave-4M.git
cd RetroWave-4M
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
pytest
```

---

## Questions

Open an issue or start a discussion in the GitHub Discussions tab.
