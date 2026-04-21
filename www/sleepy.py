#!/usr/bin/env python3
import time
import sys

# Output header first
sys.stdout.write("Content-Type: text/plain\r\n\r\n")

# Simulate long task by sleeping
print("Sleeping for 5 seconds...")
sys.stdout.flush()
time.sleep(5)
print("Done sleeping!")