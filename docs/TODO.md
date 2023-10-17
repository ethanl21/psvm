# TODO

## Build

- [x] Convert to CMake project
- [x] Create a CMake build script for QuickJS. This project only needs to build `libquickjs.a` and `qjsc`
- [x] Create a CMake script to compile the js bundle with `qjsc`
- [x] Possibly use a MVSC compatible QuickJS fork
- [x] Bundle the JavaScript source during the CMake build workflow

## Documentation

- [x] Document code, Doxygen -> Sphinx + Breathe -> GitHub Pages
- [x] Find a way to make Doxygen/Sphinx optional dependencies since users don't need to build the docs
- [ ] Provide a size on disk and execution speed comparison between `psvm`'s CLI program and `smogon/pokemon-showdown`'s
  CLI program when packaged with `vercel/pkg`, `deno compile`, and `bun compile`

## Features

- [ ] Produce a CLI executable mirroring that of `smogon/pokemon-showdown`'s Node script
- [x] Allow creation of multiple battles with UUID
- [ ] Explore tracking battle state in more detail with `@pkmn/battle`

## Refactor

- [ ] Add exceptions to handle errors, remove any exit calls
- [x] Allow user to specify C++ callback function for any simulator output
