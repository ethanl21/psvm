import shutil
import subprocess
from pathlib import Path

npm_path = shutil.which("npm")
build_dir = Path.cwd()

psvmjs_src_dir = (build_dir / "../subprojects/psvmjs/js_src").resolve()
psvmjs_build_dir = (build_dir / "subprojects/psvmjs").resolve()

qjsc_executable = (build_dir / "subprojects/quickjs/qjsc").resolve()

# Bundle the JS project
subprocess.Popen([npm_path, "i"], cwd = psvmjs_src_dir)
subprocess.Popen([npm_path, "run", "build"], cwd = psvmjs_src_dir)

# Compile to bytecode
subprocess.Popen([qjsc_executable, "-c", "-o", (psvmjs_build_dir / "psvmjs.c").resolve(), "dist/globalize.js"], cwd = psvmjs_src_dir)