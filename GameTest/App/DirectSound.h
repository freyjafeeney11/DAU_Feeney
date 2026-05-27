

























#if !defined(AFX_DIRECTSOUND_H__A20FE86F_118F_11D2_9AB3_0060B0CDC13E__INCLUDED_)
#define AFX_DIRECTSOUND_H__A20FE86F_118F_11D2_9AB3_0060B0CDC13E__INCLUDED_

#include <dsound.h>
#include <Windows.h>
#pragma message("linking with Microsoft's DirectSound library ...")
#pragma comment(lib, "dsound.lib")

class CDirectSound  
{
public:		
	CDirectSound();
	virtual ~CDirectSound();
	
	
	BOOL	Create(LPCTSTR pszResource, HWND * pWnd = 0);
	BOOL	Create(UINT uResourceID, HWND * pWnd = 0) {
				return Create(MAKEINTRESOURCE(uResourceID), pWnd);
			}
	
	
	
	
	
	BOOL	Create(LPVOID pSoundData, HWND * pWnd = 0);

public:		
	BOOL			IsValid() const;
	void			Play(DWORD dwStartPosition = 0, BOOL bLoop = FALSE);
	void			Stop();
	void			Pause();
	void			Continue();
	CDirectSound &	EnableSound(BOOL bEnable = TRUE) {
						m_bEnabled = bEnable;
						if( ! bEnable )
							Stop();
						return * this;
					}
	BOOL			IsEnabled() const { return m_bEnabled; }

protected:	
	BOOL SetSoundData(LPVOID pSoundData, DWORD dwSoundSize);
	BOOL CreateSoundBuffer(WAVEFORMATEX * pcmwf);
	BOOL GetWaveData(void * pRes, WAVEFORMATEX * & pWaveHeader, void * & pbWaveData, DWORD & cbWaveSize);

private:	
	LPVOID m_pTheSound;
	DWORD m_dwTheSound;
	LPDIRECTSOUNDBUFFER m_pDsb;
	BOOL m_bEnabled;
	static LPDIRECTSOUND m_lpDirectSound;
	static DWORD m_dwInstances;
};

#endif 
