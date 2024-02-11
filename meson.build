project('psvm', ['cpp', 'c'], version: '0.1',
    default_options : [
        'cpp_std=c++17',
        'c_std=c99',
        'default_library=static',
        'quickjs:default_library=static',
        'psvmjs:default_library=static',
        'stduuid:using_cxx20_span=disabled'
        ]
    )

if build_machine.system() == 'darwin'
  add_global_arguments(['-arch', 'x86_64', '-arch', 'arm64'], language: ['c', 'cpp', 'objcpp'])
  add_global_link_arguments(['-arch', 'x86_64', '-arch', 'arm64'], language: ['c', 'cpp', 'objcpp'])
endif

# frida/quickjs wrap subproject
quickjs_proj = subproject('quickjs')
quickjs_dep = quickjs_proj.get_variable('quickjs_dep')

# psvm js bytecode subproject
psvmjs_proj = subproject('psvmjs')
psvmjs_dep = psvmjs_proj.get_variable('psvmjs_dep')

# stduuid wrap subproject
stduuid_proj = subproject('stduuid')
stduuid_dep = stduuid_proj.get_variable('stduuid_dep')

# psvm library
psvm_inc = include_directories('include')
psvm_lib = library('psvm', sources: ['src/psvm.cpp'], dependencies: [quickjs_dep, psvmjs_dep, stduuid_dep], include_directories: psvm_inc)

executable('psvm_test_driver', sources: ['test/test_driver.cpp'], link_with: [psvm_lib], include_directories: psvm_inc)
