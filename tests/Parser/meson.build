parser_source = [ 'test_parser.cpp','test_parser.h','test_main.cpp']

test_exe = executable('TestParser' , parser_source,
                       dependencies:[gtestdep,simple3dengine_dep]
                     )

test('TestParser', test_exe)