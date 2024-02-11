# PSVM

[![Documentation Status](https://readthedocs.org/projects/psvm/badge/?version=latest)](https://psvm.readthedocs.io/en/latest/?badge=latest)

PSVM is Pokémon Showdown's simulator compiled to native code with no runtime Javascript engine dependency.

> **Warning**  
> PSVM is still under construction. Implementation details are likely to change.

### Details

PSVM consists of [Smogon's Pokémon Showdown battle engine](https://github.com/smogon/pokemon-showdown) bundled
by [esbuild](https://esbuild.github.io) for [QuickJS](https://github.com/frida/quickjs).

PSVM is intended to be used as a battle engine for Pokémon fangames, similar to Essentials' PBS. It has no runtime
Javascript engine dependency and isn't made for a specific game engine, so it should be easy enough to add to any game
engine that supports native C++ modules. An example Godot 4 integration is planned.

## Building

To build PSVM, Node.js and Meson are required.

To build the test driver program:

```bash
# Clone the repository with submodules
git clone
cd psvm

# build psvm
# (in the root directory)
meson setup builddir
meson compile -C builddir
```

A compiled test driver executable will be located at `builddir/psvm_test_driver`.

## Attribution

PSVM uses [pkmn/ps](https://github.com/pkmn/ps) (a modular version
of [smogon/pokemon-showdown](https://github.com/smogon/pokemon-showdown)) to simulate battles. Both are distributed
under the MIT license.

[frida/quickjs](https://github.com/frida/quickjs) is distributed under the MIT license.

PSVM itself is distributed under the MIT license.
