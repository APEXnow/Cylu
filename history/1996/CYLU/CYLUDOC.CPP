// Cyludoc.cpp : implementation of the CCyluDoc class
//

#include "stdafx.h"
#include "Cylu.h"

#include "Cyludoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCyluDoc

IMPLEMENT_DYNCREATE(CCyluDoc, CDocument)

BEGIN_MESSAGE_MAP(CCyluDoc, CDocument)
	//{{AFX_MSG_MAP(CCyluDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCyluDoc construction/destruction

CCyluDoc::CCyluDoc()
{
	// TODO: add one-time construction code here

}

CCyluDoc::~CCyluDoc()
{
}

BOOL CCyluDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCyluDoc serialization

void CCyluDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCyluDoc diagnostics

#ifdef _DEBUG
void CCyluDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCyluDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCyluDoc commands
