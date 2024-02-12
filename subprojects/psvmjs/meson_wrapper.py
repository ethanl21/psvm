import shutil
import subprocess
from pathlib import Path
import sys

def main():
    npm_path = shutil.which("npm")
    psvmjs_src_dir = (Path(sys.argv[1]) / "js_src").resolve()
    psvmjs_build_dir = Path(sys.argv[2])
    
    qjsc_executable = Path(sys.argv[3])

    # Bundle the JS project
    subprocess.Popen([npm_path, "i"], cwd = psvmjs_src_dir)
    subprocess.Popen([npm_path, "run", "build"], cwd = psvmjs_src_dir)

    # Compile to bytecode
    subprocess.Popen([qjsc_executable, "-c", "-o", (psvmjs_build_dir / "psvmjs.c").resolve(), "dist/globalize.js"], cwd = psvmjs_src_dir)


if __name__ == "__main__":
    main()