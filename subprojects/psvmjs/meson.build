project('psvmjs', 'c', version: '0.1')

# find python executable
python_exe = find_program('python3', 'python')

# quickjs-ng wrap subproject
quickjs_proj = subproject('quickjs-ng')
qjsc_exe = quickjs_proj.get_variable('qjsc_exe')

# bundle psvmjs
psvmjs_ct = custom_target('psvmjs_build',
    input: 'meson_wrapper.py',
    command: [python_exe, '@INPUT@', '@CURRENT_SOURCE_DIR@'],
    output: ['psvm.js'],
)

# compile psvmjs to bytecode
psvmjs_c_ct = custom_target('psvmjs_bytecode',
    depends: [qjsc_exe, psvmjs_ct],
    command: [qjsc_exe, '-e', '-s', '-o',  meson.current_build_dir() + '/psvmjs.raw.c', meson.current_source_dir() + '/js_src/dist/psvm.js'],
    output: ['psvmjs.raw.c']
)

psvmjs_c_cleaned_ct = custom_target('psvmjs_bytecode_clean',
    input: 'clean_bytecode.py',
    depends: [psvmjs_c_ct],
    command: [python_exe, '@INPUT@', meson.current_build_dir() + '/psvmjs.raw.c', meson.current_build_dir() + '/psvmjs.c'],
    output: ['psvmjs.c']
)

psvmjs_inc = include_directories('include')
psvmjs_lib = library('psvmjs', sources: [psvmjs_c_cleaned_ct], include_directories: psvmjs_inc)

psvmjs_dep = declare_dependency(
    include_directories: psvmjs_inc,
    link_with: psvmjs_lib
)

meson.override_dependency('psvmjs', psvmjs_dep)