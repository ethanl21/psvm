#!/bin/sh

# Wrapper script that bundles and compiles the JS code to C using esbuild and qjsc
# this is probably not the right way to do this though :(

# store the build directory path
builddir=$(pwd)
qjsc_exe=$builddir/subprojects/quickjs/qjsc

# build the npm project
cd ../subprojects/psvmjs
npm i
npm run build

# compile the bundled js code to bytecode
$qjsc_exe "-c" "-o" "psvmjs.c" "dist/globalize.js"

