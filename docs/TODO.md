# TODO

## Build

- [ ] Convert to CMake project
- [ ] Create a CMake build script for QuickJS. This project only needs to build `libquickjs.a` and `qjsc`
- [ ] Create a CMake script to compile the js bundle with `qjsc`
- [ ] Possibly use a MVSC compatible QuickJS fork

## Documentation

- [ ] Document code, Doxygen -> Sphinx + Breathe -> GitHub Pages
- [ ] Provide a size on disk and execution speed comparison between `psvm`'s CLI program and `smogon/pokemon-showdown`'s
  CLI program when packaged with `vercel/pkg`, `deno compile`, and `bun compile`

## Features

- [ ] Produce a CLI executable mirroring that of `smogon/pokemon-showdown`'s Node script
- [x] Allow creation of multiple battles with UUID
- [ ] Explore tracking battle state in more detail with `@pkmn/battle`

## Refactor

- [ ] Add exceptions to handle errors, remove any exit calls
- [x] Allow user to specify C++ callback function for any simulator output
