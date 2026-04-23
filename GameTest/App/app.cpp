//---------------------------------------------------------------------------------
// App.cpp
// Implementation of the calls that are exposed via the App namespace.
//---------------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------------
#include <string>
#include "main.h"
#include "app.h"
#include "SimpleSound.h"
#include "SimpleController.h"
#include "SimpleSprite.h"

//---------------------------------------------------------------------------------
// Utils and externals for system info.

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

static GLuint          g_fontTexture[2] = { 0, 0 };
static stbtt_bakedchar g_charData[2][96];
static float        g_fontScale = 1.0f;


namespace App
{
	void InitFont(const char* fontPath, float pixelHeight, int slot) {
		FILE* f = fopen(fontPath, "rb");
		if (!f) return;
		fseek(f, 0, SEEK_END);
		long size = ftell(f);
		fseek(f, 0, SEEK_SET);
		unsigned char* buf = new unsigned char[size];
		fread(buf, 1, size, f);
		fclose(f);

		unsigned char bitmap[512 * 512];
		stbtt_BakeFontBitmap(buf, 0, pixelHeight, bitmap, 512, 512, 32, 96, g_charData[slot]);
		delete[] buf;

		glGenTextures(1, &g_fontTexture[slot]);
		glBindTexture(GL_TEXTURE_2D, g_fontTexture[slot]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void PrintTTF(float x, float y, const char* text, float r, float g, float b, int slot) {
		float nx = x, ny = y;
#if APP_USE_VIRTUAL_RES
		APP_VIRTUAL_TO_NATIVE_COORDS(nx, ny);
#endif
		// scale pixel offsets from stb down to native GL coord space
		const float scaleX = 2.0f / WINDOW_WIDTH;
		const float scaleY = 2.0f / WINDOW_HEIGHT;

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, g_fontTexture[slot]);
		glColor3f(r, g, b);
		glBegin(GL_QUADS);

		float cx = 0.0f, cy = 0.0f; // stb tracks advance in pixel space from 0
		while (*text) {
			if (*text >= 32 && *text < 128) {
				stbtt_aligned_quad q;
				stbtt_GetBakedQuad(g_charData[slot], 512, 512, *text - 32, &cx, &cy, &q, 1);

				float vx0 = nx + q.x0 * scaleX;
				float vx1 = nx + q.x1 * scaleX;
				float vy0 = ny - q.y0 * scaleY;
				float vy1 = ny - q.y1 * scaleY;

				glTexCoord2f(q.s0, q.t1); glVertex2f(vx0, vy1);
				glTexCoord2f(q.s1, q.t1); glVertex2f(vx1, vy1);
				glTexCoord2f(q.s1, q.t0); glVertex2f(vx1, vy0);
				glTexCoord2f(q.s0, q.t0); glVertex2f(vx0, vy0);
			}
			text++;
		}

		glEnd();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	void DrawLine(float sx, float sy, float ex, float ey, float r, float g, float b)
	{
#if APP_USE_VIRTUAL_RES		
		APP_VIRTUAL_TO_NATIVE_COORDS(sx, sy);
		APP_VIRTUAL_TO_NATIVE_COORDS(ex, ey);
#endif
		glBegin(GL_LINES);
		glColor3f(r, g, b); // Yellow
		glVertex2f(sx, sy);
		glVertex2f(ex, ey);
		glEnd();
	}
	
	CSimpleSprite *CreateSprite(const char *fileName, int columns, int rows)
	{
		return new CSimpleSprite(fileName, columns, rows);
	}

	bool IsKeyPressed(int key)
	{
		return ((GetAsyncKeyState(key) & 0x8000) != 0);
	}

	void GetMousePos(float &x, float &y)
	{
		POINT mousePos;
		GetCursorPos(&mousePos);	// Get the mouse cursor 2D x,y position			
		ScreenToClient(MAIN_WINDOW_HANDLE, &mousePos);
		x = (float)mousePos.x;
		y = (float)mousePos.y;
		x = (x * (2.0f / WINDOW_WIDTH) - 1.0f);
		y = -(y * (2.0f / WINDOW_HEIGHT) - 1.0f);

#if APP_USE_VIRTUAL_RES		
		APP_NATIVE_TO_VIRTUAL_COORDS(x, y);
#endif
	}
	void PlaySound(const char *fileName, bool looping)
	{
		DWORD flags = (looping) ? DSBPLAY_LOOPING : 0;
		CSimpleSound::GetInstance().PlaySound(fileName, flags);
	}
	void StopSound(const char *fileName)
	{
		CSimpleSound::GetInstance().StopSound(fileName);
	}
	bool IsSoundPlaying(const char *fileName)
	{
		return CSimpleSound::GetInstance().IsPlaying(fileName);
	}
	// This prints a string to the screen
	void Print(float x, float y, const char *st, float r, float g, float b, void *font)
	{

#if APP_USE_VIRTUAL_RES		
		APP_VIRTUAL_TO_NATIVE_COORDS(x, y);
#endif		
		// Set location to start printing text
		glColor3f(r, g, b); // Yellow
		glRasterPos2f(x, y);
		int l = (int)strlen(st);
		for (int i = 0; i < l; i++)
		{
			glutBitmapCharacter(font, st[i]); // Print a character on the screen
		}
	}
	const CController &GetController( int pad )
	{
		return CSimpleControllers::GetInstance().GetController(pad);
	}

}