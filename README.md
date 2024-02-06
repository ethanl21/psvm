# PSVM

[![Documentation Status](https://readthedocs.org/projects/psvm/badge/?version=latest)](https://psvm.readthedocs.io/en/latest/?badge=latest)

PSVM is Pokémon Showdown's simulator compiled to native code with no runtime Javascript engine dependency.

> **Warning**  
> PSVM is still under construction. Implementation details are likely to change.

### Details

PSVM consists of [Smogon's Pokémon Showdown battle engine](https://github.com/smogon/pokemon-showdown) bundled
by [esbuild](https://esbuild.github.io) for [QuickJS](https://github.com/bellard/quickjs).

PSVM is intended to be used as a battle engine for Pokémon fangames, similar to Essentials' PBS. It has no runtime
Javascript engine dependency and isn't made for a specific game engine, so it should be easy enough to add to any game
engine that supports native C++ modules. An example Godot 4 integration is planned.

### Todo

- [ ] Replace c-smile/quickjspp with suchipi/quickjs
- [ ] Handle simulator response callback correctly instead of the current way
- [ ] Split build process into three steps
    - [ ] Compile suchipi/quickjs for Windows, macOS, Linux using Docker or directly on Unix
    - [ ] Bundle the Typescript project and transform it into bytecode using suchipi/quickjs's bytecode compiler
    - [ ] Compile psvm using suchipi/quickjs' static library, the Typescript bytecode, and PSVM's source
- [ ] Update documentation
- [ ] Add error handling to ShowdownService (both cpp and ts)
- [ ] Rename cpp or ts class for clarity
- [ ] Update README
- [ ] Use a real random number generator instead of the polyfill hack

## Building

> **Warning**
> Due to a compiler error produced by quickjs, psvm cannot be compiled using MVSC under the Release preset. The Debug
> preset can successfully be compiled.

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

PSVM itself is distributed under the MIT license.
