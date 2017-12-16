#!/bin/sh
cd $MESON_SOURCE_ROOT/src
cppcheck --enable=all \
    --std=c++14 --language=c++ \
    --xml --project=$MESON_BUILD_ROOT/compile_commands.json 2> $MESON_BUILD_ROOT/cppcheck.xml

cppcheck-htmlreport --file=$MESON_BUILD_ROOT/cppcheck.xml \
    --report-dir=$MESON_BUILD_ROOT/cppcheckReport
