parser_source = [ 'test_parser.cpp','test_parser.h','test_main.cpp']

test_exe = executable('TestParser' , parser_source,
                       dependencies:gtestdep,
                       link_with:simple3dengine,
                       include_directories:incdir)

test('TestParser', test_exe)