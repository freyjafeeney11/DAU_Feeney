#pragma once

typedef	HRESULT	(WINAPI *LPGETAUDIOSAMPLES_PROGRESS)(LPBYTE lpDesBuf, const DWORD dwRequiredSamples, DWORD &dwRetSamples, LPVOID lpData);

class CMyDirectSound  
{
public:
		
	CMyDirectSound();
	virtual ~CMyDirectSound();

	void SetFormat(WAVEFORMATEX WFE);
	void SetCallback(LPGETAUDIOSAMPLES_PROGRESS Function_Callback, LPVOID lpData);
	void Play();
	void Pause();
	void Stop();
	DWORD GetSamplesPlayed();	
	void TimerCallback();
	
private:
	
	
	WAVEFORMATEX m_WFE;
	LPDIRECTSOUND m_lpDS;
	LPDIRECTSOUNDBUFFER m_lpDSB;
	HANDLE m_pHEvent[2];
	

	
	LPBYTE m_lpAudioBuf;
	LPGETAUDIOSAMPLES_PROGRESS m_lpGETAUDIOSAMPLES;
	LPVOID m_lpData;
	

	
	MMRESULT m_timerID;
	DWORD m_dwCircles1;
	DWORD m_dwCircles2;
	int m_iDB;	
	

	
	CString m_strLastError;
	
};
