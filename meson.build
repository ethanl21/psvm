project(
  'psvm',
  ['cpp', 'c'],
  version: '0.1',
  default_options: ['cpp_std=c++17', 'c_std=c99', 'default_library=static'],
)

if not meson.is_subproject() and host_machine.system() == 'darwin'
  add_global_arguments(['-arch', 'x86_64', '-arch', 'arm64'], language: ['c', 'cpp', 'objcpp'])
  add_global_link_arguments(['-arch', 'x86_64', '-arch', 'arm64'], language: ['c', 'cpp', 'objcpp'])
endif

psvm_inc = include_directories('include')

# Subprojects

# quickjs-ng/quickjs
quickjs_proj = subproject('quickjs-ng', default_options: ['libc=true'])
quickjs_dep = quickjs_proj.get_variable('qjs_dep')

# stduuid (only used for test driver)
stduuid_proj = subproject('stduuid')
stduuid_dep = stduuid_proj.get_variable('stduuid_dep')

# js bytecode from bundled TS project
psvmjs_dep = dependency('psvmjs', default_options: ['default_library=static'])

# Library
psvm_lib = library(
  'psvm',
  sources: ['src/psvm.cpp'],
  dependencies: [psvmjs_dep, quickjs_dep],
  include_directories: psvm_inc,
  gnu_symbol_visibility: 'hidden' # hide QuickJS/stduuid symbols,
)

# Declare dependency for consumers
psvm_dep = declare_dependency(include_directories: psvm_inc, link_with: psvm_lib)
meson.override_dependency('psvm', psvm_dep)

# Install public headers
install_headers('include/psvm/psvm.hpp', subdir: 'psvm')

# Optional test executable
test_exe = executable(
  'test_driver',
  sources: ['test/test_driver.cpp'],
  dependencies: [psvm_dep, stduuid_dep],
)

test('driver', test_exe)