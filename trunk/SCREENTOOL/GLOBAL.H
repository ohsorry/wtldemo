// ==========================================================================
// GLOBAL.h : 
// ==========================================================================
#ifndef ___GLOBAL_H___
#define ___GLOBAL_H___

// Constants
const int HIMETRIC_PER_INCH = 2540;


// ==========================================================================
// File, Directory Functions
// ==========================================================================
static BOOL FileExists(CString strFile)
{
	HANDLE hFile = ::CreateFile(strFile, GENERIC_READ, 0, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	::CloseHandle(hFile);
	return TRUE;
}

static void GetCurrentDir(LPTSTR strDir)
{
	TCHAR strCurrentDir[MAX_PATH] = {0};
	::GetModuleFileName(NULL, strCurrentDir, sizeof(strCurrentDir));
	if(_tcslen(strCurrentDir) > 0)
	{
		for(int x = (int)_tcslen(strCurrentDir) - 1; x > 0; x --)
		{
			if(strCurrentDir[x] == _T('\\'))
				break;
			strCurrentDir[x] = _T('\0');
		}
	}
	_tcscpy(strDir, strCurrentDir);
}

static void VerifyDirectory(CString strDir)
{
	// Remove trailing backslash
	if(strDir.Right(1) == _T('\\'))
		strDir = strDir.Left(strDir.GetLength()-1); 
	// See if its there
	if(::GetFileAttributes(strDir)!=-1) 
		return;
	// Recursive call
	int i = strDir.ReverseFind(_T('\\'));
	VerifyDirectory(strDir.Left(i));
	// Create it
	::CreateDirectory(strDir, NULL); 
}

static void DelTree(CString strDir)
{  
	// Make sure theyres a trailing backslash
	if(strDir.Right(1) != _T('\\'))
		strDir += _T('\\');
	// Format search string
	TCHAR strFind[MAX_PATH] = {NULL};
	_stprintf(strFind, _T("%s*.*"), strDir);
	// Begin search operation
	WIN32_FIND_DATA fd; 
	HANDLE hFind = ::FindFirstFile(strFind, &fd);
	if(hFind == INVALID_HANDLE_VALUE)
		return;
	do
	{
		CString strFound = (LPCTSTR)&fd.cFileName;
		// See if its a directory
		if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if((strFound != _T(".")) && (strFound != _T("..")))
			{
				CString strFullPath = strDir;
				strFullPath += strFound;
				// Recursive call
				DelTree(strFullPath);
				// Kill the directory
				::RemoveDirectory(strFullPath);
			}
		}
		else
		{
			// Kill the file
			CString strFile = strDir;
			strFile += strFound;
			::DeleteFile(strFile);
		}
	}
	while(::FindNextFile(hFind, &fd));
	::FindClose(hFind);
	// Remove the baskshlash we ensured was there
	strDir = strDir.Left(strDir.GetLength()-1);
	// Kill the directory
	::RemoveDirectory(strDir);
}


// ==========================================================================
// Image Loading Functions
// ==========================================================================
static IPicture* LoadImageFromFile(LPTSTR strFile)
{
	// See if the file exists
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(strFile, &fd);
	if(hFind == INVALID_HANDLE_VALUE) 
		return NULL;
	::FindClose(hFind);
	
	IPicture* pPicture = NULL;
	HRESULT hr = ::OleLoadPicturePath(CComBSTR(strFile), 
		NULL, 0, 0, IID_IPicture, (LPVOID*)&pPicture);

	/*
	// Open file
	HANDLE hFile = ::CreateFile(strFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	ASSERT(INVALID_HANDLE_VALUE != hFile);

	// Get file size
	DWORD dwFileSize = ::GetFileSize(hFile, NULL);
	ASSERT(dwFileSize != -1);

	// Alloc memory based on file size
	void* pData = NULL;
	HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, dwFileSize+1);
	ASSERT(hGlobal);
	pData = ::GlobalLock(hGlobal);
	ASSERT(pData);

	// Read the file 
	DWORD dwBytesRead = 0;
	if(!::ReadFile(hFile, pData, dwFileSize, &dwBytesRead, NULL))
		return NULL;
	::GlobalUnlock(hGlobal);
	::CloseHandle(hFile);

	IStream* pStream = NULL;
	IPicture* pPicture = NULL;
	if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
	{
		HRESULT hr = ::OleLoadPicture(pStream, 0, 
			FALSE, IID_IPicture, (void**)&pPicture);
		ASSERT(SUCCEEDED(hr) && pPicture);
		pStream->Release();
	}

	::GlobalFree(hGlobal);
	*/
	return pPicture; 
}

static IPicture* LoadImageFromBuffer(BYTE* pBuff, int iSize)
{
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, iSize);
	void* pData = GlobalLock(hGlobal);
	memcpy(pData, pBuff, iSize);
	::GlobalUnlock(hGlobal);

	IStream* pStream = NULL;
	IPicture* pPicture = NULL;

	if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
	{
		HRESULT hr = ::OleLoadPicture(pStream, 0, 
			FALSE, IID_IPicture, (void**)&pPicture);
		pStream->Release();
	}

	::GlobalFree(hGlobal);

	return pPicture; 
}
  	
static IPicture* LoadImageFromResource(UINT nID, LPTSTR strType, HINSTANCE hResInst = NULL)
{
	if(hResInst == NULL)
		hResInst = AfxGetResourceHandle();

	// Find resource in resource dll
	HRSRC hRes = ::FindResource(hResInst, MAKEINTRESOURCE(nID), strType);
	if(!hRes)
		return NULL;//::GetLastError();

	// Load resource into memory
	DWORD dwSize = ::SizeofResource(hResInst, hRes);
	BYTE* pRsc = (BYTE*)::LoadResource(hResInst, hRes);
	if(!pRsc)
		return NULL;

	// Load the image
	return LoadImageFromBuffer(pRsc, dwSize);
}

// ==========================================================================
// Aspect Ratio 
// ==========================================================================
static CRect GetTargetRect(CRect rTarget, CSize szSource)
{
	// Keep the aspect ratio
	int iLeft = rTarget.left;
	int iTop = rTarget.top;
	float fWidth = (float)rTarget.Width();
	float fHeight = (float)rTarget.Height();

	// See if were stretching or shrinking
	if((szSource.cx >= fWidth) || (szSource.cy >= fHeight))
	{
		// Shrinking
		float fPercentCy = ((fHeight / szSource.cy) * 100);
		float fPercentCx = ((fWidth / szSource.cx) * 100);
		if(fPercentCx > fPercentCy)
		{
			// Must shrink more horizontally
			int iNewWidth = (int)((szSource.cx * fPercentCy) / 100);
			iLeft += (int)((fWidth - iNewWidth) / 2);
			fWidth = (float)iNewWidth;
		}
		else
		{
			// Must shrink more vertically
			int iNewHeight = (int)((szSource.cy * fPercentCx) / 100);
			iTop += (int)((fHeight - iNewHeight) / 2);
			fHeight = (float)iNewHeight;
		}
	}
	else
	{
		// Stretching
		float fPercentCy = ((fHeight / szSource.cy) * 100);
		float fPercentCx = ((fWidth / szSource.cx) * 100);
		if(fPercentCx > fPercentCy)
		{
			// Must stretch more horizontally
			int iNewWidth = (int)((szSource.cx * fPercentCy) / 100);
			iLeft -= (int)((iNewWidth - fWidth) / 2);
			fWidth = (float)iNewWidth;
		}
		else
		{
			// Must stretch more vertically
			int iNewHeight = (int)((szSource.cy * fPercentCx) / 100);
			iTop -= (int)((iNewHeight - fHeight) / 2);
			fHeight = (float)iNewHeight;
		}
	}

	return CRect(iLeft, iTop, (int)(iLeft + fWidth), (int)(iTop + fHeight));
}


// ==========================================================================
// Image Conversion Functions
// ==========================================================================
static HBITMAP CreateBmpFromImg(IPicture* pPicture)
{
	// Get the picture size
	CWindowDC dcScreen(CWnd::GetDesktopWindow());
	CSize szInHiMetric, szInPix;
	pPicture->get_Width(&szInHiMetric.cx);
	pPicture->get_Height(&szInHiMetric.cy);
	// Pixels per logical inch along width
	const int nPixelsPerInchX = dcScreen.GetDeviceCaps(LOGPIXELSX);
	// Pixels per logical inch along height
	const int nPixelsPerInchY = dcScreen.GetDeviceCaps(LOGPIXELSY);
	// Convert himetric to pixels
	szInPix.cx = ((nPixelsPerInchX * szInHiMetric.cx + HIMETRIC_PER_INCH / 2) / HIMETRIC_PER_INCH);
	szInPix.cy = ((nPixelsPerInchY * szInHiMetric.cy + HIMETRIC_PER_INCH / 2) / HIMETRIC_PER_INCH);

	// Create a dc and bitmap the size of the destination image
	CDC dcBmp;
	dcBmp.CreateCompatibleDC(&dcScreen);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dcScreen, szInPix.cx, szInPix.cy);
	CBitmap* pOldBmp = (CBitmap*)dcBmp.SelectObject(&bmp);

	// Try to get the palette
	HPALETTE hPal = NULL;
	pPicture->get_hPal(reinterpret_cast<OLE_HANDLE *>(&hPal));
	CPalette* pOldPal = NULL;
	if(hPal)
	{
		pOldPal = dcBmp.SelectPalette(CPalette::FromHandle(hPal), FALSE);
		dcBmp.RealizePalette();
	}

	// Render the image
	pPicture->Render(dcBmp.GetSafeHdc(), 0, 0, szInPix.cx, szInPix.cy, 
		0, szInHiMetric.cy, szInHiMetric.cx, -szInHiMetric.cy, NULL);

	// Clean up
	if(pOldPal)
		dcBmp.SelectPalette(pOldPal, FALSE);
	dcBmp.SelectObject(pOldBmp);

	return (HBITMAP)bmp.Detach();
}


#endif // ___GLOBAL_H___


