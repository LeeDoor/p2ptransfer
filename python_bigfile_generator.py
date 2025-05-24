#!/usr/bin/env python3
import argparse
import os

def create_file(file_size_bytes, filename='big_data.txt'):
    """Create a file with repeating pattern content of the specified size."""
    try:
        # Define a repeating pattern (you can modify this)
        pattern = b"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" * 100  # ~3.6KB pattern
        
        with open(filename, 'wb') as f:
            remaining_bytes = file_size_bytes
            pattern_length = len(pattern)
            
            while remaining_bytes > 0:
                chunk = pattern[:min(pattern_length, remaining_bytes)]
                f.write(chunk)
                remaining_bytes -= len(chunk)
                print(f"\rProgress: {100 * (file_size_bytes - remaining_bytes) / file_size_bytes:.1f}%", end='')
        
        print(f"\nFile '{filename}' created successfully with size {os.path.getsize(filename)} bytes.")
    except Exception as e:
        print(f"\nError creating file: {e}")

def parse_size(size_str):
    """Parse human-readable size string (e.g., '10MB', '1GB') to bytes."""
    size_str = size_str.upper().strip()
    if size_str.isdigit():
        return int(size_str)
    
    units = {'B': 1, 'KB': 1024, 'MB': 1024**2, 'GB': 1024**3, 'TB': 1024**4}
    for unit, multiplier in units.items():
        if size_str.endswith(unit):
            try:
                num = float(size_str[:-len(unit)])
                return int(num * multiplier)
            except ValueError:
                break
    raise ValueError(f"Invalid size format: {size_str}. Use formats like '100MB', '1GB', or '500000' (bytes)")

def main():
    parser = argparse.ArgumentParser(description='Generate a file with repeating pattern content of specified size.')
    parser.add_argument('size', type=str, 
                       help='Size of the file to generate (e.g., "10MB", "1GB", "500000" for bytes)')
    parser.add_argument('--output', '-o', type=str, default='big_data.txt',
                       help='Output filename (default: big_data.txt)')
    
    args = parser.parse_args()
    
    try:
        file_size_bytes = parse_size(args.size)
        print(f"Generating file '{args.output}' with size {args.size} ({file_size_bytes} bytes)...")
        create_file(file_size_bytes, args.output)
    except ValueError as e:
        print(f"Error: {e}")
        parser.print_help()

if __name__ == '__main__':
    main()
