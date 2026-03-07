# Development Setup and Installation Instructions

## Prerequisites
- Python 3.10 or higher (<https://www.python.org/>)
- `pip` (comes with Python)
- Recommended: a virtual environment tool (`venv` or `virtualenv`)

## Installation Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/GeneSmelling/RetroWave-4M.git
   cd RetroWave-4M
   ```

2. Create and activate a virtual environment (recommended):
   ```bash
   python -m venv venv
   # On Windows:
   venv\Scripts\activate
   # On macOS/Linux:
   source venv/bin/activate
   ```

3. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```

## Running the Project
```bash
python main.py
```

## Running Tests
```bash
pytest
```

## Additional Info
- `RPi.GPIO` in `requirements.txt` is only needed if running on a Raspberry Pi; it can be skipped on standard desktop systems.