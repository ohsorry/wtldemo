// ==========================================================================
// Registry.cpp : 
// ==========================================================================
#include "stdafx.h"
#include "Registry.h"

#define MAX_REG_VAL		2048


CRegistry::CRegistry()
{
	m_hMainKey	= HKEY_CURRENT_USER;
	memset(m_strSubKey, 0, sizeof(m_strSubKey));
}

CRegistry::CRegistry(HKEY hMainKey, LPCTSTR strSubKey)
{
	m_hMainKey	= hMainKey;
	_stprintf(m_strSubKey, strSubKey);
}

CRegistry::~CRegistry()
{
}

int	CRegistry::GetRegInt(LPCTSTR strValName, int iDefault)
{
	HKEY  hOpenKey = 0;
	DWORD dwDisp   = 0;
	DWORD dwType   = 0;
	LONG  lResult  = 0;
	DWORD dwData   = 0;
	DWORD dwLen	   = sizeof(dwData);

	// Open the registry key
	lResult = ::RegCreateKeyEx(m_hMainKey, m_strSubKey, 0L, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &hOpenKey, &dwDisp);

	// Check for errors
	if(lResult != ERROR_SUCCESS)
	{
		return iDefault;
	}

	// Read the value
	lResult = ::RegQueryValueEx(hOpenKey, strValName, 0, &dwType, (LPBYTE)&dwData, &dwLen);
	
	// Check for errors
	if(lResult != ERROR_SUCCESS) 
	{
		::RegCloseKey(hOpenKey);
		return iDefault;
	}

	// Close the registry key
	::RegCloseKey(hOpenKey);

	return((int)dwData);
}

int	CRegistry::SetRegInt(LPCTSTR strValName, int iValue)
{
	HKEY  hOpenKey = 0;
	DWORD dwDisp   = 0;
	LONG  lResult  = 0;
	DWORD dwData   = (DWORD)iValue;
	DWORD dwLen	   = sizeof(dwData);

	// Open the registry key
	lResult = ::RegCreateKeyEx(m_hMainKey, m_strSubKey, 0L, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS|KEY_WRITE, NULL, &hOpenKey, &dwDisp);

	// Check for errors
	if(lResult != ERROR_SUCCESS)
		return lResult;
	
	// Write the value
	lResult = ::RegSetValueEx(hOpenKey, strValName, 0, REG_DWORD, (LPBYTE)&dwData, dwLen);

	// Close the registry key
	::RegCloseKey(hOpenKey);

	return lResult;
}

int	CRegistry::GetRegStr(LPCTSTR strValName, LPTSTR strValue, LPCTSTR strDefault)
{
	HKEY  hOpenKey = 0;
	DWORD dwDisp   = 0;
	DWORD dwType   = 0;
	LONG  lResult  = 0;
	TCHAR strData[MAX_PATH] = {0};
	DWORD dwLen	= sizeof(strData);

	// Open the registry key
	lResult = ::RegCreateKeyEx(m_hMainKey, m_strSubKey, 0L, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &hOpenKey, &dwDisp);

	// Check for errors
	if(lResult != ERROR_SUCCESS)
	{
		if(strDefault)
			_stprintf(strValue, strDefault);
		return lResult;
	}

	// Read the value
	lResult = ::RegQueryValueEx(hOpenKey, strValName, 0, &dwType, (LPBYTE)strData, &dwLen);
	
	// Check for errors
	if(lResult != ERROR_SUCCESS) 
	{
		if(strDefault)
			_stprintf(strValue, strDefault);
		::RegCloseKey(hOpenKey);
		return lResult;
	}

	_stprintf(strValue, strData);
	
	// Close the registry key
	::RegCloseKey(hOpenKey);
	
	return lResult;
}

int	CRegistry::SetRegStr(LPCTSTR strValName, LPCTSTR strValue)
{
	HKEY  hOpenKey = 0;
	DWORD dwDisp   = 0;
	LONG  lResult  = 0;
	DWORD dwLen	   = (DWORD)_tcslen(strValue);
	#ifdef _UNICODE
	dwLen  = dwLen * 2;
	#endif
	dwLen++;

	// Open the registry key
	lResult = ::RegCreateKeyEx(m_hMainKey, m_strSubKey, 0L, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS|KEY_WRITE, NULL, &hOpenKey, &dwDisp);

	// Check for errors
	if(lResult != ERROR_SUCCESS)
		return lResult;

	// Write the value
	lResult = ::RegSetValueEx(hOpenKey, strValName, 0, REG_SZ, (LPBYTE)strValue, dwLen);

	// Close the registry key
	::RegCloseKey(hOpenKey);

	return lResult;
}

int	CRegistry::GetRegBin(LPCTSTR strValName, void* pData, int iSize)
{
	HKEY  hOpenKey = 0;
	DWORD dwDisp   = 0;
	DWORD dwType   = 0;
	LONG  lResult  = 0;
	DWORD dwData   = 0;
	DWORD dwLen	   = (DWORD)iSize;

	// Open the registry key
	lResult = ::RegCreateKeyEx(m_hMainKey, m_strSubKey, 0L, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &hOpenKey, &dwDisp);

	// Check for errors
	if(lResult != ERROR_SUCCESS) 
		return lResult;

	// Read the value
	lResult = ::RegQueryValueEx(hOpenKey, strValName, 0, &dwType, (LPBYTE)pData, &dwLen);

	// Close the registry key
	::RegCloseKey(hOpenKey);

	return lResult;
}

int	CRegistry::SetRegBin(LPCTSTR strValName, void* pData, int iSize)
{
	HKEY  hOpenKey = 0;
	DWORD dwDisp   = 0;
	LONG  lResult  = 0;
	DWORD dwLen	   = (DWORD)iSize;

	// Open the registry key
	lResult = ::RegCreateKeyEx(m_hMainKey, m_strSubKey, 0L, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS|KEY_WRITE, NULL, &hOpenKey, &dwDisp);

	// Write the value
	lResult = ::RegSetValueEx(hOpenKey, strValName, 0, REG_BINARY, (LPBYTE)pData, dwLen);

	// Close the registry key
	::RegCloseKey(hOpenKey);

	return lResult;
}

int	CRegistry::GetRegStrEnc(int iSwap, LPCTSTR strValName, LPTSTR strValue, LPCTSTR strDefault)
{
	// Try and retrieve the value
	TCHAR strRegVal[MAX_REG_VAL] = {0};
	long lResult = GetRegBin(strValName, strRegVal, MAX_REG_VAL);
	
	// Check for errors
	if(lResult != ERROR_SUCCESS) 
	{
		if(strDefault)
			_stprintf(strValue, strDefault);
		return lResult;
	}

	// Decrypt the value
	SwapBits(strRegVal, (int)_tcslen(strRegVal) + 1, iSwap);
	_stprintf(strValue, strRegVal);

	return lResult;
}

int	CRegistry::SetRegStrEnc(int iSwap, LPCTSTR strValName, LPCTSTR strValue)
{
	int iSize = (int)_tcslen(strValue) + 1;
	TCHAR strRegVal[MAX_REG_VAL] = {0};
	_tcscpy(strRegVal, strValue);
	SwapBits(strRegVal, iSize, iSwap);

	return SetRegBin(strValName, strRegVal, iSize);
}

//=================================================
// Helper Functions
//=================================================
void CRegistry::SwapBits(LPTSTR strSwap, int iLength, UINT nSwap)
{
	for(int x = 0; x < iLength; x ++)
	{
		UINT n = strSwap[x]; 
		strSwap[x] = n ^ nSwap; 
	}
}

