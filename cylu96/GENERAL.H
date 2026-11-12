/****************************************************************************

File Name       :   general.h
Project Name    :   Cylu
Project Number  :   
Authors         :   Paul Millard
Systems         :   Microsoft Windows 95
Comments        :   General function declarations
History         :

****************************************************************************/

#ifndef _GENERAL_H_
#define _GENERAL_H_

BOOL DoesFileExist( CString cstrFileName );
double CStrToFloat( CString cstrValue );
CString FloatToCStr( double fValue, int nFigures, int nPrecision );
int CStrToInt( CString cstrValue );
CString IntToCStr( int nValue );
void TrimString( CString *IpString );

#endif
