import shutil
import subprocess
import os
from pathlib import Path

npm_path = shutil.which("npm")
build_dir = Path.cwd()
psvmjs_dir = (build_dir / "../subprojects/psvmjs").resolve()
qjsc_executable = (build_dir / "../subprojects/quickjs/qjsc")

# Bundle the JS project
subprocess.Popen([npm_path, "i"], cwd = psvmjs_dir)
subprocess.Popen([npm_path, "run build"], cwd = psvmjs_dir)

# Compile to bytecode
subprocess.Popen([qjsc_executable, "-c" "-o" "psvmjs.c" "dist/globalize.js"], cwd = psvmjs_dir)