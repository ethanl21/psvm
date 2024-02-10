project('psvmjs', 'c', version: '0.1')

quickjs_proj = subproject('quickjs')
quickjs_dep = quickjs_proj.get_variable('quickjs_dep')

python_exe = find_program('python3', 'python')

# bundle psvmjs
psvmjs_ct = custom_target('psvmjs_build',
    input: 'scripts/meson_wrapper.py',
    command: [python_exe, '@INPUT@'],
    output: ['psvmjs.c'],
)

psvmjs_dep = declare_dependency(
    include_directories: include_directories('.'),
)