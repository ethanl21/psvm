#!/usr/bin/env node

import * as esbuild from "esbuild";
import pkg from '@sprout2000/esbuild-copy-plugin';

const {copyPlugin} = pkg;

// noinspection JSUnresolvedReference
await esbuild
    .build({
        entryPoints: ["src/main.ts"],
        bundle: true,
        outfile: "dist/psvm.js",
        format: "esm",
        treeShaking: false,
        define: {
            global: "globalThis",
        },
        external: ["std", "os", "host"],
        plugins: [
            copyPlugin({
                src: "./src/globalize.js",
                dest: "./dist/globalize.js",
            }),
        ],
    })
    .catch(() => process.exit(1));
