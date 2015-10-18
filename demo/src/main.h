#ifndef MAIN_H_INCLUED
#define MAIN_H_INCLUED
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

//	FileManager
class FileManager
{
	public:
		FileManager();
		FileManager(std::string const & filename, std::string const & mode);
		void	LoadFile(std::string const & filename, std::string const & mode);
		FILE*	GetFilePtr();
		void	Release();
		~FileManager();
	protected:
		FILE*	file;
};
FileManager::FileManager()
{
	file	=	nullptr;
}
FileManager::FileManager(std::string const & filename, std::string const & mode)
{
	file	=	nullptr;
	file	=	fopen(filename.c_str(),mode.c_str());
}
FILE* FileManager::GetFilePtr()
{
	if (!file)
		throw std::string("Error I/O ");
	return file;
}
FileManager::~FileManager()
{
	this->Release();	
}
void	FileManager::Release()
{
	if(file)
		fclose(file);
	file	=	nullptr;
}
void 	FileManager::LoadFile(std::string const & filename, std::string const & mode)
{
	this->Release();
	file	=	fopen(filename.c_str(), mode.c_str());
}
// End FileManager
// BasicMemoryManager
template <class T>
class BasicVectorManager
{
	public:
		BasicVectorManager(){}
		BasicVectorManager(size_t nb)
		{
			m_pVect.resize(nb);
			for (size_t i=0; i < nb; ++i)
				m_pVect[i]	=	std::make_unique<T>();
		}
		virtual	void	Allocate(size_t nb)
		{
			this->Release();
			m_pVect.resize(nb);
			for (size_t i=0; i < nb;++i)
				m_pVect[i]	=	std::make_unique<T>();
		}
		virtual	void	Release()
		{
			m_pVect.clear();
		}
		virtual 		~BasicVectorManager()
		{
			this->Release();
		}
		virtual	T*	GetVectPtr(size_t i)
		{
			return m_pVect[i].get();
		}
	protected:
		std::vector<std::unique_ptr<T>>	m_pVect;
};
// End BasicMemoryManager
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
		virtual void			SetLooped(bool looped){}
		virtual Position3D<T>	GetInterpolated(T totaltime)
		{
			auto	sizeVpos	=	m_vposition3D.size();
			T	currenttime	=	0;
			auto	indice	=	0;
			while(indice < sizeVpos)
			{
				currenttime	+=	m_time[indice];
				if (totaltime <= currenttime)
					break;
				++indice;
			}
			if (1 == sizeVpos)
				return m_vposition3D.front();
			if (0	==	sizeVpos)
				throw std::string ("error no pos light defined");
			if (0 == currenttime)
				throw std::string("error currenttime equal 0 , can't divide");
			T t	=	totaltime / currenttime;
			if ((indice + 1 < sizeVpos) && (indice < sizeVpos))
			{ 
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
};
#endif
