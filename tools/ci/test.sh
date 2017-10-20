#!/bin/sh
cd build
cmake --build . --target pre_coverage
ctest . --verbose
cmake --build . --target coverage_report
