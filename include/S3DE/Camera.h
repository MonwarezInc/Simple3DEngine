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
#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Input.h"
#include <string>
#include <map>
namespace S3DE
{
	typedef     std::map<SDL_Scancode, bool> KeyStates;
	typedef     std::map <std::string, SDL_Scancode> KeyConf;
	class Camera
	{
    	public:
        	Camera();
			/** \brief Constructor of Camera
			*	
			*	@param[in]	position	Set the initial position of the camera
			*	@param[in]	target		Set the target of the camera
			*	@param[in]	up			Set the up axis of the world
			*/
        	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up);
        	void                Orient(int xRel,int yRel);
        	void                Move(const CInput &event, Uint32 elapsed);
        	void                KeyBoardEvent(const CInput &event);
        	void                LookAt(glm::mat4    &modelview);
        	void                SetTarget(const glm::vec3 &target);

       	 	float               GetSensitive() const;
        	float               GetSpeed() const;

        	void                SetSensitive(float sensitive);
        	void                SetSpeed(float speed);
        	glm::vec3           GetTarget()const {return m_target;}
        	glm::vec3           GetPosition() const {return m_position;}
        	virtual ~Camera();
    	protected:
			void				CommonConstructor();
        	float               m_phi;
        	float               m_theta;
        	glm::vec3           m_orientation;

        	glm::vec3           m_up;
        	glm::vec3           m_lateralMove;

        	glm::vec3           m_position;
        	glm::vec3           m_target;

        	float               m_sensitive;
        	float               m_speed;


        	KeyConf             m_keyconf;
        	KeyStates           m_keystat;

    	private:
	};
}
#endif // CAMERA_H
