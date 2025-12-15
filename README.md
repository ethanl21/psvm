# PSVM

[![Documentation Status](https://readthedocs.org/projects/psvm/badge/?version=latest)](https://psvm.readthedocs.io/en/latest/?badge=latest)

PSVM is Pokémon Showdown's simulator compiled to native code with no runtime Javascript engine dependency.

> **Warning**  
> PSVM is still under construction. Implementation details are likely to change.

### Details

PSVM consists of [Smogon's Pokémon Showdown battle engine](https://github.com/smogon/pokemon-showdown) bundled
by [esbuild](https://esbuild.github.io) for [QuickJS-ng](https://github.com/quickjs-ng/quickjs).

PSVM is intended to be used as a battle engine for Pokémon fangames, similar to Essentials' PBS. It has no runtime
Javascript engine dependency and isn't made for a specific game engine, so it should be easy enough to add to any game
engine that supports native C++ modules. An example Godot 4 integration is planned.

## Building

To build PSVM, Node.js and Meson are required.

### Instructions

```bash
git clone https://github.com/ethanl21/psvm.git
cd psvm
meson setup builddir --buildtype debug # or release
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

An example program is located at [test/test_driver.cpp](test/test_driver.cpp).

To use PSVM in a Meson project, add `psvm.wrap` to the `subprojects` directory in your project's root with the following contents:

```
[wrap-git]
directory = psvm-1.0.0
url = https://github.com/ethanl21/psvm.git
revision = v1.0.0
wrapdb_version = '1.0.0'
depth = 1

[provide]

psvm = psvm_dep
```

(Replace `head` with a commit hash to use a specific version.)

Then you can link to PSVM in your `build.meson` file.

```
psvm_dep = dependency('psvm')

executable('my_program', sources: ['...'], dependencies: ['psvm_dep'])
```

## Attribution

PSVM uses [pkmn/ps](https://github.com/pkmn/ps) (a modular version
of [smogon/pokemon-showdown](https://github.com/smogon/pokemon-showdown)) to simulate battles. Both are distributed
under the MIT license.

[quickjs-ng/quickjs](https://github.com/quickjs-ng/quickjs) is distributed under the MIT license.

[mariusbancila/stduuid](https://github.com/mariusbancila/stduuid) is distributed under the MIT license.

PSVM itself is distributed under the MIT license.
