#!/bin/sh
GTEST_OUTPUT="xml:report.xml"
export GTEST_OUTPUT

cd build
# pre coverage
lcov --capture --initial --directory .. --output-file coverage-base.info
# run test
ninja test
# generate final coverage
lcov --capture --directory .. --output-file coverage-test.info
lcov --add-tracefile coverage-base.info --add-tracefile coverage-test.info \
     --output-file coverage.info
lcov --extract coverage.info '*/src/engine/*' '*/src/misc/*' '*/src/tools/*' '*/src/window/*' -o filtered.info
genhtml --ignore-errors source filtered.info \
        --prefix .. --keep-descriptions --frames --show-details --legend \
        --output-directory out-coverage
