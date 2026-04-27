#!/usr/bin/env python3
import sys

def main():
    inp = sys.argv[1]
    outp = sys.argv[2]
    
    with open(inp) as f:
        content = f.read()
    
    # Replace content
    REPLACEMENTS = {
        '#include "quickjs-libc.h"': "#include <inttypes.h>"
    }
    for item in REPLACEMENTS:
        content = content.replace(item, REPLACEMENTS[item])
    
    # Strip after a given line
    START_MARKER = "static JSContext *JS_NewCustomContext"
    out = []
    for line in content.split("\n"):
        if line.startswith(START_MARKER):
            break
        out.append(line)
    
    with open(outp, "w") as f:
        f.write("\n".join(out) + "\n")

if __name__ == "__main__":
    main()
