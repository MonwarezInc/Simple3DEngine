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
#include <S3DE_Input.h>

CInput::CInput():m_x(0),m_y(0),m_xRel(0),m_yRel(0),m_terminer(false)
{
    for (unsigned i =0; i < SDL_NUM_SCANCODES; ++i)
        m_touches[i]        =   false;

    for (unsigned int i=0; i < 8 ; ++i)
        m_boutonSouris[i]   =   false;

}
CInput::~CInput()
{

}
void    CInput::UpdateEvent()
{
    m_xRel  =   0;
    m_yRel  =   0;
    while (SDL_PollEvent(&m_event))
    {
        switch (m_event.type)
        {
            case SDL_KEYDOWN:
                m_touches[m_event.key.keysym.scancode]  =   true;
                break;

            case SDL_KEYUP:
                m_touches[m_event.key.keysym.scancode]  =   false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_boutonSouris[m_event.button.button]   =   true;
                break;

            case SDL_MOUSEBUTTONUP:
                m_boutonSouris[m_event.button.button]   =   false;
                break;

            case    SDL_MOUSEMOTION:
                m_x                                     =   m_event.motion.x;
                m_y                                     =   m_event.motion.y;
                m_xRel                                  =   m_event.motion.xrel;
                m_yRel                                  =   m_event.motion.yrel;
                break;

            case SDL_WINDOWEVENT:
                if (m_event.window.event    ==  SDL_WINDOWEVENT_CLOSE)
                    m_terminer  =   true;
            default:
                break;
        }


    }
}
bool       CInput::terminer() const
{
    return m_terminer;
}
bool       CInput::GetTouche(const SDL_Scancode touche) const
{
    return m_touches[touche];
}
bool        CInput::GetBoutonSouris(const Uint8 bouton) const
{
    return m_boutonSouris[bouton];
}
bool        CInput::MotionMouse() const
{
    if (m_xRel == 0 && m_yRel == 0)
        return false;
    else
        return true;
}
int         CInput::GetX() const
{
    return m_x;
}
int         CInput::GetY() const
{
    return m_y;
}
int         CInput::GetXRel() const
{
    return m_xRel;
}
int         CInput::GetYRel() const
{
    return m_yRel;
}
void        CInput::ShowCursor(bool reponse) const
{
    reponse ? SDL_ShowCursor(SDL_ENABLE) : SDL_ShowCursor(SDL_DISABLE);
}
void        CInput::GrabCursor(bool reponse) const
{
    reponse ? SDL_SetRelativeMouseMode(SDL_TRUE) : SDL_SetRelativeMouseMode(SDL_FALSE);
}
