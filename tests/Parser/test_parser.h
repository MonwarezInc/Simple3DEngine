#pragma once
#include <S3DE_Parser.h>
#include <array>
#include <gtest/gtest.h>
#include <string>

struct ParserTestParam
{
    ParserTestParam() = default;
    std::string parseCouple;
    std::string parseTuple;
    std::array<unsigned long, 2> integerValue{{0, 0}};
    std::array<float, 3> floatValue{{0, 0, 0}};
};

class ParserTest : public ::testing::TestWithParam<ParserTestParam>
{
public:
    virtual void SetUp() override;
    virtual void TestBody() override;

    ParserTestParam parameter;
};
