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
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>

#include "S3DE_Interpolate.hpp"


namespace S3DE
{
template <class T>
class LinearInterpolate : public CurveInterpolateImpl<T>
{
public:
    LinearInterpolate() = default;
    virtual glm::vec3	GetInterpolated(T totaltime) final
    {
        size_t sizeVpos     = position_.size();
        T currenttime{0};
        T lasttime{0};
        T t{0};
        size_t indice{0};
        if (isClosed)
        {
            currenttime += sizeVpos * time_[indice];
            while(totaltime > currenttime)
            {
                totaltime -= currenttime;
            }
            currenttime = 0;
        }
        while(indice < sizeVpos)
        {
            currenttime += time_[indice];
            if (totaltime <= currenttime)
            {
       	        break;
            }
            ++indice;
            lasttime = currenttime;
        }
        if (1 == sizeVpos)
        {
            return position_.front();
        }
        if (0 == sizeVpos)
        {
            throw std::string ("error no key pos defined");
        }
        if (0 == currenttime)
        {
            throw std::string("error currenttime equal 0 , can't divide");
        }
        t= totaltime - lasttime;
        if ((indice + 1 < sizeVpos) && (indice < sizeVpos))
        { 
            t /= time_[indice];
            T x,y,z;
            x =  t * position_[indice+1].x + (1 - t)* position_[indice].x;
            y =  t * position_[indice+1].y + (1 - t)* position_[indice].y;
            z =  t * position_[indice+1].z + (1 - t)* position_[indice].z;
            return {x,y,z};
        }
        else
        {
            return position_.back();		
        }
    }
    bool isClosed;
private:
    std::vector<glm::vec3>  position_;
    std::vector<T>          time_;
};


}  // end of S3DE namespace
