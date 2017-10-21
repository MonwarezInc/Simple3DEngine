/*
Copyright (c) 2017, Payet Thibault
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Monwarez Inc nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL PAYET THIBAULT BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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
INSTANTIATE_TEST_CASE_P(
    FloatingValue, ParserTest,
    testing::ValuesIn(
        {createParameter( "0,0", "1,2,3", {{0, 0}}, {{1, 2, 3}} ),
         createParameter( "0    ,    0", "   1,   100, 33.3 ", {{0, 0}}, {{1, 100, 33.3}} ),

         createParameter( "0 ,  000000", "   1.00000,   100, 33.3 ", {{0, 0}}, {{1, 100, 33.3}} ),


         createParameter( "00000,0", "   1000000.0001,   100, 33.3 ", {{0, 0}},
                          {{1000000.0001, 100, 33.3}} ),


         createParameter( "100,    0", "   1,   100, 33.3 ", {{100, 0}}, {{1, 100, 33.3}} ),
         createParameter( "100,    50", "   1,   100, 33.3 ", {{100, 50}}, {{1, 100, 33.3}} ),
         createParameter( "0    ,    40", "   1,   100, 33.3 ", {{0, 40}}, {{1, 100, 33.3}} )} ) );
