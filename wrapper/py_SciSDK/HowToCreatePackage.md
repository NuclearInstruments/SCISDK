# How to create python package

## Requirements

```bash
pip install hatchling
pip install twine
```

## Compile Package

From folder py_SciSDK run:

```bash
python -m build
twine check dist/*
twine upload dist/*
```

## Test

Create a virtual environment and install the package:

From console:
```bash
python -m venv venv
venv\Scripts\activate.bat
pip install scisdk
```

From powershell:
```bash
python -m venv venv
venv\Scripts\Activate.ps1
pip install scisdk
```