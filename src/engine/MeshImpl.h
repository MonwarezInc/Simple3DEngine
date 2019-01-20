#pragma once

#include "Shader.h"
#include <chrono>
#include <string>

namespace S3DE
{
class MeshImpl
{
public:
    virtual ~MeshImpl() = default;

    virtual void loadFromFile( std::string const& filename ) = 0;

    virtual void draw( std::chrono::duration<float, std::chrono::seconds::period> elapsed_time,
                       Shader const& shader, std::string const& animation )
        = 0;
};
} // namespace S3DE
