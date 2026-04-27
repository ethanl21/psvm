project('psvmjs', 'c', version: '0.1')

python_exe = find_program('python3', 'python')
npm_path = find_program('pnpm', 'npm').full_path()
quickjs_proj = subproject('quickjs-ng')
qjsc_exe = quickjs_proj.get_variable('qjsc_exe')

js_src_dir = meson.current_source_dir() / 'js_src'
dist_js = js_src_dir / 'dist/psvm.js'

# Bundle JS - use absolute paths to avoid cd issues
psvmjs_bundle = custom_target('psvmjs_bundle',
    output: ['psvm.js'],
    depend_files: files('js_src/src/index.ts', 'js_src/package.json', 'js_src/scripts/esbuild.mjs'),
    command: [
        'sh', '-c',
        '@0@ --dir "@1@" i && @0@ --dir "@1@" run build && cp "@2@" "@OUTPUT0@"'.format(
            npm_path, js_src_dir, dist_js)
    ],
)

# Compile to bytecode
psvmjs_raw = custom_target('psvmjs_bytecode',
    output: ['psvmjs.raw.c'],
    depends: [qjsc_exe, psvmjs_bundle],
    command: [qjsc_exe, '-e', '-s', '-o', '@OUTPUT0@', '@INPUT0@'],
    input: psvmjs_bundle
)

# Clean bytecode
psvmjs_c = custom_target('psvmjs_clean',
    output: ['psvmjs.c'],
    depends: psvmjs_raw,
    command: [python_exe, files('clean_bytecode.py'), '@INPUT0@', '@OUTPUT0@'],
    input: psvmjs_raw
)

psvmjs_inc = include_directories('include')
psvmjs_lib = library('psvmjs', sources: psvmjs_c, include_directories: psvmjs_inc)
psvmjs_dep = declare_dependency(
    include_directories: psvmjs_inc,
    link_with: psvmjs_lib
)
meson.override_dependency('psvmjs', psvmjs_dep)
