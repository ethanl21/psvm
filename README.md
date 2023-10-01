# PSVM

PSVM is Pokemon Showdown's simulator with no runtime dependencies (Node.js).

> **Warning**  
PSVM is still under construction, and not currently usable for its intended purpose.

### Details
PSVM consists of [Smogon's Pokemon Showdown battle engine](https://github.com/smogon/pokemon-showdown) bundled by [esbuild](https://esbuild.github.io) for [QuickJS](https://github.com/bellard/quickjs).

PSVM is intended to be used as a battle engine for Pokemon fangames, similar to Essentials' PBS. It has no runtime dependencies and isn't dependent on any particular engine, so it should be easy enough to add to any game engine that supports native C++ modules. An example Godot 4 integration is planned.

## Building
To build PSVM, Node.js and a C/C++ compiler are required. The provided makefile uses ``clang``, but ``gcc`` should also be sufficient.

```bash
git clone https://github.com/ethanl21/psvm
cd psvm

# build the JavaScript Bundle
cd psvmjs
pnpm i
pnpm build

# build PSVM
cd ..
make all
```
The compiled executable will be located at ``dist/psvm``.

## Attribution
PSVM uses [pkmn/ps](https://github.com/pkmn/ps) (a modular version of [smogon/pokemon-showdown](https://github.com/smogon/pokemon-showdown)) to simulate battles. Both are distributed under the MIT license.

PSVM uses [bellard/quickjs](https://github.com/bellard/quickjs) as its JavaScript runtime, which is distributed under the MIT license.

PSVM itself is distributed under the [GPL-3.0-Only](https://choosealicense.com/licenses/gpl-3.0/) license.