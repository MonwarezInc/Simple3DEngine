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
#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED
#include <SDL.h>

class CInput
{
    public:
        CInput();
        virtual             ~CInput();
        virtual void        UpdateEvent();
        virtual bool        terminer() const;
        virtual bool        GetTouche(const SDL_Scancode touche) const;
        virtual bool        GetBoutonSouris(const Uint8 bouton) const;
        virtual bool        MotionMouse() const;
        virtual int         GetX() const;
        virtual int         GetY() const;
        virtual int         GetXRel() const;
        virtual int         GetYRel() const;
        virtual void        ShowCursor(bool reponse) const;
        virtual void        GrabCursor(bool reponse) const;

    protected:
        SDL_Event       m_event;
        bool            m_touches[SDL_NUM_SCANCODES];
        bool            m_boutonSouris[8];

        int             m_x;
        int             m_y;
        int             m_xRel;
        int             m_yRel;

        bool            m_terminer;

};


#endif // INPUT_H_INCLUDED
