#!/usr/bin/env python3
"""Small local helper: Python is tooling only; the HTTP backend remains C."""
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]
subprocess.run(["make", "build"], cwd=ROOT, check=True)
subprocess.run([str(ROOT / "bin" / "finadvisor")], cwd=ROOT, check=True)
