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
#include "Fractal.h"

#include "RadixSort.sh"
#include "qjulia4D.sh"

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
	DXConstantBuffer& constantBuffer = dx.getConstantBuffer(sizeof(Fractal::MainConstantBuffer));
	DXStructuredBuffer& sortBuffer = dx.getStructuredBuffer(sizeof(float)*4, WINWIDTH * WINHEIGHT);

	DXShader& shader = dx.getComputeShader(g_main, sizeof(g_main));
	DXShader& qjulia = dx.getComputeShader(g_CS_QJulia4D, sizeof(g_CS_QJulia4D));

	Fractal::initialize();

	MSG msg;

	bool running = true;
	while (running) {
		PeekMessage(&msg, windowHandle, 0, 0, PM_REMOVE);
		if (GetAsyncKeyState(VK_ESCAPE)) running = false;

		Fractal::update();

		Fractal::MainConstantBuffer* b = dx.map<Fractal::MainConstantBuffer>(constantBuffer);
		
		b->c_height = WINHEIGHT;
		b->c_width = WINWIDTH;
		Fractal::fill(b);

		b = NULL;
		dx.unmap(constantBuffer);

		// QJulia
		dx.setComputeShader(qjulia);
		dx.setUAV(0,1,sortBuffer.getUAV());
		dx.setConstantBuffer(0,1,constantBuffer);
		dx.runShader(WINWIDTH / THREADSX, WINHEIGHT / THREADSY, 1);

		// Sort
		dx.setComputeShader(shader);
		dx.setUAV(0, 1, backBuffer.getUAV());
		dx.setConstantBuffer(0, 1, constantBuffer);
		dx.setSRV(0, 1, sortBuffer.getSRV());
		dx.runShader(WINWIDTH, WINHEIGHT, 1);

		dx.resetShader();

		dx.present();
	}

	// release buffers and shaders automatically with destructors
	return msg.wParam;
}