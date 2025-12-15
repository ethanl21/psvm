import esbuild from "esbuild";
import process from "process";

const isProd = process.env.NODE_ENV === "production";

await esbuild.build({
	entryPoints: ["src/index.ts"],
	bundle: true,

	// QuickJS-safe output
	platform: "neutral",
	format: "iife",
	target: "es2017",

	// Dependency resolution
	mainFields: ["module", "browser", "main"],

	// Output
	outfile: "dist/psvm.js",

	// Optimizations
	minify: isProd,
	minifyIdentifiers: isProd,
	minifySyntax: isProd,
	minifyWhitespace: isProd,
	treeShaking: true,
	legalComments: "none",

	// Debugging
	sourcemap: isProd ? false : "inline",
	keepNames: isProd,

	// Prevent accidental Node usage
	define: {
		"process.env.NODE_ENV": JSON.stringify(
			process.env.NODE_ENV ?? "production"
		),
		global: "globalThis",
		process: "undefined",
		Buffer: "undefined"
	},

	// Drop console and debugger statements
	drop: ["debugger", "console"],

	logLevel: "info"
});
