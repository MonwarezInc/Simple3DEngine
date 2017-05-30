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
#pragma once
#include <exception>
#include <string>
namespace S3DE
{
enum class MeshExceptFlag { FATAL, RELEASE };
struct ResourceExcept
{
    size_t id;           ///< the id of the resource
    MeshExceptFlag flag; ///< the flag to set the severity of the exception
};
class MeshException : public std::exception
{
public:
    virtual const char* what() const throw() { return m_msg.c_str(); }
    ///	\brief	Set the resource except settings
    ///	\param	rc	a structure filled with id of the resource and the severity of the exception
    virtual void SetResource(ResourceExcept const& rc);
    ///	\brief	Set the message of the exception
    ///	\param	msg the message of the exception
    virtual void SetMsg(std::string const& msg) { m_msg = msg; }
    ///	\brief	Return the structure that contain the id of the resource and the severity of the
    ///exception
    virtual ResourceExcept GetResourceExcept() const;

protected:
    ResourceExcept m_rc; ///< the resource exception information
    std::string m_msg;   ///< the description of the error
};
} // end of S3DE namespace
