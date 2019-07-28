parser_sources = files([
                        'main.cpp',
                        'Parser_test.cpp',
                      ])
parser_headers = files([
                        'Parser_test.h',
                      ])

parserTest = executable('test-Parser',
                        sources: [parser_sources,
                                  parser_headers],
                        dependencies: [gtest_dep,
                                       gmock_dep,
                                       simple3dengine_tools_dep])

test('test-Parser', parserTest)
