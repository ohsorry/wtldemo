// ==========================================================================
// Registry.h : 
// ==========================================================================
#ifndef ___REGISTRY_H___
#define ___REGISTRY_H___


class CRegistry
{
public:
	// Construction / Destruction
	CRegistry();
	CRegistry(HKEY hMainKey, LPCTSTR strSubKey);
	~CRegistry();

protected:
	// Properties
	HKEY	m_hMainKey;
	TCHAR	m_strSubKey[MAX_PATH];

public:
	// Implementation
	HKEY	GetMainKey(){return m_hMainKey;}
	void	SetMainKey(HKEY h){m_hMainKey = h;}
	LPTSTR  GetSubKey(){return (LPTSTR)m_strSubKey;}
	void	SetSubKey(LPCTSTR s){_stprintf(m_strSubKey, s);}

	int		GetRegInt(LPCTSTR strValName, int iDefault = 0);
	int		SetRegInt(LPCTSTR strValName, int iValue);
	int		GetRegStr(LPCTSTR strValName, LPTSTR strValue, LPCTSTR strDefault = NULL);
	int		SetRegStr(LPCTSTR strValName, LPCTSTR strValue);
	int		GetRegStrEnc(int iSwap, LPCTSTR strValName, LPTSTR strValue, LPCTSTR strDefault = NULL);
	int		SetRegStrEnc(int iSwap, LPCTSTR strValName, LPCTSTR strValue);
	int		GetRegBin(LPCTSTR strValName, void* pData, int iSize);
	int		SetRegBin(LPCTSTR strValName, void* pData, int iSize);

protected:
	// Helper Functions
	void SwapBits(LPTSTR strSwap, int iLength, UINT nSwap);
};


#endif // ___REGISTRY_H___