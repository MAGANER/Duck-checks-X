#!/usr/bin/env python3
try:
    from pyconvertio import Convert
except Exception as e:
    print(str(e))
    print("use: pip install pyconvertio")

import sys

infile, key = sys.argv[1:]
convert_io = Convert()
convert_io.local_file(api_key=key, file=infile, output_format="html", verbose=True)
