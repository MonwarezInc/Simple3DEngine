/*
Copyright (c) 2016, Payet Thibault
Copyright (c) 2017, Payet Thibault
Copyright (c) 2018, Payet Thibault
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

#include <sstream>
#include <string>
#include <utility>

#include "WindowHandle.h"

namespace S3DE
{
template <class Container, class WindowType>
class WindowManager
{
private:
    Container windows_;
    int counter_{0};

public:
    WindowManager() = default;
    template <class... Args>
    WindowHandle NewWindow( Args&&... args );

    constexpr auto begin() { return windows_.begin(); };
    constexpr auto end() { return windows_.end(); };
};

template <class Container, class WindowType>
template <class... Args>
WindowHandle WindowManager<Container, WindowType>::NewWindow( Args&&... args )
{
    std::stringstream ss;
    ss << counter_;
    ++counter_;
    WindowHandle handle{ss.str()};
    WindowType window{args...};
    windows_.emplace( handle, window );
    return handle;
}
}
