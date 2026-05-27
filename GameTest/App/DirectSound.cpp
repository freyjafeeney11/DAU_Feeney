

























#include "stdafx.h"
#define ASSERT(_f_)	printf("assert\n");
#include "DirectSound.h"



#ifndef DSBLOCK_ENTIREBUFFER
	#define DSBLOCK_ENTIREBUFFER        0x00000002
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static void DSError( HRESULT hRes ) {
	switch(hRes) {
		case DS_OK: printf("NO ERROR\n"); break;
		case DSERR_ALLOCATED: printf("ALLOCATED\n"); break;
		case DSERR_INVALIDPARAM: printf("INVALIDPARAM\n"); break;
		case DSERR_OUTOFMEMORY: printf("OUTOFMEMORY\n"); break;
		case DSERR_UNSUPPORTED: printf("UNSUPPORTED\n"); break;
		case DSERR_NOAGGREGATION: printf("NOAGGREGATION\n"); break;
		case DSERR_UNINITIALIZED: printf("UNINITIALIZED\n"); break;
		case DSERR_BADFORMAT: printf("BADFORMAT\n"); break;
		case DSERR_ALREADYINITIALIZED: printf("ALREADYINITIALIZED\n"); break;
		case DSERR_BUFFERLOST: printf("BUFFERLOST\n"); break;
		case DSERR_CONTROLUNAVAIL: printf("CONTROLUNAVAIL\n"); break;
		case DSERR_GENERIC: printf("GENERIC\n"); break;
		case DSERR_INVALIDCALL: printf("INVALIDCALL\n"); break;
		case DSERR_OTHERAPPHASPRIO: printf("OTHERAPPHASPRIO\n"); break;
		case DSERR_PRIOLEVELNEEDED: printf("PRIOLEVELNEEDED\n"); break;
		default: printf("%lu\n",hRes);break;
	}
}





LPDIRECTSOUND CDirectSound::m_lpDirectSound;
DWORD CDirectSound::m_dwInstances;


CDirectSound::CDirectSound()
{
	m_lpDirectSound = 0;
	m_pDsb = 0;
	m_pTheSound = 0;
	m_dwTheSound = 0;
	m_bEnabled = TRUE;

	++m_dwInstances;
}

CDirectSound::~CDirectSound()
{
	if( m_pDsb )
		m_pDsb->Release();

	if( !--m_dwInstances && m_lpDirectSound ) {
		m_lpDirectSound->Release();
		m_lpDirectSound = 0;
	}
}

BOOL CDirectSound::Create(LPCTSTR pszResource, HWND * pWnd)
{
	
	
	HINSTANCE hApp = ::GetModuleHandle(0);
	ASSERT(hApp);

	HRSRC hResInfo = ::FindResource(hApp, pszResource, TEXT("WAVE"));
	if(hResInfo == 0)
		return FALSE;

	HGLOBAL hRes = ::LoadResource(hApp, hResInfo);
	if(hRes == 0)
		return FALSE;

	LPVOID pTheSound = ::LockResource(hRes);
	if(pTheSound == 0)
		return FALSE;

	return Create(pTheSound, pWnd);
}


BOOL CDirectSound :: Create(LPVOID pSoundData, HWND * pWnd) {



	ASSERT(pWnd != 0);


	ASSERT(pSoundData != 0);

	
	
	
	if( m_lpDirectSound == 0 ) {
		
		
		
		HRESULT hRes = DS_OK;
		short nRes = 0;

		do {
			if( nRes )
				::Sleep(500);
			hRes = ::DirectSoundCreate(0, &m_lpDirectSound, 0);
			++nRes;
		} while( nRes < 10 && (hRes == DSERR_ALLOCATED || hRes == DSERR_NODRIVER) );

		if( hRes != DS_OK )
			return FALSE;
	
		m_lpDirectSound->SetCooperativeLevel(*pWnd, DSSCL_NORMAL);
	}

	ASSERT(m_lpDirectSound != 0);

	WAVEFORMATEX * pcmwf;
	if( ! GetWaveData(pSoundData, pcmwf, m_pTheSound, m_dwTheSound) ||
		! CreateSoundBuffer(pcmwf) ||
		! SetSoundData(m_pTheSound, m_dwTheSound) )
		return FALSE;

	return TRUE;
}


BOOL CDirectSound :: GetWaveData(void * pRes, WAVEFORMATEX * & pWaveHeader, void * & pbWaveData, DWORD & cbWaveSize) {
	pWaveHeader = 0;
	pbWaveData = 0;
	cbWaveSize = 0;

	DWORD * pdw = (DWORD *)pRes;
	DWORD dwRiff = *pdw++;
	DWORD dwLength = *pdw++;
	DWORD dwType = *pdw++;

	if( dwRiff != mmioFOURCC('R', 'I', 'F', 'F') )
		return FALSE;      

	if( dwType != mmioFOURCC('W', 'A', 'V', 'E') )
		return FALSE;      

	DWORD * pdwEnd = (DWORD *)((BYTE *)pdw + dwLength-4);

	while( pdw < pdwEnd ) {
		dwType = *pdw++;
		dwLength = *pdw++;

		switch( dwType ) {
			case mmioFOURCC('f', 'm', 't', ' '):
				if( !pWaveHeader ) {
					if( dwLength < sizeof(WAVEFORMAT) )
						return FALSE;      

					pWaveHeader = (WAVEFORMATEX *)pdw;

					if( pbWaveData && cbWaveSize )
						return TRUE;
				}
				break;

			case mmioFOURCC('d', 'a', 't', 'a'):
				pbWaveData = LPVOID(pdw);
				cbWaveSize = dwLength;

				if( pWaveHeader )
					return TRUE;
				break;
		}
		pdw = (DWORD *)((BYTE *)pdw + ((dwLength+1)&~1));
	}

	return FALSE;
}


BOOL CDirectSound::CreateSoundBuffer(WAVEFORMATEX * pcmwf)
{
	DSBUFFERDESC dsbdesc;

	
	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); 
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);
	
	dsbdesc.dwFlags = DSBCAPS_STATIC;		
	dsbdesc.dwBufferBytes = m_dwTheSound;
	dsbdesc.lpwfxFormat = pcmwf;    
	HRESULT hRes;
	if( DS_OK != (hRes = m_lpDirectSound->CreateSoundBuffer(&dsbdesc, &m_pDsb, 0)) ) {
		
		DSError(hRes);
		m_pDsb = 0;
		return FALSE;
	}

	return TRUE;
}


BOOL CDirectSound::SetSoundData(void * pSoundData, DWORD dwSoundSize) {
	LPVOID lpvPtr1;
	DWORD dwBytes1;
	
	HRESULT hr = m_pDsb->Lock(0, 0, &lpvPtr1, &dwBytes1, 0, 0, DSBLOCK_ENTIREBUFFER);    
    
	if(DSERR_BUFFERLOST == hr) {
		m_pDsb->Restore();
		hr = m_pDsb->Lock(0, 0, &lpvPtr1, &dwBytes1, 0, 0, DSBLOCK_ENTIREBUFFER);
	}
	if(DS_OK == hr) {
		
		::CopyMemory(lpvPtr1, pSoundData, dwBytes1);
		
		hr = m_pDsb->Unlock(lpvPtr1, dwBytes1, 0, 0);
		if(DS_OK == hr)
            return TRUE;
	}
	
	return FALSE;
}

void CDirectSound::Play(DWORD dwStartPosition, BOOL bLoop)
{
	if( ! IsValid() || ! IsEnabled() )
		return;		

	if( dwStartPosition > m_dwTheSound )
		dwStartPosition = m_dwTheSound;
	m_pDsb->SetCurrentPosition(dwStartPosition);
	if( DSERR_BUFFERLOST == m_pDsb->Play(0, 0, bLoop ? DSBPLAY_LOOPING : 0) ) {
		
		
		
		SetSoundData(m_pTheSound, m_dwTheSound);

		
		m_pDsb->Play(0, 0, bLoop ? DSBPLAY_LOOPING : 0);
	}
}

void CDirectSound::Stop()
{
	if( IsValid() )
		m_pDsb->Stop();
}

void CDirectSound::Pause()
{
	Stop();
}

void CDirectSound::Continue()
{
	if( IsValid() ) {
		DWORD dwPlayCursor, dwWriteCursor;
		m_pDsb->GetCurrentPosition(&dwPlayCursor, &dwWriteCursor);
		Play(dwPlayCursor);
	}
}

BOOL CDirectSound::IsValid() const
{
	return (m_lpDirectSound && m_pDsb && m_pTheSound && m_dwTheSound) ? TRUE : FALSE;
}
