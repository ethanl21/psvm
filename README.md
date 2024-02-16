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

To build PSVM, Node.js and Meson are required. To compile for Windows, use the provided `cross/x86_64-w64-mingw32.txt` to cross compile on a Linux machine.
### Instructions

```bash
git clone https://github.com/ethanl21/psvm.git
cd psvm
meson setup builddir --buildtype debug # or release
meson compile -C builddir
```

#### For Windows

```bash
git clone https://github.com/ethanl21/psvm.git
cd psvm
meson setup --cross-file cross/x86_64-w64-mingw32.txt builddir --buildtype debug # or release
meson compile -C builddir
```

PSVM is built as a static library by default. To build as a shared library:

```bash
# build psvm
# (in the root directory)
meson setup builddir -Ddefault_library=shared
meson compile -C builddir
```

A compiled test driver executable will be located at `builddir/psvm_test_driver`.

## Usage

To use PSVM in a Meson project, add `psvm.wrap` to the `subprojects` directory in your project's root with the following contents:

```
[wrap-git]
url = https://github.com/ethanl21/psvm.git
revision = head
depth = 1
```

(Replace `head` with a commit hash to use a specific version.)

Then you can link to PSVM in your `build.meson` file.

```
psvm_proj = subproject('psvm')
psvm_dep = dependency('psvm')

executable('my_program', sources: ['...'], dependencies: ['psvm_dep'])
```

## Attribution

PSVM uses [pkmn/ps](https://github.com/pkmn/ps) (a modular version
of [smogon/pokemon-showdown](https://github.com/smogon/pokemon-showdown)) to simulate battles. Both are distributed
under the MIT license.

[bellard/quickjs](https://github.com/bellard/quickjs) is distributed under the MIT license.

[mariusbancila/stduuid](https://github.com/mariusbancila/stduuid) is distributed under the MIT license.

PSVM itself is distributed under the MIT license.
