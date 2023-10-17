# PSVM
[![Documentation Status](https://readthedocs.org/projects/psvm/badge/?version=latest)](https://psvm.readthedocs.io/en/latest/?badge=latest)

PSVM is Pokémon Showdown's simulator compiled to native code with no runtime dependencies.

> **Warning**  
> PSVM is still under construction. Implementation details are likely to change.


### Details

PSVM consists of [Smogon's Pokémon Showdown battle engine](https://github.com/smogon/pokemon-showdown) bundled
by [esbuild](https://esbuild.github.io) for [QuickJS](https://github.com/bellard/quickjs).

PSVM is intended to be used as a battle engine for Pokémon fangames, similar to Essentials' PBS. It has no runtime
dependencies and isn't dependent on any particular engine, so it should be easy enough to add to any game engine that
supports native C++ modules. An example Godot 4 integration is planned.

## Building

To build PSVM, Node.js and CMake are required.

To build the test driver program:

```bash
# Clone the repository with submodules
git clone --recursive https://github.com/ethanl21/psvm
cd psvm

# build psvm
# (in the root directory)
cmake -B [build directory] -S .
cmake --build [build directory]
```

A compiled test driver executable will be located at ``[build directory]/test/psvm_test``.

## Attribution

PSVM uses [pkmn/ps](https://github.com/pkmn/ps) (a modular version
of [smogon/pokemon-showdown](https://github.com/smogon/pokemon-showdown)) to simulate battles. Both are distributed
under the MIT license.

[bellard/quickjs](https://github.com/bellard/quickjs) is distributed under the MIT license.

[boostorg/uuid](https://github.com/boostorg/uuid) is distributed under the BSL-1.0 license.

PSVM itself is distributed under the [GPL-3.0-Only](https://choosealicense.com/licenses/gpl-3.0/) license.
