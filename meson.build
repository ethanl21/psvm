project('psvm', ['cpp', 'c'], version: '0.1',
    default_options : [
        'cpp_std=c++17',
        'c_std=c99',
        'default_library=static'
        ]
    )

if meson.is_subproject() == false
    if build_machine.system() == 'darwin'
        add_global_arguments(['-arch', 'x86_64', '-arch', 'arm64'], language: ['c', 'cpp', 'objcpp'])
        add_global_link_arguments(['-arch', 'x86_64', '-arch', 'arm64'], language: ['c', 'cpp', 'objcpp'])
    endif
endif

# bellard/quickjs wrap dependency
quickjs_dep = dependency('quickjs', default_options: ['default_library=static'])

# psvm js bytecode subproject
psvmjs_dep = dependency('psvmjs', default_options: ['default_library=static'])

# stduuid wrap dependency
stduuid_dep = dependency('stduuid', default_options: ['using_cxx20_span=disabled', 'tests=false'])

# psvm library
psvm_inc = include_directories('include')
psvm_lib = library('psvm', sources: ['src/psvm.cpp'], dependencies: [quickjs_dep, psvmjs_dep, stduuid_dep], include_directories: psvm_inc)
psvm_dep = declare_dependency(
    include_directories: psvm_inc,
    link_with: psvm_lib
)

#executable('psvm_test_driver', sources: ['test/test_driver.cpp'], dependencies: psvm_dep)

meson.override_dependency('psvm', psvm_dep)
