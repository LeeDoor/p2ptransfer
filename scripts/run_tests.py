#!/usr/bin/env python3
import os
import sys
import subprocess
from pathlib import Path

def main():
    platform = os.uname().sysname
    build_dir = Path(__file__).parent.parent / "build" / platform
    
    if not build_dir.exists():
        print(f"Build directory not found: {build_dir}")
        sys.exit(1)
    
    test_dirs = []
    for test_dir in ["cli", "graphics"]:
        if (build_dir / test_dir).exists():
            test_dirs.append(test_dir)
    
    if not test_dirs:
        print(f"Can't find a directory to run tests here: {build_dir}")
        sys.exit(1)
    
    for test_dir in test_dirs:
        print(f"\nRunning tests in {test_dir}...")
        cmd = [
            "ctest",
            "--test-dir", str(build_dir / test_dir),
            "--timeout", "4",
            "--output-on-failure",
            "--stop-on-failure",
            *sys.argv[1:]
        ]
        
        result = subprocess.run(cmd)
        if result.returncode != 0:
            print(f"\nTests in {test_dir} failed with exit code {result.returncode}")
            sys.exit(result.returncode)
    
    print(f"\nAll tests passed successfully: {test_dirs}")
    sys.exit(0)

if __name__ == "__main__":
    main()
