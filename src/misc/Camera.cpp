/*
Copyright (C) 2014-2015 Payet thibault

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#include "Camera.h"
#include <iostream>

using namespace glm;
Camera::Camera():m_phi(0.0),m_theta(0.0),m_orientation(), m_axeVertical(0,0,1), m_deplacementLateral(), m_position(), m_pointCible(), m_sensitive(0.5),
                m_speed(10)
{
	this->CommonConstructor();
}
Camera::Camera(vec3 position, vec3 pointCible, vec3 axeVertical):
                m_phi(0.0),m_theta(0.0),m_orientation(), m_axeVertical(axeVertical),
                m_deplacementLateral(), m_position(position), m_pointCible(pointCible), m_sensitive(0.5),m_speed(0.1)
{
	this->CommonConstructor();
}
void Camera::CommonConstructor()
{

    this->setPointCible(m_pointCible);

    // Keyboard configuation
    // Key	init
    m_keyconf["forward"]    =   SDL_SCANCODE_W;
    m_keyconf["backward"]   =   SDL_SCANCODE_S;
    m_keyconf["left"]       =   SDL_SCANCODE_A;
    m_keyconf["right"]      =   SDL_SCANCODE_D;
    // Keystates init
    m_keystat[m_keyconf["forward"]]     =   false;
    m_keystat[m_keyconf["backward"]]    =   false;
    m_keystat[m_keyconf["left"]]        =   false;
    m_keystat[m_keyconf["right"]]       =   false;


}
void Camera::orienter(int xRel, int yRel)
{
    m_phi               +=  -yRel * m_sensitive;
    m_theta             +=  -xRel * m_sensitive;

    if (m_theta >= 360)
        m_theta = 0;
    else if (m_theta <= -360)
        m_theta = 0;

    if (m_phi > 89.0)
        m_phi = 89.0;
    else if (m_phi < - 89.0)
        m_phi = - 89.0;
    float   phiRad      = m_phi * M_PI / 180.0;
    float   thetaRad    = m_theta*M_PI / 180.0;

    if (m_axeVertical.x == 1.0)
    {
        m_orientation.x         =   sin(phiRad);
        m_orientation.y         =   cos(phiRad)*cos(thetaRad);
        m_orientation.z         =   cos(phiRad)*sin(thetaRad);
    }
    else if (m_axeVertical.y == 1.0)
    {
        m_orientation.x         =   cos(phiRad) * sin (thetaRad);
        m_orientation.y         =   sin(phiRad);
        m_orientation.z         =   cos(phiRad) * cos(thetaRad);
    }
    else
    {
        m_orientation.x         =   cos(phiRad)*cos(thetaRad);
        m_orientation.y         =   cos(phiRad)*sin(thetaRad);
        m_orientation.z         =   sin(phiRad);
    }

    m_deplacementLateral        =   cross(m_axeVertical, m_orientation);
    m_deplacementLateral        =   normalize(m_deplacementLateral);

    m_pointCible                =   m_position  +   m_orientation;
}
void Camera::keyBoardEvent(const CInput &event)
{
    for (KeyStates::iterator it = m_keystat.begin(); it != m_keystat.end(); it++)
    {
        it->second  =   event.GetTouche(it->first);
        /*
        if (event.type == sf::Event::KeyPressed)
        {
            if (it->first == event.key.code)
                it->second = true;
        }
        else if (event.type  == sf::Event::KeyReleased)
        {
            if (it->first == event.key.code)
                it->second = false;
        }
        */
    }
}
void Camera::deplacer(const CInput   &event,  Uint32 elapsed)
{
    float time = elapsed;
    (void) time;

    if (m_keystat[m_keyconf["forward"]])
    {
        m_position      +=  m_orientation * m_speed*time;
        m_pointCible    =   m_position + m_orientation;
    }

    if (m_keystat[m_keyconf["left"]])
    {
        m_position      +=  m_deplacementLateral*m_speed*time;
        m_pointCible    =   m_position + m_orientation;
    }
    if (m_keystat[m_keyconf["backward"]])
    {
        m_position      -=  m_orientation * m_speed*time;
        m_pointCible    =   m_position + m_orientation;
    }
    if (m_keystat[m_keyconf["right"]])
    {
        m_position      -=  m_deplacementLateral*m_speed*time;
        m_pointCible    =   m_position + m_orientation;
    }

    if (event.MotionMouse())
    {

        orienter(event.GetXRel(), event.GetYRel());

    }

}
void Camera::setPointCible(const vec3& cible)
{
    //m_orientation           =   m_pointCible - m_position;
    m_orientation           =   cible - m_position;
    m_orientation           =   normalize(m_orientation);

    if (m_axeVertical.x == 1.0)
    {
        m_phi               =   asin(m_orientation.x);
        m_theta             =   acos((m_orientation.y / cos(m_phi)));

        if(m_orientation.y < 0)
            m_theta *= -1;

    }
    else if (m_axeVertical.y == 1.0)
    {
        m_phi               =   asin(m_orientation.y);
        m_theta             =   acos((m_orientation.z / cos(m_phi)));

        if(m_orientation.y < 0)
            m_theta *= -1;
    }
    else
    {
        m_phi               =   asin(m_orientation.x);
        m_theta             =   acos((m_orientation.z / cos(m_phi)));

        if(m_orientation.y < 0)
            m_theta *= -1;
    }

    m_phi                   *=  180.0 / M_PI ;
    m_theta                 *=  180.0 / M_PI ;
}
void Camera::lookAt(mat4 &modelview)
{
    modelview               =   glm::lookAt(m_position, m_pointCible, m_axeVertical);
}
float Camera::getSensitive() const
{
    return m_sensitive;
}
float Camera::getSpeed() const
{
    return m_speed;
}
void Camera::setSensitive(float sensitive)
{
    m_sensitive             =   sensitive;
}
void Camera::setSpeed(float speed)
{
    m_speed                 =   speed;
}
Camera::~Camera()
{
    //dtor
}
