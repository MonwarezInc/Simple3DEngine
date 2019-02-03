project('Simple3DEngine','cpp', default_options : ['cpp_std=c++1z'],
       version: '0.0.0')

sdl2_dep = dependency('sdl2',required:true)
sdl2image_dep = dependency('SDL2_image',required:true)
assimp_dep = dependency('assimp',required:true)
glew_dep = dependency('glew',required:true)
glm_dep = dependency('glm',required:true)
bullet_dep = dependency('bullet', required:true)

test_option = get_option('test')
if (test_option)
  gtest_dep = dependency('gtest', required:false,fallback:['gtest','gtest_dep'])
  gmock_dep = dependency('gmock', required:false,fallback:['gtest','gmock_dep'])
endif

simple3dengine_inc = include_directories('src')
subdir('src')

simple3dengine_dep = declare_dependency(link_with: simple3dengine_lib,
                                        include_directories: simple3dengine_inc,
                                        dependencies: [sdl2_dep,
                                                       sdl2image_dep,
                                                       assimp_dep,
                                                       glew_dep,
                                                       glm_dep,
                                                       bullet_dep])


# test section
if (test_option)
  test_list = ['Parser', 'Engine']

  foreach tst : test_list
    exe = executable('@0@Test'.format(tst),
                     ['src/tests/@0@/main.cpp'.format(tst),
                      'src/tests/@0@/@0@_test.h'.format(tst),
                      'src/tests/@0@/@0@_test.cpp'.format(tst)],
                     dependencies : [gtest_dep,gmock_dep,simple3dengine_dep,
                                    sdl2_dep, sdl2image_dep, assimp_dep, glew_dep, glm_dep])
    test('@0@ Test'.format(tst), exe)
  endforeach
endif

documentation_option = get_option('documentation')
# doxygen support
if (documentation_option)
  doxygen = find_program('doxygen' , required: true)
  doxyfile_conf = configuration_data()
  doxyfile_conf.set('PACKAGE_NAME', meson.project_name())
  doxyfile_conf.set('PACKAGE_VERSION', meson.project_version())
  doxyfile_conf.set('top_srcdir', meson.source_root())
  doxyfile_conf.set('top_builddir', meson.build_root())

  dot = find_program('dot', required: false)

  if dot.found()
      doxyfile_conf.set('HAVE_DOT', 'YES')
  else
      doxyfile_conf.set('HAVE_DOT', 'NO')
  endif

  doxyfile = configure_file(input: 'Doxyfile.in',
                            output: 'Doxyfile',
                            configuration: doxyfile_conf,
                            install: false)

  doxygen_target = custom_target('doxygen',
                                 input: [doxyfile],
                                 output: ['docs'],
                                 command: [ doxygen, doxyfile ],
                                 install: false)
endif

# pkg config section

pkg = import('pkgconfig')
pkg.generate(description:'Simple3DEngine is a 3D engine for c++ project',
             filebase: 'Simple3DEngine',
             libraries: [simple3dengine_lib],
             name: 'Simple3DEngine',
             subdirs:'S3DE',
             version: meson.project_version()
            )

# cppcheck section
cppcheck_exe = find_program('cppcheck',required:false)
cppcheck_html = find_program('cppcheck-htmlreport',required:false)

cppcheck_target = run_target('cppcheck',command : ['tools/cppcheck.sh'])
