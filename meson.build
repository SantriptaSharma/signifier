project('signify', 'cpp', 'c')

cc = meson.get_compiler('c')
cxx = meson.get_compiler('cpp')

c_args = []
cpp_args = []
link_args = []
name_suffix = []
dependencies = []

# raylib logic inherited from https://github.com/gilzoide/raylib-meson-template/blob/main/meson.build
cmake = import('cmake')
raylib_opts = cmake.subproject_options()
raylib_opts.set_install(false)
raylib_opts.add_cmake_defines({
  'BUILD_EXAMPLES': 'OFF',
  'PLATFORM': 'Desktop'
  # TODO: Add other CMake options for Raylib, if needed
})

raylib_subproject = cmake.subproject('raylib', options: raylib_opts)
dependencies += raylib_subproject.dependency('raylib')

if host_machine.system() == 'windows'
  dependencies += [
    cc.find_library('winmm'),
  ]
elif host_machine.system() == 'linux'
  dependencies += [
    cc.find_library('m'),
    cc.find_library('dl'),
  ]
endif

inc_dir = include_directories(
  'src'
)

sources = files(
  'src/main.cpp', 'src/scene.cpp', 'src/object.cpp'
)

executable('signify',
  sources,
  c_args: c_args,
  cpp_args: cpp_args,
  dependencies: dependencies,
  include_directories: inc_dir,
  link_args: link_args,
  name_suffix: name_suffix
)