#pragma once

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex
{
    glm::vec3 m_pos;
    glm::vec2 m_tex;
    glm::vec3 m_normal;

    Vertex() {}

    Vertex( glm::vec3 const& pos, glm::vec2 const& tex, glm::vec3 const& normal )
    {
        m_pos    = pos;
        m_tex    = tex;
        m_normal = normal;
    }
};
