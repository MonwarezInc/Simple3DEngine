#include "test_parser.h"

void ParserTest::SetUp()
{
    parameter = GetParam();
}

void ParserTest::TestBody()
{
}

TEST_P( ParserTest, Simple )
{
    float x{0};
    float y{0};
    float z{0};

    unsigned long u{0};
    unsigned long v{0};

    S3DE::Parser::Find3uple( parameter.parseTuple, x, y, z );

    EXPECT_FLOAT_EQ( x, parameter.floatValue[ 0 ] );
    EXPECT_FLOAT_EQ( y, parameter.floatValue[ 1 ] );
    EXPECT_FLOAT_EQ( z, parameter.floatValue[ 2 ] );

    S3DE::Parser::FindCouple( parameter.parseCouple, u, v );

    EXPECT_EQ( u, parameter.integerValue[ 0 ] );
    EXPECT_EQ( v, parameter.integerValue[ 1 ] );
}

TEST_P( ParserTest, IntegerValue )
{
}
ParserTestParam createParameter( std::string toParseCouple, std::string toParseTuple,
                                 std::array<unsigned long, 2> couple, std::array<float, 3> tuple )
{
    ParserTestParam testParameter{toParseCouple, toParseTuple, couple, tuple};
    return testParameter;
}
INSTANTIATE_TEST_CASE_P( FloatingValue, ParserTest,
                         testing::ValuesIn( {createParameter( "0,0", "1,2,3", {{0, 0}},
                                                              {{1, 2, 3}} ),
                                             createParameter( "0    ,    0", "   1,   100, 33.3 ",
                                                              {{0, 0}}, {{1, 100, 33.3}} )} ) );
