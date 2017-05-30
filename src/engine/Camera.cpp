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
#include <S3DE_Camera.h>
#include <cmath>
#include <iostream>

using namespace S3DE;
using glm::vec3;
using glm::mat4;
Camera::Camera()
    : m_phi(0.0)
    , m_theta(0.0)
    , m_orientation()
    , m_up(0, 0, 1)
    , m_lateralMove()
    , m_position()
    , m_target()
    , m_sensitive(0.5)
    , m_speed(10)
{
    this->CommonConstructor();
}
Camera::Camera(vec3 position, vec3 target, vec3 up)
    : m_phi(0.0)
    , m_theta(0.0)
    , m_orientation()
    , m_up(up)
    , m_lateralMove()
    , m_position(position)
    , m_target(target)
    , m_sensitive(0.5)
    , m_speed(0.1)
{
    this->CommonConstructor();
}
void Camera::CommonConstructor()
{
    this->SetTarget(m_target);

    // Keyboard configuation
    // Key init
    m_keyconf["forward"]  = SDL_SCANCODE_W;
    m_keyconf["backward"] = SDL_SCANCODE_S;
    m_keyconf["left"]     = SDL_SCANCODE_A;
    m_keyconf["right"]    = SDL_SCANCODE_D;
    // Keystates init
    m_keystat[m_keyconf["forward"]]  = false;
    m_keystat[m_keyconf["backward"]] = false;
    m_keystat[m_keyconf["left"]]     = false;
    m_keystat[m_keyconf["right"]]    = false;
}
void Camera::Orient(int xRel, int yRel)
{
    m_phi += -yRel * m_sensitive;
    m_theta += -xRel * m_sensitive;

    if (m_theta >= 360)
        m_theta = 0;
    else if (m_theta <= -360)
        m_theta = 0;

    if (m_phi > 89.0)
        m_phi = 89.0;
    else if (m_phi < -89.0)
        m_phi      = -89.0;
    float phiRad   = m_phi * M_PI / 180.0;
    float thetaRad = m_theta * M_PI / 180.0;

    if (m_up.x == 1.0)
    {
        m_orientation.x = sin(phiRad);
        m_orientation.y = cos(phiRad) * cos(thetaRad);
        m_orientation.z = cos(phiRad) * sin(thetaRad);
    }
    else if (m_up.y == 1.0)
    {
        m_orientation.x = cos(phiRad) * sin(thetaRad);
        m_orientation.y = sin(phiRad);
        m_orientation.z = cos(phiRad) * cos(thetaRad);
    }
    else
    {
        m_orientation.x = cos(phiRad) * cos(thetaRad);
        m_orientation.y = cos(phiRad) * sin(thetaRad);
        m_orientation.z = sin(phiRad);
    }

    m_lateralMove = cross(m_up, m_orientation);
    m_lateralMove = normalize(m_lateralMove);

    m_target = m_position + m_orientation;
}
void Camera::KeyBoardEvent(CInput const &event)
{
    for (auto &key : m_keystat)
        key.second = event.GetTouche(key.first);
}
void Camera::Move(CInput const &event,
                  std::chrono::duration<float, std::chrono::milliseconds::period> elapsed)
{
    auto time = elapsed.count();

    if (m_keystat[m_keyconf["forward"]])
    {
        m_position += m_orientation * m_speed * time;
        m_target = m_position + m_orientation;
    }

    if (m_keystat[m_keyconf["left"]])
    {
        m_position += m_lateralMove * m_speed * time;
        m_target = m_position + m_orientation;
    }
    if (m_keystat[m_keyconf["backward"]])
    {
        m_position -= m_orientation * m_speed * time;
        m_target = m_position + m_orientation;
    }
    if (m_keystat[m_keyconf["right"]])
    {
        m_position -= m_lateralMove * m_speed * time;
        m_target = m_position + m_orientation;
    }

    if (event.MotionMouse())
    {
        this->Orient(event.GetXRel(), event.GetYRel());
    }
}
void Camera::SetTarget(vec3 const &target)
{
    m_orientation = target - m_position;
    m_orientation = normalize(m_orientation);

    if (m_up.x == 1.0)
    {
        m_phi   = asin(m_orientation.x);
        m_theta = acos((m_orientation.y / cos(m_phi)));

        if (m_orientation.y < 0)
            m_theta *= -1;
    }
    else if (m_up.y == 1.0)
    {
        m_phi   = asin(m_orientation.y);
        m_theta = acos((m_orientation.z / cos(m_phi)));

        if (m_orientation.y < 0)
            m_theta *= -1;
    }
    else
    {
        m_phi   = asin(m_orientation.x);
        m_theta = acos((m_orientation.z / cos(m_phi)));

        if (m_orientation.y < 0)
            m_theta *= -1;
    }

    m_phi *= 180.0 / M_PI;
    m_theta *= 180.0 / M_PI;
}
void Camera::SetPosition(vec3 const &position)
{
    m_position = position;
}
void Camera::LookAt(mat4 &modelview)
{
    modelview = glm::lookAt(m_position, m_target, m_up);
}
float Camera::GetSensitive() const
{
    return m_sensitive;
}
float Camera::GetSpeed() const
{
    return m_speed;
}
void Camera::SetSensitive(float sensitive)
{
    m_sensitive = std::fabs(sensitive);
}
void Camera::SetSpeed(float speed)
{
    m_speed = std::fabs(speed);
}
