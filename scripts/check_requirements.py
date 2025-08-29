#!/usr/bin/env python3

import subprocess
import sys

def check_dependency(dep):
    """Check if a dependency is available using 'which' command"""
    try:
        # Run 'which' command and capture output
        result = subprocess.run(['which', dep], 
                              capture_output=True, 
                              text=True, 
                              check=True)
        return True
    except subprocess.CalledProcessError:
        print(f"✗ Error: {dep} not found in PATH")
        return False
    except FileNotFoundError:
        print(f"✗ Error: 'which' command not available (unexpected)")
        return False

def main():
    # Add your dependencies here
    dependencies = ["cmake", "make", "g++", "gcc"]
    
    missing_deps = []
    
    for dep in dependencies:
        if not check_dependency(dep):
            missing_deps.append(dep)
    
    if missing_deps:
        print(f"\nError: Missing {len(missing_deps)} dependencies:")
        for dep in missing_deps:
            print(f"  - {dep}")
        print("\nPlease install the missing dependencies and try again.")
        sys.exit(1)
    else:
        sys.exit(0)

if __name__ == "__main__":
    main()
