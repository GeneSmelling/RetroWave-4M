# Development Setup and Installation Instructions

## Prerequisites
- Python 3.10 or higher
- `pip` (included with Python)
- Recommended: virtual environment (`venv`)

## Installation Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/GeneSmelling/RetroWave-4M.git
   cd RetroWave-4M
   ```

2. Create and activate a virtual environment:
   ```bash
   python -m venv .venv
   source .venv/bin/activate  # On Windows: .venv\Scripts\activate
   ```

3. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```
   > **Note:** `RPi.GPIO` in `requirements.txt` is only needed on Raspberry Pi hardware. It is commented out by default.

## Running the Project
```bash
python main.py
```

## Running Tests
```bash
pytest
```
