target_link_libraries(${PROJECT_NAME} Simple3DEngine)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)
target_link_libraries(${PROJECT_NAME} gmock gmock_main)

list(APPEND CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

if (asan)
  add_definitions("-fsanitize=address -fno-omit-frame-pointer")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()
