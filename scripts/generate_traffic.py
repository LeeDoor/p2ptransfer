import socket
import time
import random
import string
import threading
import argparse

def generate_random_data(length=100):
    """Generate random string data of specified length"""
    return ''.join(random.choices(string.ascii_letters + string.digits, k=length))

def spam_connection(host, port, interval, max_packets=None, data_length=100):
    """Connect to specified host:port and send random data"""
    packets_sent = 0
    
    try:
        while True:
            if max_packets is not None and packets_sent >= max_packets:
                break
                
            try:
                with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                    s.connect((host, port))
                    data = generate_random_data(data_length)
                    s.sendall(data.encode())
                    print(f"Sent: {data[:50]}...")  # Print first 50 chars of data
                    packets_sent += 1
                    
            except ConnectionRefusedError:
                print(f"Connection refused - is the server running on {host}:{port}?")
                break
            except Exception as e:
                print(f"Error: {e}")
                break
                
            if interval > 0:
                time.sleep(interval)
                
    except KeyboardInterrupt:
        print("\nSpamming stopped by user")
    
    print(f"Total packets sent: {packets_sent}")

def main():
    parser = argparse.ArgumentParser(description="TCP data spammer to localhost:8080")
    parser.add_argument("--host", default="localhost", help="Target host (default: localhost)")
    parser.add_argument("--port", type=int, default=8080, help="Target port (default: 8080)")
    parser.add_argument("--interval", type=float, default=0.1, 
                        help="Interval between packets in seconds (default: 0.1)")
    parser.add_argument("--max", type=int, default=None, 
                        help="Maximum number of packets to send (default: unlimited)")
    parser.add_argument("--length", type=int, default=100, 
                        help="Length of random data to send (default: 100)")
    parser.add_argument("--threads", type=int, default=1, 
                        help="Number of concurrent threads (default: 1)")
    
    args = parser.parse_args()
    
    print(f"Starting spammer to {args.host}:{args.port}")
    print(f"Interval: {args.interval}s, Data length: {args.length}, Threads: {args.threads}")
    if args.max:
        print(f"Maximum packets: {args.max}")
    
    threads = []
    for i in range(args.threads):
        t = threading.Thread(
            target=spam_connection,
            args=(args.host, args.port, args.interval, args.max, args.length)
        )
        t.start()
        threads.append(t)
    
    try:
        for t in threads:
            t.join()
    except KeyboardInterrupt:
        print("\nStopping all threads...")

if __name__ == "__main__":
    main()
