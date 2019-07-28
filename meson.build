project('Simple3DEngine','cpp', default_options : ['cpp_std=c++1z'],
       version: '0.0.0')

sdl2_dep = dependency('sdl2',required:true)
sdl2image_dep = dependency('SDL2_image',required:true)
assimp_dep = dependency('assimp',required:true)
glew_dep = dependency('glew',required:true)
glm_dep = dependency('glm',required:true)
bullet_dep = dependency('bullet', required:true)

range_v3_dep = dependency('range-v3', required:true, fallback:['range-v3', 'range_dep'])

test_option = get_option('test')
if (test_option)
  gtest_dep = dependency('gtest', required:false,fallback:['gtest','gtest_dep'])
  gmock_dep = dependency('gmock', required:false,fallback:['gtest','gmock_dep'])
endif

simple3dengine_cpp_args = '-DS3DE_USE_DSA=1'

subdir('src')



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
