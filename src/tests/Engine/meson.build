engine_sources = files([
                        'main.cpp',
                        'Engine_test.cpp',
                      ])
engine_headers = files([
                        'Engine_test.h',
                      ])

engineTest = executable('test-Engine',
                        sources: [engine_sources,
                                  engine_headers],
                        dependencies: [gtest_dep,
                                       gmock_dep,
                                       simple3dengine_dep,
                                       simple3dengine_window_sdl2_dep,])

test('test-Engine', engineTest)
