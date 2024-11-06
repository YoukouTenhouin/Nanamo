project('nanamo',
        'cpp',
        default_options: [
          'warning_level=3',
          'cpp_std=c++20',
        ]
       )

subdir('src')

gl_dep = dependency('gl')
glm_dep = dependency('glm')
glew_dep = dependency('GLEW')
glfw_dep = dependency('glfw3')

executable('nanamo', srcs, dependencies: [
  gl_dep,
  glm_dep,
  glew_dep,
  glfw_dep,
])
