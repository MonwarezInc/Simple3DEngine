
#include "MeshImplFactory.h"

#include "MeshAssimpImpl.h"

#include <memory>


std::unique_ptr<S3DE::MeshImpl> S3DE::createMesh( S3DE::MeshImplType const& meshImplType )
{
    switch ( meshImplType )
    {
        case S3DE::MeshImplType::assimp: return std::make_unique<S3DE::MeshAssimpImpl>(); break;
    }
}
