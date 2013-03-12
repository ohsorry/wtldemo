#ifndef _GDI_PLUS_UTIL_H_
#define _GDI_PLUS_UTIL_H_

#include <GdiPlus.h>
#include <map>
#include <string>
//using namespace std;

namespace GdiPlusHelper
{
	class GdiPlusInit
	{
	public:
		void Init()
		{
			Gdiplus::GdiplusStartup(&m_gdiplusToken,&StartupInput,NULL);
		}
		void DeInit()
		{
			Gdiplus::GdiplusShutdown(m_gdiplusToken); 
		}
	private:
		Gdiplus::GdiplusStartupInput StartupInput;
		ULONG_PTR m_gdiplusToken;  
	};

	class GdiPlusAutoInit
	{
	public:
		GdiPlusAutoInit()
		{
			mIns.Init();
		}

		~GdiPlusAutoInit()
		{
			mIns.DeInit();
		}

		GdiPlusInit mIns;
	};



	class ImageManager
	{
	public:
		typedef std::map<std::string,Gdiplus::Image*> ImageMapType;
	public:
		bool IsExist(std::string);
		bool Load(std::string name);
		void Release(std::string name);
		Gdiplus::Image*GetImage(std::string filename);		
	private:
		std::map<std::string,Gdiplus::Image*> mImgMap;
	};



}




#endif