# PSVM

PSVM is Pokemon Showdown's simulator with no runtime dependencies (Node.js).

## Building
Build the JavaScript bundle:
```bash
cd psvmjs
pnpm i
pnpm build
```
Then, build the main ``psvm`` executable:
```bash
# (from the root directory)
make all
```
The compiled executable will be located at ``dist/psvm``.
