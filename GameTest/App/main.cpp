


#include "stdafx.h"

#include <windows.h>  
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "app.h"
#include "SimpleSound.h"
#include "SimpleController.h"




int WINDOW_WIDTH = APP_INIT_WINDOW_WIDTH;
int WINDOW_HEIGHT = APP_INIT_WINDOW_HEIGHT;
int VIEWPORT_WIDTH = APP_INIT_WINDOW_WIDTH;
int VIEWPORT_HEIGHT = APP_INIT_WINDOW_HEIGHT;
int VIEWPORT_X = 0;
int VIEWPORT_Y = 0;
HWND MAIN_WINDOW_HANDLE = nullptr;


static const double UPDATE_MAX = ((1.0 / APP_MAX_FRAME_RATE)*1000.0);


double gPCFreq = 0.0;
__int64 gCounterStart = 0;
double gLastTime;




extern void Init();
extern void Update(float deltaTime);
extern void Render();
extern void Shutdown();

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
	{
		printf("Failed to init performance counters.");
	}
	gPCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	gCounterStart = li.QuadPart;
}

double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return (double(li.QuadPart - gCounterStart) / gPCFreq);
}


class CProfiler
{
public:
	CProfiler() : m_startTime(0), m_elapsedTime(0)
	{		
	}
	void Start()
	{
		m_startTime = GetCounter();
	}
	double Stop()
	{ 
		m_elapsedTime = GetCounter() - m_startTime;
		return m_elapsedTime;
	}
	void Print(float x, float y, const char *text)
	{
		char textBuffer[64];
		sprintf(textBuffer, "%s: %0.4f ms", text,m_elapsedTime);
		App::Print(x, y, textBuffer,1.0f,0.0f,1.0f, GLUT_BITMAP_HELVETICA_10);
	}
private:	
	double m_startTime;
	double m_elapsedTime;
};

CProfiler	gUserRenderProfiler;
CProfiler	gUserUpdateProfiler;
CProfiler	gUpdateDeltaTime;
bool		gRenderUpdateTimes = APP_RENDER_UPDATE_TIMES;


void InitGL()
{
	StartCounter();
	gLastTime = GetCounter();
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
}

void Reshape(int w, int h)
{
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT = h;

	float targetAspect = 1024.0f / 720.0f;
	float windowAspect = (float)w / (float)h;

	VIEWPORT_WIDTH = w;
	VIEWPORT_HEIGHT = h;
	VIEWPORT_X = 0;
	VIEWPORT_Y = 0;

	if (windowAspect > targetAspect) {
		
		VIEWPORT_WIDTH = (int)(h * targetAspect);
		VIEWPORT_X = (w - VIEWPORT_WIDTH) / 2;
	} else {
		
		VIEWPORT_HEIGHT = (int)(w / targetAspect);
		VIEWPORT_Y = (h - VIEWPORT_HEIGHT) / 2;
	}

	glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
}





void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);   

	gUserRenderProfiler.Start();	
	Render();						
	gUserRenderProfiler.Stop();
	if (gRenderUpdateTimes)
	{
		gUpdateDeltaTime.Print	 (10, 40, "Update");
		gUserRenderProfiler.Print(10, 25, "User Render");
		gUserUpdateProfiler.Print(10, 10, "User Update");
	}
	glFlush();  
}




void Idle()
{	
	static double prevTime = GetCounter();
	double tick = GetCounter() - prevTime;
	double currentTime = GetCounter();
	double deltaTime = currentTime - gLastTime;
	
	if (deltaTime > (UPDATE_MAX))
	{	
		gUpdateDeltaTime.Stop();
		glutPostRedisplay(); 
		CSimpleControllers::GetInstance().Update();

		gUserUpdateProfiler.Start();
		Update((float)deltaTime);				
		gUserUpdateProfiler.Stop();
		
		gLastTime = currentTime;		
		RECT tileClientArea;
		if (GetClientRect( MAIN_WINDOW_HANDLE, &tileClientArea))
		{
			WINDOW_WIDTH = tileClientArea.right - tileClientArea.left;
			WINDOW_HEIGHT = tileClientArea.bottom - tileClientArea.top;
		}

		if (App::GetController().CheckButton(APP_ENABLE_DEBUG_INFO_BUTTON) )
		{
			gRenderUpdateTimes = !gRenderUpdateTimes;
		}

		if (App::IsKeyPressed(APP_QUIT_KEY))
		{		
			exit(0);
		}

		static bool lastF11 = false;
		bool f11 = App::IsKeyPressed(VK_F11);
		if (f11 && !lastF11)
		{
			glutFullScreenToggle();
		}
		lastF11 = f11;
		gUpdateDeltaTime.Start();
	}
	
}


void CheckMemCallback()
{
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance, 	_In_opt_ HINSTANCE hPrevInstance,	_In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{	
	int argc = 0;	char* argv = "";

	
	const int result_1 = std::atexit(CheckMemCallback);

	
	glutInit(&argc, &argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	int glutWind = glutCreateWindow(APP_WINDOW_TITLE);	
	HDC dc = wglGetCurrentDC();
	MAIN_WINDOW_HANDLE = WindowFromDC(dc);
	glutIdleFunc(Idle);
	glutDisplayFunc(Display);       
	glutReshapeFunc(Reshape);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	InitGL();                       



	
	CSimpleSound::GetInstance().Initialize(MAIN_WINDOW_HANDLE);
	
	
	Init();

	glutFullScreen();

	
	glutMainLoop();
	
	
	Shutdown();	

	
	CSimpleSound::GetInstance().Shutdown();

	
	return 0;
}


