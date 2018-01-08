#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#ifdef WIN32
	
	#include <windows.h>
	typedef HGLRC CtxHandle;
	typedef HWND WndHandle;
	
	#include <QtPlatformHeaders/QWGLNativeContext>
	typedef QWGLNativeContext NativeContext;
	
#else
	
	#error FIXME: non-windows renderer needed
	typedef int CtxHandle;
	typedef int WndHandle;
	
	typedef void NativeContext;
	
#endif

#endif // PLATFORM_HPP
