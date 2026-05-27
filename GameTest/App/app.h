


#ifndef _APP_H
#define _APP_H

#include <windows.h> 

#include "../glut/include/GL/freeglut.h"
#include "AppSettings.h"
#include "SimpleController.h"
#include "SimpleSprite.h"

#define APP_VIRTUAL_TO_NATIVE_COORDS(_x_,_y_)			_x_ = ((_x_ / APP_VIRTUAL_WIDTH )*2.0f) - 1.0f; _y_ = (((_y_ - 40.0f) / 720.0f)*2.0f) - 1.0f;
#define APP_NATIVE_TO_VIRTUAL_COORDS(_x_,_y_)			_x_ = ((_x_ + 1.0f) * APP_VIRTUAL_WIDTH) / 2.0f; _y_ = (((_y_ + 1.0f) * 720.0f) / 2.0f) + 40.0f;




namespace App
{
	
	
	
	
	
	
	
	
	void DrawLine( float sx, float sy, float ex, float ey, float r = 1.0f, float g = 1.0f, float b = 1.0f );

	
	
	
	
	
	
	
	
	
	void Print(float x, float y, const char *text, float r = 1.0f, float g = 1.0f, float b = 1.0f, void *font = GLUT_BITMAP_HELVETICA_18);

	
	
	
	
	
	
	
	
	CSimpleSprite *CreateSprite(const char *fileName, int columns, int rows);
		
	
	
	
	
	
	
	
	
	
	void PlaySound(const char *fileName, bool looping = false);

	
	
	
	
	
	void StopSound(const char *fileName);

	
	
	
	
	
	bool IsSoundPlaying(const char *filename);
	
	
	
	
	
	
	
	
	
	
	
	
	bool IsKeyPressed(int key);

	
	
	
	
	
	void GetMousePos(float &x, float &y);

	
	
	
	
	
	
	
	
	
	
	
	const CController &GetController( int pad = 0 );
	void SetSoundVolume(const char* fileName, float volume);

	void InitFont(const char* fontPath, float pixelHeight, int slot);
	void PrintTTF(float x, float y, const char* text, float r, float g, float b, int slot = 0, float a = 1.0f);
};
#endif 