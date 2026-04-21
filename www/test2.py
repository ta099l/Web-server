#!/usr/bin/env python3

import sys
import os

# Output header FIRST (important for CGI)
sys.stdout.write("Content-Type: text/plain\r\n\r\n")

# Where to save the uploaded file
output_path = "uploaded.bin"

# Open file for writing (binary)
with open(output_path, "wb") as f:
    total = 0

    while True:
        chunk = sys.stdin.buffer.read(4096)  # read in chunks
        if not chunk:
            break

        f.write(chunk)
        total += len(chunk)

# Final response
print(f"Upload complete. Received {total} bytes.")