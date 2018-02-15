#ifndef _PLATFORM_HPP_
#define _PLATFORM_HPP_


#if defined WIN32
	
	#include <windows.h>
	typedef HGLRC CtxHandle;
	typedef HWND WndHandle;
	
	#include <QtPlatformHeaders/QWGLNativeContext>
	typedef QWGLNativeContext NativeContext;
	
#elif defined __linux__
	
	#include <X11/Xlib.h>
	typedef VisualID CtxHandle;
	typedef Window WndHandle;
	
	#include <QtPlatformHeaders/QGLXNativeContext>
	typedef QGLXNativeContext NativeContext;
	
#elif defined __APPLE__
	
	#import <Cocoa/Cocoa.h>
	typedef NSOpenGLContext CtxHandle;
	typedef int WndHandle;
	
	#include <QtPlatformHeaders/QCocoaNativeContext>
	typedef QCocoaNativeContext NativeContext;
	
#endif


#endif // _PLATFORM_HPP_
