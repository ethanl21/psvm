import sys

inp, outp = sys.argv[1], sys.argv[2]

content = None
with open(inp) as f:
    content = f.read()

# Replace content
REPLACEMENTS = {
    '#include "quickjs-libc.h"': "#include <inttypes.h>"}
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
    f.writelines("\n".join(out) + "\n")
