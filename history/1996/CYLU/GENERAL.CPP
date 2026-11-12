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

double CStrToFloat( CString cstrValue )
{   double dRet;
    
    dRet = atof( cstrValue );
    return dRet;						//  Cast to a float.
}

CString FloatToCStr( double fValue, int nFigures, int nPrecision )
{
    char szFmt[20];
    int nCopied;
    sprintf( szFmt, "%%%u.%uf",nFigures,nPrecision );
    CString szTemp;
    char *pBuffer = szTemp.GetBufferSetLength( 20 );
    nCopied = sprintf( pBuffer, szFmt, fValue );
    szTemp.ReleaseBuffer(-1);
    TrimString( &szTemp );
    return szTemp;
}

int CStrToInt( CString cstrValue )
{
    int nRet;
    nRet = atoi( cstrValue );
    return nRet;
}

CString IntToCStr( int nValue )
{
    CString cstrTemp;
    char *pBuffer = cstrTemp.GetBufferSetLength( 10 );
    wsprintf( pBuffer, "%d", nValue );
    cstrTemp.ReleaseBuffer(-1);
    TrimString( &cstrTemp );
    return cstrTemp;
}

void TrimString( CString *IpString )
{
    char *p;
    char szWork[256];
    int i=0;

    p = IpString->GetBuffer( IpString->GetLength() );

	while( *(p+i)==' ' )
        ++i;

    strcpy( szWork, p+i );
    i = strlen( szWork )-1;

    while( szWork[i]==' ' )
    {
        szWork[i]=NULL;
        i--;
    }

    strcpy( p, szWork );
    IpString->ReleaseBuffer(-1);
}
