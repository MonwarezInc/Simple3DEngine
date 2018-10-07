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

#include "Camera.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


namespace S3DE
{
template <typename InputT, typename KeyCodeT>
Camera<InputT, KeyCodeT>::Camera( CameraKey<KeyCodeT> const &cameraKey )
    : m_phi( 0.0 )
    , m_theta( 0.0 )
    , m_orientation()
    , m_up( 0, 0, 1 )
    , m_lateralMove()
    , m_position()
    , m_target()
    , m_sensitive( 0.5 )
    , m_speed( 10 )
{
    this->CommonConstructor( cameraKey );
}
template <typename InputT, typename KeyCodeT>
Camera<InputT, KeyCodeT>::Camera( CameraKey<KeyCodeT> const &cameraKey, glm::vec3 position,
                                  glm::vec3 target, glm::vec3 up )
    : m_phi( 0.0 )
    , m_theta( 0.0 )
    , m_orientation()
    , m_up( up )
    , m_lateralMove()
    , m_position( position )
    , m_target( target )
    , m_sensitive( 0.5 )
    , m_speed( 0.1 )
{
    this->CommonConstructor( cameraKey );
}
template <typename InputT, typename KeyCodeT>
void Camera<InputT, KeyCodeT>::CommonConstructor( CameraKey<KeyCodeT> const &cameraKey )
{
    this->SetTarget( m_target );

    // Keyboard configuation
    // Key init
    // m_keyconf[ "forward" ]  = SDL_SCANCODE_W;
    // m_keyconf[ "backward" ] = SDL_SCANCODE_S;
    // m_keyconf[ "left" ]     = SDL_SCANCODE_A;
    // m_keyconf[ "right" ]    = SDL_SCANCODE_D;

    m_keyconf[ "forward" ]  = cameraKey.forward;
    m_keyconf[ "backward" ] = cameraKey.backward;
    m_keyconf[ "left" ]     = cameraKey.left;
    m_keyconf[ "right" ]    = cameraKey.right;

    // Keystates init
    m_keystat[ m_keyconf[ "forward" ] ]  = false;
    m_keystat[ m_keyconf[ "backward" ] ] = false;
    m_keystat[ m_keyconf[ "left" ] ]     = false;
    m_keystat[ m_keyconf[ "right" ] ]    = false;
}
template <typename InputT, typename KeyCodeT>
void Camera<InputT, KeyCodeT>::Orient( int xRel, int yRel )
{
    m_phi += -yRel * m_sensitive;
    m_theta += -xRel * m_sensitive;

    if ( m_theta >= 360 )
        m_theta = 0;
    else if ( m_theta <= -360 )
        m_theta = 0;

    if ( m_phi > 89.0 )
        m_phi = 89.0;
    else if ( m_phi < -89.0 )
        m_phi = -89.0;
    float phiRad   = m_phi * M_PI / 180.0;
    float thetaRad = m_theta * M_PI / 180.0;

    if ( m_up.x == 1.0 )
    {
        m_orientation.x = sin( phiRad );
        m_orientation.y = cos( phiRad ) * cos( thetaRad );
        m_orientation.z = cos( phiRad ) * sin( thetaRad );
    }
    else if ( m_up.y == 1.0 )
    {
        m_orientation.x = cos( phiRad ) * sin( thetaRad );
        m_orientation.y = sin( phiRad );
        m_orientation.z = cos( phiRad ) * cos( thetaRad );
    }
    else
    {
        m_orientation.x = cos( phiRad ) * cos( thetaRad );
        m_orientation.y = cos( phiRad ) * sin( thetaRad );
        m_orientation.z = sin( phiRad );
    }

    m_lateralMove = cross( m_up, m_orientation );
    m_lateralMove = normalize( m_lateralMove );

    m_target = m_position + m_orientation;
}
template <typename InputT, typename KeyCodeT>
void Camera<InputT, KeyCodeT>::KeyBoardEvent( InputT const &event )
{
    for ( auto &key : m_keystat )
        key.second = event.GetTouche( key.first );
}
template <typename InputT, typename KeyCodeT>
void Camera<InputT, KeyCodeT>::Move(
    InputT const &event, std::chrono::duration<float, std::chrono::milliseconds::period> elapsed )
{
    auto time = elapsed.count();

    if ( m_keystat[ m_keyconf[ "forward" ] ] )
    {
        m_position += m_orientation * m_speed * time;
        m_target = m_position + m_orientation;
    }

    if ( m_keystat[ m_keyconf[ "left" ] ] )
    {
        m_position += m_lateralMove * m_speed * time;
        m_target = m_position + m_orientation;
    }
    if ( m_keystat[ m_keyconf[ "backward" ] ] )
    {
        m_position -= m_orientation * m_speed * time;
        m_target = m_position + m_orientation;
    }
    if ( m_keystat[ m_keyconf[ "right" ] ] )
    {
        m_position -= m_lateralMove * m_speed * time;
        m_target = m_position + m_orientation;
    }

    if ( event.MotionMouse() )
    {
        this->Orient( event.GetXRel(), event.GetYRel() );
    }
}
template <typename InputT, typename KeyCodeT>
void Camera<InputT, KeyCodeT>::SetTarget( glm::vec3 const &target )
{
    m_orientation = target - m_position;
    m_orientation = normalize( m_orientation );

    if ( m_up.x == 1.0 )
    {
        m_phi   = asin( m_orientation.x );
        m_theta = acos( ( m_orientation.y / cos( m_phi ) ) );

        if ( m_orientation.y < 0 )
            m_theta *= -1;
    }
    else if ( m_up.y == 1.0 )
    {
        m_phi   = asin( m_orientation.y );
        m_theta = acos( ( m_orientation.z / cos( m_phi ) ) );

        if ( m_orientation.y < 0 )
            m_theta *= -1;
    }
    else
    {
        m_phi   = asin( m_orientation.x );
        m_theta = acos( ( m_orientation.z / cos( m_phi ) ) );

        if ( m_orientation.y < 0 )
            m_theta *= -1;
    }

    m_phi *= 180.0 / M_PI;
    m_theta *= 180.0 / M_PI;
}
template <typename InputT, typename KeyCodeT>
void Camera<InputT, KeyCodeT>::SetPosition( glm::vec3 const &position )
{
    m_position = position;
}
template <typename InputT, typename KeyCodeT>
void Camera<InputT, KeyCodeT>::LookAt( glm::mat4 &modelview )
{
    modelview = glm::lookAt( m_position, m_target, m_up );
}
template <typename InputT, typename KeyCodeT>
float Camera<InputT, KeyCodeT>::GetSensitive() const
{
    return m_sensitive;
}
template <typename InputT, typename KeyCodeT>
float Camera<InputT, KeyCodeT>::GetSpeed() const
{
    return m_speed;
}
template <typename InputT, typename KeyCodeT>
void Camera<InputT, KeyCodeT>::SetSensitive( float sensitive )
{
    m_sensitive = std::fabs( sensitive );
}
template <typename InputT, typename KeyCodeT>
void Camera<InputT, KeyCodeT>::SetSpeed( float speed )
{
    m_speed = std::fabs( speed );
}

} // namespace S3DE
