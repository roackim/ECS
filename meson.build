project('ECS', 'cpp',
    version : '0.1',
    default_options : [
        'cpp_std=c++17',
        'warning_level=3'
    ]
)

target = 'main.exe'
test_target = 'test_main.exe'
target_folder = 'bin'
lib_folder = 'lib'

# Compiler configuration
cpp = meson.get_compiler('cpp')

add_project_arguments('-Wall', '-Wextra', language : 'cpp')


# Source files
lib_src = [
    'include/ECS.hpp',
    'include/ECS/ComponentArray.hpp',
    'include/ECS/ComponentManager.hpp',
    'include/ECS/Entity.hpp',
    'include/ECS/EntityManager.hpp',
    # Add other source files here
]

ecs_lib = static_library('ecs',
    sources: lib_src,
    include_directories: include_directories('include'),
    install: true,
    install_dir: join_paths(meson.current_source_dir(), lib_folder)
)


test_src = [
    'tests/main.cpp',
    'tests/ECS/ComponentArray.cpp',
    'tests/ECS/ComponentManager.cpp',
    'tests/ECS/ECS.cpp',
    'tests/ECS/EntityManager.cpp',
]

# Create executable
test_exe = executable(test_target,
    sources : test_src,
    include_directories: include_directories('include', 'tests'),
    install: true,
    link_with: ecs_lib,
    install_dir: join_paths(meson.current_source_dir(), target_folder)
)
