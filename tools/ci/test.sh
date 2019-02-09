#!/bin/sh
GTEST_OUTPUT="xml:report.xml"
export GTEST_OUTPUT

cd build
ninja test
ninja coverage
