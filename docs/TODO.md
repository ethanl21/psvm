# Todo
- [ ] convert to CMake project
- [ ] add exceptions to handle errors
- [ ] produce shared library, static object, standalone cli executable
- [ ] use thread instead of fork to prevent methods from being called in the simulator process
- [ ] pass messages using message queues instead of pipes
- [ ] possibly use Boost
- [ ] possibly use ftk/quickjspp wrapper
- [ ] possibly use an msvc compatible QuickJS fork
- [ ] add demo CLI executable (Boost program options?)

## NOTES
Writing to the sim stream in JS returns a promise that resolves when it's safe to write to the stream again. find out if this is a good time to check for output and insert it into the message queue.

explore replicating smogon's pokemon-showdown cli file (pokemon-showdown in their git repo). it's a nodejs script, so replace the node parts with vanilla js or c++ wrapper fns. provide a size comparison between that replicated cli program and smogon's cli script bundled with vercel/pkg, deno compile, and bun compile.