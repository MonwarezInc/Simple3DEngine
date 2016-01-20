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
