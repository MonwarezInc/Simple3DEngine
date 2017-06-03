/*
Copyright (c) 2016, Payet Thibault
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
#include <S3DE_Parser.h>

void S3DE::Parser::Find3uple( std::string buf, float &x, float &y, float &z,
                              std::string const &sep )
{
    auto i1 = buf.find( sep );
    if ( ( i1 == std::string::npos ) || ( i1 == 0 ) )
        throw std::string( "Error could not get the first parameter of 3-uple" );
    auto substr1 = buf.substr( 0, i1 );
    buf          = buf.substr( i1 + 1 );
    x            = std::stof( substr1 );
    i1           = buf.find( sep );
    if ( ( i1 == std::string::npos ) || ( i1 == 0 ) )
        throw std::string( "Error could not get the second parameter of 3-uple" );
    substr1 = buf.substr( 0, i1 );
    y       = std::stof( substr1 );
    buf     = buf.substr( i1 + 1 );
    if ( buf.length() == 0 )
        throw std::string( "Error could not get the third parameter of 3-uple" );

    z = std::stof( buf );
}
void S3DE::Parser::FindCouple( std::string buf, unsigned long &a, unsigned long &b,
                               std::string const &sep )
{
    auto i1 = buf.find( sep );
    if ( ( i1 == std::string::npos ) || ( i1 == 0 ) )
        throw std::string( "Error could not get the first parameter of couple" );
    auto substr1 = buf.substr( 0, i1 );
    buf          = buf.substr( i1 + 1 );
    if ( buf.length() == 0 )
        throw std::string( "Error could not get the second parameter of couple" );

    a = std::stoul( substr1 );
    b = std::stoul( buf );
}
size_t S3DE::Parser::ExtractMatch( std::string const &in, std::string &out,
                                   std::string const &start, std::string const &end )
{
    auto i1 = in.find( start );
    auto i2 = in.find( end );
    if ( ( i1 == std::string::npos ) || ( i2 == std::string::npos ) || i1 > i2 )
        throw std::string( "Error , " ) + start + std::string( " or" ) + end
            + std::string( "  are not match" );
    out = in.substr( i1 + 1, i2 - i1 - 1 );
    return i2;
}
