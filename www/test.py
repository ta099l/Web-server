#!/usr/bin/env python3

import time
import sys

# Required CGI header
sys.stdout.write("Content-Type: text/plain\r\n\r\n")

print("Starting long task...\n")
sys.stdout.flush()

# Simulate long processing (10 seconds total)
for i in range(1, 11):
    print(f"Step {i}/10: working...")
    sys.stdout.flush()
    time.sleep(1)

print("\nDone!")