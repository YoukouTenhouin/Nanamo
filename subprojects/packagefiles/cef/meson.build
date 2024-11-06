project('cef', 'cpp')

# Compile CEF wrapper library

cmake = find_program('cmake')
cef_compile_script = find_program(
  join_paths(meson.current_source_dir(), 'build_cef_dll_wrapper.sh'))

cef_dll_target = custom_target(
  'libcef_dll_wrapper',
  input: join_paths(meson.current_source_dir(), 'libcef_dll/CMakeLists.txt'),
  output: 'libcef_dll_wrappper.a',
  command: ['build_cef_dll_wrapper.sh', '@BUILD_DIR@',
            meson.current_source_dir(), cmake, '@OUTPUT@'])

cef_libs = ['libcef.so', 'libEGL.so', 'libGLESv2.so', 'libGLESv2.so',
            'libvk_swiftshader.so', 'libvulkan.so.1', 'vk_swiftshader_icd.json',
            'snapshot_blob.bin', 'v8_context_snapshot.bin']
cef_resources = ['resources.pak', 'chrome_100_percent.pak',
                 'chrome_200_percent.pak', 'icudtl.dat']

foreach f: cef_libs
  install_data('./Release' / f, install_dir: 'nanamo')
endforeach

foreach f: cef_resources
  install_data('./Resources' / f, install_dir: 'nanamo')
endforeach

cxx = meson.get_compiler('cpp')

cef_dep = declare_dependency(
  compile_args:[
    '-DOS_LINUX=1',
    '-DPROJECT_ARCH=x86_64'
  ],
  link_with: cef_dll_target[0],
  include_directories: include_directories(['./', 'include']),
  dependencies: cxx.find_library(
    'cef', dirs: join_paths(meson.current_source_dir(), 'Release'))
)

meson.override_dependency('cef', cef_dep)
