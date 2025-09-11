#!/usr/bin/env python3
import os
import sys
import subprocess
import glob

def run_scripts():
    args = sys.argv[1:]
    sh_scripts = sorted(glob.glob("*.sh"))
    
    if not sh_scripts:
        print("No .sh scripts found in current directory")
        return 1
    
    print(f"Found {len(sh_scripts)} .sh scripts: {', '.join(sh_scripts)}")
    print(f"Arguments to pass: {args}")
    print("-" * 50)
    
    for script in sh_scripts:
        base_name = os.path.splitext(script)[0]
        output_file = f"{base_name}.o"
        expected_file = f"{base_name}.i"
        
        print(f"Processing: {script}")
        print(f"  Output file: {output_file}")
        print(f"  Expected file: {expected_file}")

        try:
            with open(output_file, 'w') as f_out:
                process = subprocess.Popen(
                    ['bash', script] + args,
                    stdout=f_out,
                    stderr=subprocess.STDOUT,
                    text=True
                )
                process.wait()
            
            return_code = 0
            if process.returncode != 0:
                print(f"  ⚠ Script exited with code: {process.returncode}")
                return_code = process.returncode
            
            if os.path.exists(expected_file):
                try:
                    result = subprocess.run(
                        ['diff', '-u', output_file, expected_file],
                        capture_output=True,
                        text=True
                    )
                    
                    if result.returncode != 0:
                        print("  ✗ Output differs from expected result")
                        if result.stdout:
                            print("  Differences:")
                            for line in result.stdout.split('\n'):
                                if line:
                                    print(f"    {line}")
                        return return_code
                except Exception as diff_error:
                    print(f"  Error during comparison: {diff_error}")
                    return return_code
            else:
                print("  ⓘ No expected output file found for comparison")
                return return_code
        except Exception as e:
            print(f"  ❌ Failed to execute script: {e}")
            return 1
        

if __name__ == "__main__":
    run_scripts()
