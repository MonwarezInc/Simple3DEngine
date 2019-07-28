#pragma once

#include "MeshImpl.h"
#include "Texture.h"
#include "Vertex.h"

#include <map>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace S3DE
{
inline glm::mat4 aiMatrixToMat4( aiMatrix4x4 const& src );
inline glm::mat4 aiMatrixToMat4( aiMatrix3x3 const& src );

class MeshAssimpImpl : public MeshImpl
{
public:
    MeshAssimpImpl();
    /// \brief Copy constructor
    /// \param  The mesh to be copied, not that it will simply do LoadFromFile with m.filename
    MeshAssimpImpl( MeshAssimpImpl const& m );
    virtual ~MeshAssimpImpl();
    /// \brief  Load a mesh from a file
    ///
    /// This function can launch exception and assertion, the loader are really strict. <br/>
    /// Loading a resource that is actually loaded may lead to unbehavior issue.
    /// \param  filename    filename of the Mesh to load
    virtual void loadFromFile( std::string const& filename ) override;
    /// \brief  Draw the mesh with the animation specified WIP
    ///
    /// For the moment only the first animation is supported, need some resource with multiple
    /// animation <br/>
    /// which can be loaded, and need to implement a correct search animation id by name function
    /// \param  elapsed_time    The elapsed time since the beginning of the application
    /// \param  shader A reference to set some information to the shader (like bone
    /// information
    /// etc...)
    /// \param  animation  The name of the animation to play
    virtual void draw( std::chrono::duration<float, std::chrono::seconds::period> elapsed_time,
                       Shader const& shader, std::string const& animation ) override;

private:
#define NUM_BONES_PER_VERTEX 4
#define INVALID_MATERIAL 0xFFFFFFFF

    struct BoneInfo
    {
        glm::mat4 BoneOffset;
        glm::mat4 FinalTransformation;

        BoneInfo()
        {
            BoneOffset          = glm::mat4( 0 );
            FinalTransformation = glm::mat4( 0 );
        }
    };
    struct VertexBoneData
    {
        uint IDs[ NUM_BONES_PER_VERTEX ];
        float Weights[ NUM_BONES_PER_VERTEX ];

        VertexBoneData()
        {
            for ( unsigned int i = 0; i < NUM_BONES_PER_VERTEX; ++i )
            {
                IDs[ i ]     = 0;
                Weights[ i ] = 0;
            }
        }
        void AddBoneData( unsigned int boneID, float weight );
    };
    void clear_();
    bool initFromScene_( const aiScene* pScene );
    void initMesh_( unsigned int index, const aiMesh* paiMesh, unsigned int BaseVertex,
                    unsigned int BaseIndex );
    bool initMaterials_( const aiScene* pScene );
    void loadBones_( unsigned int index, const aiMesh*, std::vector<VertexBoneData>& bones );
    void boneTransform_( float TimeInSec, std::vector<glm::mat4>& Transforms,
                         unsigned int idAnimation );
    void readNodeHierarchy_( float AnimationTime, const aiNode* pNode,
                             glm::mat4 const& ParentTransform, unsigned int idAnimation );

    void calcInterpolatedScaling_( aiVector3D& Out, float AnimationTime,
                                   const aiNodeAnim* pNodeAnim );
    void calcInterpolatedRotation_( aiQuaternion& Out, float AnimationTime,
                                    const aiNodeAnim* pNodeAnim );
    void calcInterpolatedPosition_( aiVector3D& Out, float AnimationTime,
                                    const aiNodeAnim* pNodeAnim );

    const aiNodeAnim* findNodeAnim_( const aiAnimation* pAnimation, std::string const& NodeName );
    unsigned int findScaling_( float AnimationTime, const aiNodeAnim* pNodeAnim );
    unsigned int findRotation_( float AnimationTime, const aiNodeAnim* pNodeAnim );
    unsigned int findPosition_( float AnimationTime, const aiNodeAnim* pNodeAnim );

    unsigned int getAnimationIndex_( std::string const& animation );
    void throwIfNotBetween0And1_( float Factor, std::string const& file, int line );
    struct MeshEntry
    {
        MeshEntry() = default;
        ~MeshEntry();

        void Init( std::vector<Vertex> const& vertices, std::vector<unsigned int> const& indices,
                   std::vector<VertexBoneData> const& bones );
        GLuint VAO{0};
        GLuint VB{0};
        GLuint IB{0};
        GLuint BONES{INVALID_MATERIAL};
        unsigned int NumIndices{0};
        unsigned int MaterialIndex{0};
        unsigned int BaseVertex{0};
        unsigned int BaseIndex{0};
        int skinned{0};
    };
    std::string filename_;
    std::vector<MeshEntry> entries_;
    std::vector<Texture> textures_;
    const aiScene* pScene_;
    Assimp::Importer importer_;
    glm::mat4 globalInverseTransform_;
    std::map<std::string, unsigned int> boneMapping_;
    unsigned int numBones_;
    std::vector<BoneInfo> boneInfos_;
    std::string animationName_;
};
} // namespace S3DE
