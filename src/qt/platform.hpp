#ifndef PLATFORM_HPP
#define PLATFORM_HPP


#if defined WIN32
	
	#include <Windows.h>
	typedef HGLRC CtxHandle;
	typedef HWND WndHandle;
	
	#include <QtPlatformHeaders/QWGLNativeContext>
	typedef QWGLNativeContext NativeContext;
	
#elif defined __linux__
	
	#include <QDataStream>
	#include <qcoreevent.h>
	
	#include <X11/Xlib.h>
	#include <GL/glx.h>
	typedef GLXContext CtxHandle;
	typedef Window WndHandle;
	
	#include <QtPlatformHeaders/QGLXNativeContext>
	// GLXContext ctx, Display *dpy = 0, Window wnd = 0, VisualID vid = 0
	typedef QGLXNativeContext NativeContext;
	
#elif defined __APPLE__
	
	#import <Cocoa/Cocoa.h>
	typedef NSOpenGLContext *CtxHandle;
	typedef int WndHandle;
	
	#include <QtPlatformHeaders/QCocoaNativeContext>
	typedef QCocoaNativeContext NativeContext;
	
#endif


#endif // PLATFORM_HPP
