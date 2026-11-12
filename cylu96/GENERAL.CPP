/****************************************************************************

File Name       :   general.cpp
Project Name    :   Cylu
Project Number  :   
Authors         :   Paul Millard
Systems         :   Microsoft Windows 95
Comments        :   General function definitions used throughout
					the program.
History         :

****************************************************************************/

#include "stdafx.h"
#include "general.h"

BOOL DoesFileExist( CString cstrFileName )
{
    BOOL bRet;
    int nFlags = CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary;
    CFile cFile;
    CFileException cFileException;
    
    bRet = cFile.Open( cstrFileName, nFlags, &cFileException );
    if (cFileException.m_cause)
    {
        bRet = FALSE;
    }
    else
        bRet = TRUE;

    return bRet;
}

//	2026: the functions below were rewritten for the Unicode build of
//	MFC; they do the same jobs as the 1996 char versions.

double CStrToFloat( CString cstrValue )
{   double dRet;
    
    dRet = _tstof( cstrValue );
    return dRet;						//  Cast to a float.
}

CString FloatToCStr( double fValue, int nFigures, int nPrecision )
{
    CString szTemp;

    szTemp.Format( _T("%*.*f"), nFigures, nPrecision, fValue );
    TrimString( &szTemp );
    return szTemp;
}

int CStrToInt( CString cstrValue )
{
    int nRet;
    nRet = _ttoi( cstrValue );
    return nRet;
}

CString IntToCStr( int nValue )
{
    CString cstrTemp;

    cstrTemp.Format( _T("%d"), nValue );
    TrimString( &cstrTemp );
    return cstrTemp;
}

void TrimString( CString *IpString )
{
    IpString->Trim( _T(' ') );
}
