/******************************************************************

	Project:	Cylu
	File:		cpicbox.cpp
	Author:		Paul Millard
	Original:	Unknown
	Notes:		Functions that make up the CPictureBox class

******************************************************************/

#include "stdafx.h"
#include "Cylu.h"

#include "cpicbox.hpp"

CPictureBox::CPictureBox( void )
{
	m_hgblCAnimate = NULL;
	m_lppCAnimate = NULL;
	m_nSequences = NULL;
	SetScriptFile( (CFile*)0 );
	ClearFieldStrings();
}

CPictureBox::~CPictureBox()
{
	CloseScriptFile();
	if( m_hgblCAnimate )
	{
		DestroyFilms();
	}
}

CFile* CPictureBox::GetScriptFile()
{
    return m_lpScriptFile;
}

void CPictureBox::SetScriptFile( CFile* lpCFile )
{
    m_lpScriptFile = lpCFile;
}

void CPictureBox::ClearFieldStrings()
{
    int nLoop;                                  //  Loop variable
    
    for (nLoop = 0; nLoop < _NUM_FIELD_STRINGS; nLoop++)
    {
        ((CCyluApp*)AfxGetApp())->DoMessageLoop();
        m_cstrFields[ nLoop ] = "";
    }
}

BOOL CPictureBox::ReadRecord()
{
    BOOL bRet = FALSE;
    CString cstrBuffer;
    char chFileByte;
    long lBytesRead;
    BOOL bContinue = TRUE;
    
    if ( GetScriptFile() )
    {
        do
        {
            lBytesRead = GetScriptFile()->Read( &chFileByte, 1 );
            if ( lBytesRead == 1 )
            {
                if ( (chFileByte == '\n') || (chFileByte == '\r') )
                {
                    ClearFieldStrings();
                    FilterBuffer( cstrBuffer );
                    bContinue = FALSE;
                    bRet = TRUE;
                }
                else 
                {
                    cstrBuffer+=chFileByte;
                    bContinue = TRUE;
                }
            }
            else
            {
                bContinue = FALSE;
                bRet = FALSE;
            }
            
        } while (bContinue);
    }
    return bRet;
}

CString CPictureBox::GetFieldString( int nIndex )
{
    CString cstrRet;
    if (( nIndex >= 0) && ( nIndex <=(_NUM_FIELD_STRINGS ) ) )
        cstrRet = m_cstrFields[ nIndex ];
    else
        cstrRet = "Invalid parameter";
    return cstrRet;
}

BOOL CPictureBox::FieldStringUsed( int nIndex )
{
    BOOL bRet = FALSE;
    if (( nIndex >= 0) && ( nIndex <=(_NUM_FIELD_STRINGS ) ) )
    {
        bRet = m_cstrFields[ nIndex ].IsEmpty();
    }
    return !bRet;
}

BOOL CPictureBox::OpenScriptFile( CString cstrFileName, int *lpError )
{
    BOOL bRet;
        
    CloseScriptFile();
    int nFlags = CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary;
    CFile *lpScriptFile;
    CFileException cFileException_Script;
    
    lpScriptFile = new CFile;
    SetScriptFile( lpScriptFile );
    
    lpScriptFile->Open( cstrFileName, nFlags, &cFileException_Script );
    if (cFileException_Script.m_cause)
    {
        delete lpScriptFile;
        SetScriptFile( (CFile*)0 );
        *lpError = cFileException_Script.m_cause;
        bRet = FALSE;
    }
    else
    {
        bRet = TRUE;
        *lpError = 0;
    }

    return bRet;
}

void CPictureBox::CloseScriptFile()
{
    if (GetScriptFile())
    {
        GetScriptFile()->Close();
        delete GetScriptFile();
        SetScriptFile( (CFile*)0 );
    }
}

void CPictureBox::FilterBuffer( CString cstrBuffer )
{
    int nCurrentChar;
    int nFieldIndex;
    BYTE byFieldChar;
    BOOL bLineFinished;
    BOOL bDone;
    int nStringLength;
    
    if (!cstrBuffer.IsEmpty())
    {
        nStringLength = cstrBuffer.GetLength();
        
        bDone = FALSE;
        nCurrentChar = 0;
        for (nFieldIndex = 0; nFieldIndex < _NUM_FIELD_STRINGS && bDone == FALSE; nFieldIndex++ )
        {
            bLineFinished = FALSE;
            do
            {
                byFieldChar = (BYTE)cstrBuffer.GetAt( nCurrentChar );
                if (byFieldChar != _RECORD_DELIMITER)
                {
                    m_cstrFields[ nFieldIndex ]+=byFieldChar;
                    nCurrentChar++;
                }
                else
                {
                    nCurrentChar++;
                    bLineFinished = TRUE;
                }
                if ( nCurrentChar > nStringLength-1 )
                {
                    bLineFinished = TRUE;
                    bDone = TRUE;
                }
                ((CCyluApp*)AfxGetApp())->DoMessageLoop();
            } while (!bLineFinished);
        }
    }
}

void CPictureBox::SetScriptFileName( CString cstrName )
{
    m_cstrScriptFileName = cstrName;
}

CString CPictureBox::GetScriptFileName()
{
    return m_cstrScriptFileName;
}

int CPictureBox::ReadByte( BYTE *lpByte, CFile *lpCFile )
{
    int nRet;
    
    //  Catch any file exception if the write function fails.
    TRY
    {
        lpCFile->Read( lpByte, 1 );
    }
    CATCH( CFileException, cFileException )
    {
        nRet = cFileException->m_cause;
    }
    END_CATCH
    //  Return file exception value, if the function didn't fail, this
    //  would be FALSE.
    return nRet;
}

BOOL CPictureBox::ReadStringFromFile( CString cstrFileName, CString* lpcstrText )
{
    BOOL bRet = FALSE;
    int nFlags = CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary;
    BOOL bContinue = TRUE;
    CFile cFile;
    CFileException cFileException;
    DWORD dwFileLength;
    char chGetFromFile;
    LPSTR lpstrBuffer;
    LPSTR lpstrCurrentPosition;
    
    bRet = cFile.Open( cstrFileName, nFlags, &cFileException );
    if (cFileException.m_cause)
    {
        bRet = FALSE;
    }
    else
    {
        dwFileLength = cFile.GetLength();
        lpstrBuffer = lpcstrText->GetBuffer( (UINT)dwFileLength );
        if (lpstrBuffer)
        {
            lpstrCurrentPosition = lpstrBuffer;
            do
            {
                bContinue = TRUE;
                if (!ReadByte( (BYTE*)&chGetFromFile, &cFile ))
                    bContinue = FALSE;
                else
                {
                    *lpstrCurrentPosition = chGetFromFile;
                    lpstrCurrentPosition++;
                    if (chGetFromFile==0)
                    {
                        bContinue = FALSE;
                    }
                }
            } while (bContinue);
        }
        else
        {
            bRet = FALSE;
        }
        lpcstrText->ReleaseBuffer(-1);
    }
    return bRet;
}

BOOL CPictureBox::FindNextRecord()
{
    BOOL bRet;
    BOOL bContinue;
    BOOL bRepeatLoop = TRUE;
    BOOL bTextFound;
    int nLoop;
    
    do
    {
        bContinue = ReadRecord();
        if (bContinue)
        {
            bTextFound = FALSE;
            for (nLoop = 0; nLoop < _NUM_FIELD_STRINGS; nLoop++ )
            {
                bTextFound = FieldStringUsed( nLoop );
                if (bTextFound)
                {
                    nLoop = _NUM_FIELD_STRINGS;
                    bRepeatLoop = FALSE;
                }
            }
        }
        else
            bRepeatLoop = FALSE;

        ((CCyluApp*)AfxGetApp())->DoMessageLoop();

    } while (bRepeatLoop);
    
    if (bContinue)
        bRet = TRUE;
    else
        bRet = FALSE;
    
    return bRet;
}

BOOL CPictureBox::FindText( CString cstrText, int nIndex )
{
    BOOL bRet;
    BOOL bContinue;
    BOOL bRepeatLoop;
    
    do
    {
        bContinue = ReadRecord();
        if (bContinue)
        {
            if (cstrText == GetFieldString( nIndex ))
            {
                bRepeatLoop = FALSE;
            }
        }
        else
            bRepeatLoop = FALSE;
    } while (bRepeatLoop);
    
    if (bContinue)
        bRet = TRUE;
    else
        bRet = FALSE;
    
    return bRet;
}

BOOL CPictureBox::CreateFilms( CString cstrScriptName )
{
	BOOL bRet = TRUE;
	BOOL bScriptComplete = FALSE;
    int nOpenError;
    CString cstrCommand;

    if( DoesFileExist( cstrScriptName ) )
    {
        if( OpenScriptFile( cstrScriptName, &nOpenError ) )
        {
            do
            {
                if( !FindNextRecord() )
                {
                    bScriptComplete = TRUE;
					bRet = FALSE;
                    break;
                }
                if( GetFieldString( 0 )=="BEGIN_DOCUMENT" )
                {
                    bScriptComplete = FALSE;
                    break;
                }
            } while( TRUE );

            if( !bScriptComplete )
            {
                do
                {
                    if( !FindNextRecord() )
                    {
                        bScriptComplete = TRUE;
                        break;
                    }

                    cstrCommand = GetFieldString( 0 );
                    cstrCommand.MakeUpper();
    
                    //  Interpret script command.               
					if( cstrCommand=="END_DOCUMENT" )
						bScriptComplete = TRUE;

					if( cstrCommand=="ALLOCATE" )
					{
						if( !AllocateFilms() )
						{
							bScriptComplete = TRUE;
							bRet = FALSE;
						}
					}

					if( cstrCommand=="ANIMATION" )
					{
						if( !CreateSpecificFilm() )
						{
							bScriptComplete = TRUE;
							bRet = FALSE;
						}
					}

                    //  Let other apps do their thing.                              
                    ( (CCyluApp*)AfxGetApp() )->DoMessageLoop();

					if( bScriptComplete )
						break;

                } while( TRUE );
            }

			CloseScriptFile();
        }
        else
            bRet = FALSE;
    }
    else
        bRet = FALSE;

	return bRet;	
}

int CPictureBox::LockFilms( void )
{
	int nRet = LOCKSTATUS_FAILED;
	int nLoop;

	if( m_lppCAnimate )
		nRet = LOCKSTATUS_LOCKED;
	else
	{
		if( m_hgblCAnimate )
		{
			m_lppCAnimate = (CAnimate**)GlobalLock( m_hgblCAnimate );
			if( m_lppCAnimate )
			{
				for( nLoop=0; nLoop<m_nSequences; nLoop++ )
					m_lppCAnimate[ nLoop ]->LockAnimation();

				nRet = LOCKSTATUS_SUCCESS;
			}
		}
	}

	return nRet;
}

void CPictureBox::UnlockFilms( void )
{
	int nLoop;

	if( m_lppCAnimate )
	{
		if( m_hgblCAnimate )
		{
			for( nLoop=0; nLoop<m_nSequences; nLoop++ )
				m_lppCAnimate[ nLoop ]->UnlockAnimation();

			GlobalUnlock( m_hgblCAnimate );
			m_lppCAnimate = NULL;
		}
	}
}

BOOL CPictureBox::DestroyFilms( void )
{
	BOOL bRet = TRUE;
	int nLoop;

	if( m_hgblCAnimate )
	{
		LockFilms();
		for( nLoop=0; nLoop<m_nSequences; nLoop++ )
		{
			if( m_lppCAnimate[ nLoop ] )
			{
				delete m_lppCAnimate[ nLoop ];
				m_lppCAnimate[ nLoop ]=(CAnimate*)0;
			}
		}
		GlobalUnlock( m_hgblCAnimate );
		GlobalFree( m_hgblCAnimate );
		m_hgblCAnimate = NULL;
		m_lppCAnimate = NULL;
	}
	else
		bRet = FALSE;

	return bRet;
}

BOOL CPictureBox::RollFilms( void )
{
	BOOL bRet = TRUE;
	int nLoop;

	if( m_lppCAnimate )
	{
		for( nLoop=0; nLoop<m_nSequences; nLoop++ )
			m_lppCAnimate[ nLoop ]->AdvanceFrame();
	}
	else
		bRet = FALSE;

	return bRet;
}

CBitmap* CPictureBox::GetFrame( BOOL bImage, int nSequence )
{
	CBitmap* cBitmapRet = NULL;

	if( m_lppCAnimate )
	{
		if( (nSequence>=0) && (nSequence<=m_nSequences) )
			cBitmapRet = m_lppCAnimate[ nSequence ]->GetSprite( bImage );
	}

	return cBitmapRet;
}

BOOL CPictureBox::AllocateFilms( void )
{
	BOOL bRet = TRUE;
	long lMemoryRequired;
	int nLoop;

	int nSequences = CStrToInt( GetFieldString( 1 ) );

	lMemoryRequired = sizeof( CAnimate* ) * nSequences;

	m_hgblCAnimate = GlobalAlloc( GHND, lMemoryRequired );

	if( !m_hgblCAnimate )
	{
		bRet = FALSE;
		TRACE( "Status:   Insufficient memory to load sprites\n" );
	}
	else
	{
		m_lppCAnimate = (CAnimate**)GlobalLock( m_hgblCAnimate );
		if( m_lppCAnimate )
		{
			for( nLoop=0; nLoop<nSequences; nLoop++ )
			{
				m_lppCAnimate[ nLoop ] = new CAnimate;
				if( !m_lppCAnimate[ nLoop ] )
				{
					nLoop=nSequences;
					bRet = FALSE;
				}
			}

			if( !bRet )
			{
				for( nLoop=0; nLoop<nSequences; nLoop++ )
				{
					delete m_lppCAnimate[ nLoop ];
					m_lppCAnimate[ nLoop ]=(CAnimate*)0;
					GlobalUnlock( m_hgblCAnimate );
					GlobalFree( m_hgblCAnimate );
					m_hgblCAnimate = NULL;
				}
			}
			else
			{
				GlobalUnlock( m_hgblCAnimate );
				m_lppCAnimate = NULL;
				m_nSequences = nSequences;
			}

		}
		else
			bRet = FALSE;
	}

	return bRet;
}

BOOL CPictureBox::CreateSpecificFilm( void )
{
	BOOL bRet = TRUE;

	int nSequence = CStrToInt( GetFieldString( 1 ) );
	int nStartID = CStrToInt( GetFieldString( 2 ) );
	int nFrames = CStrToInt( GetFieldString( 3 ) );
	int nType = FieldAnimationType( 4 );

	if( (nSequence>=0) && (nSequence<GetAllocatedFilms()) )
	{
		if( LockFilms()==LOCKSTATUS_SUCCESS )
		{
			if( !m_lppCAnimate[ nSequence ]->CreateAnimation( nFrames, nType, nStartID ) )
				bRet = FALSE;

			UnlockFilms();
		}
		else
			bRet = FALSE;
	}
	else
		bRet = FALSE;

	return bRet;
}

int CPictureBox::GetAllocatedFilms( void )
{
	return m_nSequences;
}

int CPictureBox::FieldAnimationType( int nField )
{
    int nRet = 0;
    CString cstrField = GetFieldString( nField );
    cstrField.MakeUpper();
    
    if (cstrField == "STATIC")
        nRet = ANIMATE_STATIC;
    if (cstrField == "ROTATE")
        nRet = ANIMATE_ROTATE;
    if (cstrField == "BOUNCE")
        nRet = ANIMATE_BOUNCE;
    
    return nRet;
}
