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
#include "MeshAssimpImpl.h"
#include <cassert>
#include <glm/gtc/matrix_inverse.hpp>
#include <iostream>
#include <sstream>
#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 2
#define NORMAL_LOCATION 3
#define BONE_ID_LOCATION 4
#define BONE_WEIGHT_LOCATION 5

// Defining it in a general header
#define BUFFER_OFFSET( i ) reinterpret_cast<char*>( i )

using namespace S3DE;
MeshAssimpImpl::MeshAssimpImpl()
{
    pScene_   = nullptr;
    numBones_ = 0;
}
MeshAssimpImpl::MeshAssimpImpl( MeshAssimpImpl const& mesh )
{
    pScene_   = nullptr;
    numBones_ = 0;
    this->loadFromFile( mesh.filename_ );
}
MeshAssimpImpl::~MeshAssimpImpl()
{
    this->clear_();
}
void MeshAssimpImpl::loadFromFile( std::string const& filename )
{
    filename_ = filename;
    // release the previously loaded mesh (if exists)
    this->clear_();

    Assimp::Importer importer;
    pScene_ = importer_.ReadFile( filename.c_str(),
                                  aiProcess_Triangulate | aiProcess_GenSmoothNormals
                                      | aiProcess_FlipUVs | aiProcess_LimitBoneWeights
                                      | aiProcess_SplitByBoneCount | aiProcess_SplitLargeMeshes );
    if ( pScene_ != nullptr )
    {
        globalInverseTransform_ = aiMatrixToMat4( pScene_->mRootNode->mTransformation );
        globalInverseTransform_ = glm::inverse( globalInverseTransform_ );

        // can launch an except
        if ( !this->initFromScene_( pScene_ ) )
        {
            std::stringstream out;
            out << "error init from scene at: " << __FILE__ << "( " << __LINE__ << ")";
            throw out.str();
        }
    }
    else
        throw std::string( "Error parsing" + filename + " : " + importer_.GetErrorString() );
}
bool MeshAssimpImpl::initFromScene_( const aiScene* pScene )
{
    entries_.resize( pScene->mNumMeshes );
    textures_.resize( pScene->mNumMaterials );

    // Initialize the meshes in the scene one by one
    for ( unsigned int i = 0; i < entries_.size(); ++i )
    {
        auto paiMesh = pScene->mMeshes[ i ];
        this->initMesh_( i, paiMesh, 0, 0 ); // 0 because it's one VAO by meshEntrie
    }

    return initMaterials_( pScene );
}
void MeshAssimpImpl::draw( std::chrono::duration<float, std::chrono::seconds::period> elapsed_time,
                           const Shader& shader, std::string const& animation )
{
    std::vector<glm::mat4> transforms;
    // this->BoneTransform((static_cast<float>(elapsed_time))/1000.0f, transforms,
    // GetAnimationIndex(animation));
    this->boneTransform_( elapsed_time.count(), transforms, getAnimationIndex_( animation ) );
    for ( unsigned int i = 0; i < transforms.size(); ++i )
    {
        // we have to send unform matrix
        std::stringstream out;
        out << "Bones[" << i << "]";
        GLuint location = shader.GetUniformLocation( out.str().c_str() );
        glUniformMatrix4fv( location, 1, GL_FALSE,
                            (const GLfloat*)glm::value_ptr( transforms[ i ] ) );
    }
    for ( auto& entries : entries_ )
    {
        glBindVertexArray( entries.VAO );
        auto materialIndex = entries.MaterialIndex;
        glUniform1i( shader.GetUniformLocation( "skinned" ), entries.skinned );
        if ( materialIndex < textures_.size() )
        {
            glBindTexture( GL_TEXTURE_2D, textures_[ materialIndex ].GetID() );
        }
        glDrawElements( GL_TRIANGLES, entries.NumIndices, GL_UNSIGNED_INT, 0 );
    }
    glBindVertexArray( 0 ); // OpenGL state machine
}
void MeshAssimpImpl::clear_()
{
    pScene_   = nullptr;
    numBones_ = 0;
    entries_.clear();
    textures_.clear();
}
void MeshAssimpImpl::initMesh_( unsigned int index, const aiMesh* paiMesh, unsigned int BaseVertex,
                                unsigned int BaseIndex )
{
    entries_[ index ].MaterialIndex = paiMesh->mMaterialIndex;
    entries_[ index ].NumIndices    = paiMesh->mNumFaces * 3;
    entries_[ index ].BaseVertex    = BaseVertex;
    entries_[ index ].BaseIndex     = BaseIndex;

    // if skinned
    entries_[ index ].skinned = ( paiMesh->mNumBones > 0 ) ? 1 : 0;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<VertexBoneData> bones;

    unsigned int NumVertices = 0;
    unsigned int NumIndices  = 0;

    NumVertices = paiMesh->mNumVertices;
    NumIndices  = 3 * paiMesh->mNumFaces;

    vertices.reserve( NumVertices );
    bones.resize( NumVertices );
    indices.reserve( NumIndices );
    aiVector3D const zero3D( 0.0f, 0.0f, 0.0f );

    for ( unsigned int i = 0; i < paiMesh->mNumVertices; ++i )
    {
        // set vertex information
        auto pPos    = &( paiMesh->mVertices[ i ] );
        auto pNormal = &( paiMesh->mNormals[ i ] );
        auto pTexCoord
            = paiMesh->HasTextureCoords( 0 ) ? &( paiMesh->mTextureCoords[ 0 ][ i ] ) : &zero3D;
        Vertex v( glm::vec3( pPos->x, pPos->y, pPos->z ), glm::vec2( pTexCoord->x, pTexCoord->y ),
                  glm::vec3( pNormal->x, pNormal->y, pNormal->z ) );

        vertices.push_back( v );
    }
    for ( unsigned int i = 0; i < paiMesh->mNumFaces; ++i )
    {
        // set indice information
        auto face = paiMesh->mFaces[ i ];
        assert( face.mNumIndices == 3 );
        indices.push_back( face.mIndices[ 0 ] );
        indices.push_back( face.mIndices[ 1 ] );
        indices.push_back( face.mIndices[ 2 ] );
    }

    this->loadBones_( index, paiMesh, bones );

    entries_[ index ].Init( vertices, indices, bones );
}

bool MeshAssimpImpl::initMaterials_( const aiScene* pScene )
{
    // extract the directory part from the filename
    std::string::size_type slashIndex = filename_.find_last_of( "/" );
    std::string dir;
    if ( slashIndex == std::string::npos )
        dir = ".";
    else if ( slashIndex == 0 )
        dir = "/";
    else
        dir = filename_.substr( 0, slashIndex );

    bool ret = true;

    // initialize the materials
    for ( unsigned int i = 0; i < pScene->mNumMaterials; ++i )
    {
        auto pMaterial = pScene->mMaterials[ i ];


        if ( pMaterial->GetTextureCount( aiTextureType_DIFFUSE ) > 0 )
        {
            aiString path;
            if ( pMaterial->GetTexture( aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL,
                                        NULL )
                 == AI_SUCCESS )
            {
                std::string fullPath = dir + "/" + path.data;

                textures_[ i ].SetFilename( fullPath );

                if ( !textures_[ i ].Load() )
                {
                    // maybe use except mechanism ?
                    std::cerr << "Error loading texture " << fullPath << '\n';
                    std::cerr << "in file: " << __FILE__ << "(" << __LINE__ << ")" << '\n';
                    ret = false;
                }
            }
        }
        // load a white texture in case the model didn't have it's own texture
        else
        {
            textures_[ i ].SetFilename( "./white.png" );
            ret = textures_[ i ].Load();
        }
    }

    return ret;
}
MeshAssimpImpl::MeshEntry::~MeshEntry()
{
    if ( 0 != VB )
        glDeleteBuffers( 1, &VB );
    if ( 0 != IB )
        glDeleteBuffers( 1, &IB );
    if ( 0 != VAO )
        glDeleteVertexArrays( 1, &VAO );
    if ( 0 != BONES )
        glDeleteVertexArrays( 1, &BONES );
}
void MeshAssimpImpl::MeshEntry::Init( std::vector<Vertex> const& vertices,
                                      std::vector<unsigned int> const& indices,
                                      std::vector<VertexBoneData> const& bones )
{
    NumIndices = indices.size();
    glGenBuffers( 1, &VB );
    glGenBuffers( 1, &IB );
    glGenBuffers( 1, &BONES );
    glGenVertexArrays( 1, &VAO );


    // Vertex Array Object to OpenGL 3.2 core profile
    glBindVertexArray( VAO );

    glBindBuffer( GL_ARRAY_BUFFER, VB );
    glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex ) * vertices.size(), vertices.data(),
                  GL_STATIC_DRAW );

    glVertexAttribPointer( POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), 0 );
    glVertexAttribPointer( TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ),
                           BUFFER_OFFSET( 12 ) );
    glVertexAttribPointer( NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),
                           BUFFER_OFFSET( 20 ) );

    glEnableVertexAttribArray( POSITION_LOCATION );
    glEnableVertexAttribArray( TEX_COORD_LOCATION );
    glEnableVertexAttribArray( NORMAL_LOCATION );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IB );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * NumIndices, indices.data(),
                  GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, BONES );
    glBufferData( GL_ARRAY_BUFFER, sizeof( bones[ 0 ] ) * bones.size(), bones.data(),
                  GL_STATIC_DRAW );


    glVertexAttribIPointer( BONE_ID_LOCATION, 4, GL_INT, sizeof( VertexBoneData ),
                            BUFFER_OFFSET( 0 ) );
    glVertexAttribPointer( BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof( VertexBoneData ),
                           BUFFER_OFFSET( 16 ) );

    glEnableVertexAttribArray( BONE_ID_LOCATION );
    glEnableVertexAttribArray( BONE_WEIGHT_LOCATION );
    glBindVertexArray( 0 );
}
void MeshAssimpImpl::loadBones_( unsigned int index, const aiMesh* pMesh,
                                 std::vector<VertexBoneData>& bones )
{
    assert( pMesh != nullptr );
    for ( unsigned int i = 0; i < pMesh->mNumBones; ++i )
    {
        // Assert
        assert( pMesh->mBones[ i ] != nullptr );
        unsigned int BoneIndex = 0;
        std::string BoneName( pMesh->mBones[ i ]->mName.data );

        if ( boneMapping_.find( BoneName ) == boneMapping_.end() )
        {
            BoneIndex = numBones_;
            ++numBones_;
            BoneInfo bi;
            boneInfos_.push_back( bi );
        }
        else
        {
            BoneIndex = boneMapping_[ BoneName ];
        }
        boneMapping_[ BoneName ] = BoneIndex;
        // We need to check if BoneIndex is < m_BoneInfo.size()
        // Which can occur some times
        if ( BoneIndex >= boneInfos_.size() )
        {
            std::stringstream out;
            out << "Error BoneIndex is: " << BoneIndex << " but m_BoneInfo have "
                << boneInfos_.size() << " elements";
            out << "\n"
                << "Error in " << __FILE__ << "(" << __LINE__ << ")";
            throw out.str();
        }
        boneInfos_[ BoneIndex ].BoneOffset = aiMatrixToMat4( pMesh->mBones[ i ]->mOffsetMatrix );

        for ( unsigned int j = 0; j < pMesh->mBones[ i ]->mNumWeights; ++j )
        {
            unsigned int VertexID
                = entries_[ index ].BaseVertex + pMesh->mBones[ i ]->mWeights[ j ].mVertexId;
            float Weight = pMesh->mBones[ i ]->mWeights[ j ].mWeight;
            if ( VertexID < bones.size() )
                bones[ VertexID ].AddBoneData( BoneIndex, Weight );
            else
            {
                std::stringstream out;
                out << "Error bones have " << bones.size()
                    << " elements , but VertexID is equal to " << VertexID;
                out << "\n"
                    << "Error in " << __FILE__ << " (" << __LINE__ << ")";
                throw out.str();
            }
        }
    }
}
void MeshAssimpImpl::VertexBoneData::AddBoneData( unsigned int BoneID, float Weight )
{
    for ( unsigned int i = 0; i < NUM_BONES_PER_VERTEX; ++i )
    {
        if ( Weights[ i ] == 0.0 )
        {
            IDs[ i ]     = BoneID;
            Weights[ i ] = Weight;

            return;
        }
    }
    // if we have more than NUM_BONES_PER_VERTEX
    std::stringstream out;
    out << "Error we have more than " << NUM_BONES_PER_VERTEX << " Bones";
    throw out.str();
}
void MeshAssimpImpl::boneTransform_( float TimeInSec, std::vector<glm::mat4>& Transforms,
                                     unsigned int idAnimation )
{
    if ( pScene_->HasAnimations() )
    {
        if ( pScene_->mAnimations[ idAnimation ] != nullptr )
        {
            glm::mat4 Identity = glm::mat4();

            auto TicksPerSec = pScene_->mAnimations[ idAnimation ]->mTicksPerSecond;

            auto TimeInTicks = TimeInSec * TicksPerSec;
            auto AnimationTime
                = fmod( TimeInTicks, pScene_->mAnimations[ idAnimation ]->mDuration );

            this->readNodeHierarchy_( AnimationTime, pScene_->mRootNode, Identity, idAnimation );

            Transforms.resize( numBones_ );
            for ( unsigned int i = 0; i < numBones_; ++i )
            {
                Transforms[ i ] = boneInfos_[ i ].FinalTransformation;
            }
        }
    }
}
void MeshAssimpImpl::readNodeHierarchy_( float AnimationTime, const aiNode* pNode,
                                         glm::mat4 const& ParentTransform,
                                         unsigned int idAnimation )
{
    std::string NodeName( pNode->mName.data );

    auto pAnimation = pScene_->mAnimations[ idAnimation ];
    glm::mat4 NodeTransformation;
    NodeTransformation = aiMatrixToMat4( pNode->mTransformation );

    auto pNodeAnim = this->findNodeAnim_( pAnimation, NodeName );

    if ( pNodeAnim != nullptr )
    {
        // Interpolate Scaling
        aiVector3D scaling;
        this->calcInterpolatedScaling_( scaling, AnimationTime, pNodeAnim );
        glm::mat4 scalingM;
        scalingM = glm::scale( scalingM, glm::vec3( scaling.x, scaling.y, scaling.z ) );
        // Interpolate rotation
        aiQuaternion rotationQ;
        this->calcInterpolatedRotation_( rotationQ, AnimationTime, pNodeAnim );
        glm::mat4 rotationM;
        rotationM = aiMatrixToMat4( rotationQ.GetMatrix() );
        // Interpolate translation
        aiVector3D translation;
        this->calcInterpolatedPosition_( translation, AnimationTime, pNodeAnim );
        glm::mat4 translationM;
        translationM = glm::translate( translationM,
                                       glm::vec3( translation.x, translation.y, translation.z ) );

        // Combine above transformation
        NodeTransformation = translationM * rotationM * scalingM;
    }
    glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

    if ( boneMapping_.find( NodeName ) != boneMapping_.end() )
    {
        unsigned int BoneIndex = boneMapping_[ NodeName ];
        boneInfos_[ BoneIndex ].FinalTransformation
            = globalInverseTransform_ * GlobalTransformation * boneInfos_[ BoneIndex ].BoneOffset;
    }

    for ( unsigned int i = 0; i < pNode->mNumChildren; ++i )
    {
        this->readNodeHierarchy_( AnimationTime, pNode->mChildren[ i ], GlobalTransformation,
                                  idAnimation );
    }
}

unsigned int MeshAssimpImpl::findScaling_( float AnimationTime, const aiNodeAnim* pNodeAnim )
{
    assert( pNodeAnim->mNumScalingKeys > 0 );
    for ( unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; ++i )
    {
        if ( AnimationTime < static_cast<float>( pNodeAnim->mScalingKeys[ i + 1 ].mTime ) )
        {
            return i;
        }
    }

    assert( 0 );

    return 0;
}
unsigned int MeshAssimpImpl::findRotation_( float AnimationTime, const aiNodeAnim* pNodeAnim )
{
    assert( pNodeAnim->mNumRotationKeys > 0 );
    for ( unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; ++i )
    {
        if ( AnimationTime < static_cast<float>( pNodeAnim->mRotationKeys[ i + 1 ].mTime ) )
            return i;
    }

    assert( 0 );
    return 0;
}
unsigned int MeshAssimpImpl::findPosition_( float AnimationTime, const aiNodeAnim* pNodeAnim )
{
    assert( pNodeAnim->mNumPositionKeys > 0 );
    for ( unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; ++i )
    {
        if ( AnimationTime < static_cast<float>( pNodeAnim->mPositionKeys[ i + 1 ].mTime ) )
            return i;
    }
    assert( 0 );
    return 0;
}
void MeshAssimpImpl::calcInterpolatedScaling_( aiVector3D& Out, float AnimationTime,
                                               const aiNodeAnim* pNodeAnim )
{
    if ( pNodeAnim->mNumScalingKeys == 1 )
    {
        Out = pNodeAnim->mScalingKeys[ 0 ].mValue;
        return;
    }

    auto ScalingIndex     = this->findScaling_( AnimationTime, pNodeAnim );
    auto NextScalingIndex = ScalingIndex + 1;

    assert( NextScalingIndex < pNodeAnim->mNumScalingKeys );

    auto DeltaTime = static_cast<float>( pNodeAnim->mScalingKeys[ NextScalingIndex ].mTime
                                         - pNodeAnim->mScalingKeys[ ScalingIndex ].mTime );
    auto Factor    = AnimationTime
                  - static_cast<float>( pNodeAnim->mScalingKeys[ ScalingIndex ].mTime / DeltaTime );

    this->checkFactor_( Factor, __FILE__, __LINE__ );

    const auto& Start = pNodeAnim->mScalingKeys[ ScalingIndex ].mValue;
    const auto& End   = pNodeAnim->mScalingKeys[ NextScalingIndex ].mValue;
    auto Delta        = End - Start;
    Out               = Start + Factor * Delta;
}
void MeshAssimpImpl::calcInterpolatedRotation_( aiQuaternion& Out, float AnimationTime,
                                                const aiNodeAnim* pNodeAnim )
{
    if ( pNodeAnim->mNumRotationKeys == 1 )
    {
        Out = pNodeAnim->mRotationKeys[ 0 ].mValue;
        return;
    }

    auto RotationIndex     = this->findRotation_( AnimationTime, pNodeAnim );
    auto NextRotationIndex = RotationIndex + 1;

    assert( NextRotationIndex < pNodeAnim->mNumRotationKeys );

    auto DeltaTime = static_cast<float>( pNodeAnim->mRotationKeys[ NextRotationIndex ].mTime
                                         - pNodeAnim->mRotationKeys[ RotationIndex ].mTime );
    auto Factor
        = AnimationTime
          - static_cast<float>( pNodeAnim->mRotationKeys[ RotationIndex ].mTime / DeltaTime );

    this->checkFactor_( Factor, __FILE__, __LINE__ );

    const auto& Start = pNodeAnim->mRotationKeys[ RotationIndex ].mValue;
    const auto& End   = pNodeAnim->mRotationKeys[ NextRotationIndex ].mValue;
    aiQuaternion::Interpolate( Out, Start, End, Factor );
    Out.Normalize();
}
void MeshAssimpImpl::calcInterpolatedPosition_( aiVector3D& Out, float AnimationTime,
                                                const aiNodeAnim* pNodeAnim )
{
    if ( pNodeAnim->mNumPositionKeys == 1 )
    {
        Out = pNodeAnim->mPositionKeys[ 0 ].mValue;
        return;
    }

    auto PositionIndex     = this->findPosition_( AnimationTime, pNodeAnim );
    auto NextPositionIndex = PositionIndex + 1;

    assert( NextPositionIndex < pNodeAnim->mNumPositionKeys );

    auto DeltaTime = static_cast<float>( pNodeAnim->mPositionKeys[ NextPositionIndex ].mTime
                                         - pNodeAnim->mPositionKeys[ PositionIndex ].mTime );
    auto Factor
        = AnimationTime
          - static_cast<float>( pNodeAnim->mPositionKeys[ PositionIndex ].mTime / DeltaTime );

    this->checkFactor_( Factor, __FILE__, __LINE__ );

    const auto& Start = pNodeAnim->mPositionKeys[ PositionIndex ].mValue;
    const auto& End   = pNodeAnim->mPositionKeys[ NextPositionIndex ].mValue;
    aiVector3D Delta  = End - Start;
    Out               = Start + Factor * Delta;
}
const aiNodeAnim* MeshAssimpImpl::findNodeAnim_( const aiAnimation* pAnimation,
                                                 std::string const& NodeName )
{
    for ( unsigned int i = 0; i < pAnimation->mNumChannels; ++i )
    {
        const auto pNodeAnim = pAnimation->mChannels[ i ];
        if ( pNodeAnim != nullptr )
        {
            if ( std::string( pNodeAnim->mNodeName.data ) == NodeName )
                return pNodeAnim;
        }
    }

    return nullptr;
}
unsigned int MeshAssimpImpl::getAnimationIndex_( std::string const& animation )
{
    unsigned int idAnimation = 0;
    // Some default case when there is no animation or when m_pScene is nullptr
    if ( pScene_ == nullptr )
        return idAnimation;
    if ( !pScene_->HasAnimations() )
        return idAnimation;
    //  Arbitrary set the different value for animation
    if ( animation == "walk" )
        idAnimation = 1;
    // Then check if the idAnimation is in a good range
    if ( idAnimation >= pScene_->mNumAnimations )
    {
        std::cerr << "Error in idAnimation range\n";
        std::stringstream out;
        out << "Error in idAnimation range, there is only: " << pScene_->mNumAnimations;
        out << " but the following id animation has request " << idAnimation << '\n';
        throw out.str();
    }
    return idAnimation;
}
inline glm::mat4 S3DE::aiMatrixToMat4( aiMatrix4x4 const& src )
{
    glm::mat4 dest;
    dest[ 0 ][ 0 ] = src.a1;
    dest[ 0 ][ 1 ] = src.b1;
    dest[ 0 ][ 2 ] = src.c1;
    dest[ 0 ][ 3 ] = src.d1;
    dest[ 1 ][ 0 ] = src.a2;
    dest[ 1 ][ 1 ] = src.b2;
    dest[ 1 ][ 2 ] = src.c2;
    dest[ 1 ][ 3 ] = src.d2;
    dest[ 2 ][ 0 ] = src.a3;
    dest[ 2 ][ 1 ] = src.b3;
    dest[ 2 ][ 2 ] = src.c3;
    dest[ 2 ][ 3 ] = src.d3;
    dest[ 3 ][ 0 ] = src.a4;
    dest[ 3 ][ 1 ] = src.b4;
    dest[ 3 ][ 2 ] = src.c4;
    dest[ 3 ][ 3 ] = src.d4;
    return dest;
}
inline glm::mat4 S3DE::aiMatrixToMat4( aiMatrix3x3 const& src )
{
    glm::mat4 dest;
    dest[ 0 ][ 0 ] = src.a1;
    dest[ 0 ][ 1 ] = src.b1;
    dest[ 0 ][ 2 ] = src.c1;
    dest[ 0 ][ 3 ] = 0;
    dest[ 1 ][ 0 ] = src.a2;
    dest[ 1 ][ 1 ] = src.b2;
    dest[ 1 ][ 2 ] = src.c2;
    dest[ 1 ][ 3 ] = 0;
    dest[ 2 ][ 0 ] = src.a3;
    dest[ 2 ][ 1 ] = src.b3;
    dest[ 2 ][ 2 ] = src.c3;
    dest[ 2 ][ 3 ] = 0;
    dest[ 3 ][ 0 ] = 0;
    dest[ 3 ][ 1 ] = 0;
    dest[ 3 ][ 2 ] = 0;
    dest[ 3 ][ 3 ] = 1;
    return dest;
}
void MeshAssimpImpl::checkFactor_( float Factor, std::string const& file, int line )
{
    if ( Factor < 0.0f || Factor > 1.0f )
    {
        std::stringstream out;
        out << "Error interpolated factor not in the good range , Factor is: " << Factor
            << " which is not in [0.0, 1.0] \n";
        out << "This happend in " << file << "(" << line << ")";
        throw out.str();
    }
}
