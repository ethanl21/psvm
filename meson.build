project('psvm', ['cpp', 'c'], version: '0.1',
    default_options : [
        #'warning_level=1',
        'cpp_std=c++17',
        'c_std=c11',
        'default_library=static'
        ]
    )

# frida/quickjs wrap subproject
quickjs_proj = subproject('quickjs')
quickjs_dep = quickjs_proj.get_variable('quickjs_dep')

# psvm js bytecode subproject
psvmjs_proj = subproject('psvmjs')
psvmjs_dep = psvmjs_proj.get_variable('psvmjs_dep')

# stduuid wrap subproject
stduuid_proj = subproject('stduuid')


# psvm library
psvm_inc = include_directories('include')
psvm_lib = library('psvm', sources: ['src/psvm.cpp'], dependencies: [quickjs_dep, psvmjs_dep], include_directories: psvm_inc)

executable('psvm_test_driver', sources: ['test/test_driver.cpp'], link_with: [psvm_lib], include_directories: psvm_inc)
