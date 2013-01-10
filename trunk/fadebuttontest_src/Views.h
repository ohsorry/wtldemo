#if !defined(AFX_VIEWS_H__20020629_8D64_963C_A351_0080AD509054__INCLUDED_)
#define AFX_VIEWS_H__20020629_8D64_963C_A351_0080AD509054__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CView1 : public CDialogImpl<CView1>
{
public:
   enum { IDD = IDD_DIALOG1 };
   DECLARE_EMPTY_MSG_MAP()
};

class CView2 : public CDialogImpl<CView2>
{
public:
   enum { IDD = IDD_DIALOG2 };
   DECLARE_EMPTY_MSG_MAP()
};

class CView3 : public CDialogImpl<CView3>
{
public:
   enum { IDD = IDD_DIALOG3 };
   DECLARE_EMPTY_MSG_MAP()
};


#endif // !defined(AFX_VIEWS_H__20020629_8D64_963C_A351_0080AD509054__INCLUDED_)

