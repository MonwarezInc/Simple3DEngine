#ifndef Interpolate_H_INCLUED
#define Interpolate_H_INCLUED
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

// Curve interpolate
template <class T>
class Position3D
{
	public:
		T x;
		T y;
		T z;
		Position3D(T const & _x,T const & _y,T const & _z)
		{
			x	=	_x;	y	=	_y;	z	=	_z;
		}
};
template <class T>
class CurveInterpolate
{
	public:
		CurveInterpolate(){}
		virtual	Position3D<T> 	GetInterpolated(T totaltime) =0;
		virtual void			AddPoint(Position3D<T> const & pos, T time)
		{
			m_vposition3D.push_back(pos);
			m_time.push_back(time);
		}
	protected:
		std::vector <Position3D<T>>	m_vposition3D;
		std::vector <T>				m_time;
		bool						m_looped;
};
template <class T>
class LinearInterpolate : public CurveInterpolate<T>
{
	public:
		LinearInterpolate(){}
		virtual void 			AddPoint(Position3D<T> const & pos, T time)
		{
			m_vposition3D.push_back(pos);
			m_time.push_back(time);
		}
		virtual void			SetLooped(bool looped)
		{
			m_looped	=	looped;
		}
		virtual Position3D<T>	GetInterpolated(T totaltime)
		{
			auto	sizeVpos	=	m_vposition3D.size();
			T	currenttime	=	0;
			T	lasttime	=	0;
			T	t			=	0;
			auto	indice	=	0;
			if (m_looped)
			{
				for (auto i =0; i < sizeVpos; ++i)
					currenttime += m_time[indice];
				if (totaltime > currenttime)
					totaltime	= fmod(totaltime,currenttime);
				currenttime		=	0;
			}
			while(indice < sizeVpos)
			{
				currenttime	+=	m_time[indice];
				if (totaltime <= currenttime)
					break;
				++indice;
				lasttime	=	currenttime;
			}
			if (1 == sizeVpos)
				return m_vposition3D.front();
			if (0	==	sizeVpos)
				throw std::string ("error no pos light defined");
			if (0 == currenttime)
				throw std::string("error currenttime equal 0 , can't divide");
			t	=	totaltime - lasttime;
			if ((indice + 1 < sizeVpos) && (indice < sizeVpos))
			{ 
				t /= m_time[indice];
				T x,y,z;
				x	=	t * m_vposition3D[indice+1].x + (1 - t)* m_vposition3D[indice].x;
				y	=	t * m_vposition3D[indice+1].y + (1 - t)* m_vposition3D[indice].y;
				z	=	t * m_vposition3D[indice+1].z + (1 - t)* m_vposition3D[indice].z;
				Position3D<T>	result(x,y,z);
				return result;
			}
			else
				return m_vposition3D.back();		
		}
	protected:
		
		std::vector <Position3D<T>>	m_vposition3D;
		std::vector <T>				m_time;
		bool						m_looped;
};
#endif
