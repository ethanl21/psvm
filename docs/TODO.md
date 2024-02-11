# TODO

## Build

- [x] Replace c-smile/quickjspp with frida/quickjs
- [x] Migrate CMake build process to Meson

## Documentation

- [x] Document code, Doxygen -> Sphinx + Breathe -> ReadTheDocs
- [x] Find a way to make Doxygen/Sphinx optional dependencies since users don't need to build the docs
- [ ] Provide a size on disk and execution speed comparison between `psvm`'s CLI program and `smogon/pokemon-showdown`'s
  CLI program when packaged with `vercel/pkg`, `deno compile`, and `bun compile`

## Features

- [ ] Produce a CLI executable mirroring that of `smogon/pokemon-showdown`'s Node script
- [x] Allow creation of multiple battles with UUID
- [ ] Explore tracking battle state in more detail with `@pkmn/battle`

## Refactor

- [ ] Add error handling to ShowdownService (both cpp and ts)
- [x] Allow user to specify C++ callback function for any simulator output
- [ ] Handle simulator response callback correctly instead of the current way
- [ ] Rename cpp or ts class for clarity
- [x] Use a real random number generator instead of the polyfill hack
- [ ] Implement tests properly and not the hacky way