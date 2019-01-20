#pragma once

#include "MeshImpl.h"
#include <memory>

namespace S3DE
{
enum class MeshImplType { assimp };

std::unique_ptr<MeshImpl> createMesh( MeshImplType const& meshImplType );

} // namespace S3DE
