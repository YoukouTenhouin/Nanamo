project('nanamo',
        'cpp',
        default_options: [
          'warning_level=3',
          'cpp_std=c++20',
        ]
       )

subdir('src')

gl_dep = dependency('gl')
glew_dep = dependency('GLEW')
glfw_dep = dependency('glfw3')

executable('nanamo', srcs, dependencies: [
  gl_dep,
  glew_dep,
  glfw_dep,
])
