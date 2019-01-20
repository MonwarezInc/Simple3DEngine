#include "Mesh.h"

using namespace S3DE;

Mesh::Mesh( MeshImplType const& meshImplType )
    : impl_{createMesh( meshImplType )}
    , meshType_{meshImplType}
{
}


Mesh::Mesh( Mesh const& mesh )
    : impl_{createMesh( mesh.meshType_ )}
    , meshType_{mesh.meshType_}
{
    if ( !mesh.filename_.empty() )
    {
        filename_ = mesh.filename_;
        impl_->loadFromFile( filename_ );
    }
}

void Mesh::loadFromFile( const std::string& filename )
{
    filename_ = filename;
    impl_->loadFromFile( filename );
}

void Mesh::draw( std::chrono::duration<float, std::chrono::seconds::period> elapsedTime,
                 Shader const& shader, std::string const& animation )
{
    impl_->draw( elapsedTime, shader, animation );
}
