#pragma once

#include <QOpenGLContext>

#if defined WIN32
	
	#include <Windows.h>
	typedef HGLRC CtxHandle;
	typedef HWND WndHandle;
	
	typedef QNativeInterface::QWGLContext NativeContext;
	
#elif defined __linux__
	
	#include <QDataStream>
	#include <qcoreevent.h>
	
	#include <X11/Xlib.h>
	#include <GL/glx.h>
	typedef GLXContext CtxHandle;
	typedef Window WndHandle;
	
	// GLXContext ctx, Display *dpy = 0, Window wnd = 0, VisualID vid = 0
	typedef QNativeInterface::QGLXContext NativeContext;
	
#elif defined __APPLE__
	
	#import <Cocoa/Cocoa.h>
	typedef NSOpenGLContext *CtxHandle;
	typedef int WndHandle;
	
	typedef QNativeInterface::QCocoaGLContext NativeContext;
	
#endif
