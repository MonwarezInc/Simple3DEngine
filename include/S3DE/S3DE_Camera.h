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

#include "S3DE_Input.h"
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
			/// \brief Construct the camera with some settings
			///	
			///	\param[in]	position	Set the initial position of the camera
			///	\param[in]	target		Set the target of the camera
			///	\param[in]	up			Set the up axis of the world
        	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up);
			///	\brief	Set the relative mouse motion
			///	\param	xRel	Set the x-axis relative motion
			///	\param	yRel	Set the y-axis relative motion
        	void                Orient(int xRel,int yRel);
			///	\brief	Move the camera, according of mouse event and keyboard event
			///
			///	The mouse motion event are checked here , for the keyboard event
			///	the map	KeyStates are checked to see which key are pressed/released
			///
			///	\param	event	the event for getting mouse move
			///	\param	elapsed	the current elapsed time
        	void                Move(CInput const &event, Uint32 elapsed);
			///	\brief	Update the camera KeyStates
			///	\param	event	the event to check
        	void                KeyBoardEvent(CInput const &event);
			///	\brief	Set the view matrix in modelview
			///	\param[out]	modelview	The view matrix get by the camera
        	void                LookAt(glm::mat4    &modelview);
			///	\brief Set the target of the camera
			///	\param	target	The target of the camera
        	void                SetTarget(glm::vec3 const &target);
			///	\brief Set the position of the camera
			///	\param	position	The position of the camera
			void				SetPosition(glm::vec3 const & position);
			///	\brief	Get the current sensitive value
       	 	float               GetSensitive() const;
			///	\brief	Get the current speed of the camera
        	float               GetSpeed() const;
			///	\brief	Set the sensitive of the camera
			///
			///	The sensitive will always be a positive value, if a negative value is set
			///	it will be the absolute value that will be set in m_sensitive
			///
			///	\param	sensitive	The sensitivty of the camera
        	void                SetSensitive(float sensitive);
			///	\brief	Set the speed of the camera
			///
			///	The speed will always be a positive value, if a negative value is set
			///	it will be the absolute value that will be set in m_speed
        	void                SetSpeed(float speed);
			///	\brief	Return the current target
        	glm::vec3           GetTarget()const {return m_target;}
			///	\brief	Return the current position
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
