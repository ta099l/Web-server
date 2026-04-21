#!/usr/bin/env python3

import os
import sys

sys.stdout.write("Content-Type: text/plain\r\n\r\n")

print("CGI executed successfully!\n")

print("Request Method:", os.environ.get("REQUEST_METHOD"))
print("Content Length:", os.environ.get("CONTENT_LENGTH"))
print("Script Name:", os.environ.get("SCRIPT_NAME"))