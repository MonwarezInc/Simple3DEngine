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
#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED
#include <SDL2/SDL.h>

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
