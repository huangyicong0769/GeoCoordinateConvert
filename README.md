Python C++ binding for CoordinateConvert
======================================

This project exposes the C++ `CoordinateConvert` code to Python using `pybind11`.

How to build
------------

1. Install build dependencies (recommended to use a virtual env). Using `uv` (preferred):

```bash
# Create a uv-managed virtual env (Python 3.13)
uv venv .venv -p 3.13 --seed

# Activate the venv if needed (optional)
source .venv/bin/activate

# Sync project dependencies from pyproject.toml
uv sync

# Add build / dev deps (if missing)
uv add pybind11 setuptools wheel --dev
```

2. Build the extension in-place (using `uv run`).

```bash
uv run python setup.py build_ext --inplace
```

3. Run the example script (again through `uv run`).

```bash
# Run the C++-backed example
uv run python examples/convert_with_cpp.py

# Or run the main script; the script will use the C++ extension if present, otherwise fall back to pyproj
uv run ./main.py
```

Tips
----

- If you get build errors referencing missing headers, ensure you have a suitable compiler toolchain (Xcode command line tools on macOS), or add proper compiler flags in `setup.py` (e.g., `extra_compile_args=["-std=c++14"]`).
- The `uv` commands assume `uv` is installed globally (`pip install uv-cli`). If you prefer, you can install `uv` in the project using:

```bash
python -m pip install --user uv-cli
```

Notes
-----

- The example uses `20030801.xls` as an example data file (same as `main.py`) — place it at the project root or change the path accordingly.
- The C++ code uses latitude/longitude ordering as `Point(x=latitude, y=longitude, z=height)` — check `CoordinateConvert.cpp` for details.
