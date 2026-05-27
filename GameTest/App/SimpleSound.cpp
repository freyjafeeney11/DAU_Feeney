




#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <assert.h>

#include "SimpleSound.h"





CSimpleSound &CSimpleSound::GetInstance()
{
	static CSimpleSound theSoundClass;
	return theSoundClass;
}

CSimpleSound::CSimpleSound() : m_directSound(nullptr), m_primaryBuffer(nullptr)
{
}

CSimpleSound::~CSimpleSound()
{
	Shutdown();
}

bool CSimpleSound::Initialize(HWND hwnd)
{
	
	return InitializeDirectSound(hwnd);
}

void CSimpleSound::Shutdown()
{
	
	for (auto sound : m_sounds) 
	{
		if( sound.second )
		{
			sound.second->Release();
		}		
	}
	
	ShutdownDirectSound();
	m_sounds.clear();
	return;
}

bool CSimpleSound::SetVolume(const char* filename, float volume) {
	if (m_sounds[filename] == nullptr) return false;
	LONG db = (volume <= 0.0f) ? DSBVOLUME_MIN : (LONG)(2000.0f * log10f(volume));
	if (db < DSBVOLUME_MIN) db = DSBVOLUME_MIN;
	m_sounds[filename]->SetVolume(db);
	return true;
}

bool CSimpleSound::InitializeDirectSound(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	
	result = DirectSoundCreate8(NULL, &m_directSound, NULL);
	if(FAILED(result))
	{
		return false;
	}

	
	result = m_directSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if(FAILED(result))
	{
		return false;
	}

	
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	
	result = m_directSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	if(FAILED(result))
	{
		return false;
	}

	
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	
	result = m_primaryBuffer->SetFormat(&waveFormat);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void CSimpleSound::ShutdownDirectSound()
{
	
	if(m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	
	if(m_directSound)
	{
		m_directSound->Release();
		m_directSound = 0;
	}

	return;
}

bool CSimpleSound::PlaySound(const char *filename, DWORD flags)
{
	HRESULT result;

	
	
	if (m_sounds[filename] == nullptr)
	{
		result = LoadWaveFile(filename);
		if (FAILED(result))
		{
			return false;
		}
	}

	if (m_sounds[filename])
	{
		result = m_sounds[filename]->SetCurrentPosition(0);
		if (FAILED(result))
		{
			return false;
		}

		
		result = m_sounds[filename]->SetVolume(DSBVOLUME_MAX);
		if (FAILED(result))
		{
			return false;
		}

		
		result = m_sounds[filename]->Play(0, 0, flags);
		if (FAILED(result))
		{
			return false;
		}
		return true;
	}
	return false;
}


bool CSimpleSound::IsPlaying(const char *filename)
{
	if (m_sounds[filename] != nullptr)
	{
		DWORD dwStatus;
		HRESULT result = m_sounds[filename]->GetStatus(&dwStatus);
		if (!FAILED(result))
		{
			return (dwStatus&DSBSTATUS_PLAYING) != 0;
		}			
	}
	return false;
}

bool CSimpleSound::StopSound(const char *filename)
{
	if (IsPlaying(filename) )
	{
		if (m_sounds[filename] != nullptr)
		{
			HRESULT result = m_sounds[filename]->Stop();
			return (FAILED(result));
		}
	}
	return false;
}

bool CSimpleSound::LoadWaveFile(const char* filename)
{	
	int error;
	FILE* filePtr = nullptr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;
	
	IDirectSoundBuffer8** secondaryBuffer = &m_sounds[filename];

	
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	
	count = (unsigned int)fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	
	if ((waveFileHeader.m_chunkId[0] != 'R') || (waveFileHeader.m_chunkId[1] != 'I') ||
		(waveFileHeader.m_chunkId[2] != 'F') || (waveFileHeader.m_chunkId[3] != 'F'))
	{
		return false;
	}

	
	if ((waveFileHeader.m_format[0] != 'W') || (waveFileHeader.m_format[1] != 'A') ||
		(waveFileHeader.m_format[2] != 'V') || (waveFileHeader.m_format[3] != 'E'))
	{
		return false;
	}

	
	if ((waveFileHeader.m_subChunkId[0] != 'f') || (waveFileHeader.m_subChunkId[1] != 'm') ||
		(waveFileHeader.m_subChunkId[2] != 't') || (waveFileHeader.m_subChunkId[3] != ' '))
	{
		return false;
	}

	
	if ((waveFileHeader.m_dataChunkId[0] != 'd') || (waveFileHeader.m_dataChunkId[1] != 'a') ||
		(waveFileHeader.m_dataChunkId[2] != 't') || (waveFileHeader.m_dataChunkId[3] != 'a'))
	{
		return false;
	}

	
	waveFormat.wFormatTag = waveFileHeader.m_audioFormat;
	waveFormat.nSamplesPerSec = waveFileHeader.m_sampleRate;
	waveFormat.wBitsPerSample = waveFileHeader.m_bitsPerSample;
	waveFormat.nChannels = waveFileHeader.m_numChannels;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.m_dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	
	result = m_directSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result))
	{
		return false;
	}

	
	tempBuffer->Release();
	tempBuffer = 0;

	
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	
	waveData = new unsigned char[waveFileHeader.m_dataSize];
	if (!waveData)
	{
		return false;
	}

	
	count = (unsigned int)fread(waveData, 1, waveFileHeader.m_dataSize, filePtr);
	if (count != waveFileHeader.m_dataSize)
	{
		return false;
	}

	
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	
	result = (*secondaryBuffer)->Lock(0, waveFileHeader.m_dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
	{
		return false;
	}

	
	memcpy(bufferPtr, waveData, waveFileHeader.m_dataSize);

	
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
	{
		return false;
	}

	
	delete[] waveData;
	waveData = 0;

	return true;
}