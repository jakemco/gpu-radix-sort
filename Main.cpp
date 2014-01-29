////////////////////////////////////////
//
// Radix Sort in DirectCompute
// by Jacob Maskiewicz
//
// CSE 190: GPU Programming
//
////////////////////////////////////////

#include <Windows.h>

#include "DXWrapper.h"

#include "RadixSort.sh"

// define the size of the window
#define THREADSX 16			// number of threads in the thread group used in the compute shader
#define THREADSY 16			// number of threads in the thread group used in the compute shader
#define WINDOWWIDTH 1280
#define WINDOWHEIGHT 720 

#define WINWIDTH ((((WINDOWWIDTH + THREADSX - 1) / THREADSX) * THREADSX))	// multiply of ThreadsX 
#define WINHEIGHT ((((WINDOWHEIGHT + THREADSY - 1) / THREADSY) * THREADSY)) // multiply of ThreadsY

#define WINPOSX 50 
#define WINPOSY 50

int WINAPI WinMain(
	HINSTANCE hInstance,    // HANDLE TO AN INSTANCE.  This is the "handle" to YOUR PROGRAM ITSELF.
	HINSTANCE hPrevInstance,// USELESS on modern windows (totally ignore hPrevInstance)
	LPSTR szCmdLine,        // Command line arguments.  similar to argv in standard C programs
	int iCmdShow)          // Start window maximized, minimized, etc.
{
	HWND windowHandle = CreateWindow(L"edit", 0, WS_POPUP | WS_VISIBLE, WINPOSX, WINPOSY, WINWIDTH, WINHEIGHT, 0, 0, 0, 0);

	ShowCursor(false);

	DXWrapper dx(windowHandle, WINWIDTH, WINHEIGHT);

	DXTexture& backBuffer = dx.getTexture(0);
	DXStructuredBuffer& sortBuffer = dx.getStructuredBuffer(sizeof(float), WINWIDTH * WINHEIGHT);

	DXShader& shader = dx.getComputeShader(g_main);

	bool running = true;
	while (running) {
		MSG msg;
		PeekMessage(&msg, windowHandle, 0, 0, PM_REMOVE);
		if (GetAsyncKeyState(VK_ESCAPE)) running = false;

		dx.setComputeShader(shader);
		dx.attachStructuredBuffer(sortBuffer);
		dx.runShader(WINWIDTH, WINHEIGHT, 1);

		dx.resetShader();

		dx.present();
	}

	// release buffers and shaders automatically with destructors
}