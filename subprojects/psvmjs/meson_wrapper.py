import shutil
import subprocess
from pathlib import Path
import sys

def main():
    # Find pnpm or npm
    npm_path = shutil.which("pnpm")
    if npm_path is None:
        npm_path = shutil.which("npm")
    
    psvmjs_src_dir = (Path(sys.argv[1]) / "js_src").resolve()

    # Bundle the JS project
    subprocess.Popen([npm_path, "i"], cwd = psvmjs_src_dir).wait()
    subprocess.Popen([npm_path, "run", "build"], cwd = psvmjs_src_dir).wait()


if __name__ == "__main__":
    main()