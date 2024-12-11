#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QGuiApplication>

#include "qml-renderer.hpp"

#if defined WIN32
	#include <Windows.h>
#elif defined __linux__
	#include <QDataStream>
	#include <qcoreevent.h>
	#include <X11/Xlib.h>
	#include <GL/glx.h>
#elif defined __APPLE__
	#import <Cocoa/Cocoa.h>
#endif


QmlRenderer::QmlRenderer(
	const QString &workingDir, size_t windowHandle, size_t windowContext
) {
	_directoryPath = workingDir;
	
	// Native context takes different argument sets per platform
#if defined WIN32
	QOpenGLContext* extContext = QNativeInterface::QWGLContext::fromNative(
		reinterpret_cast<HGLRC>(windowContext),
		reinterpret_cast<HWND>(windowHandle)
	);
#elif defined __linux__
	if (QGuiApplication::platformName() == "wayland") {
		QOpenGLContext* extContext = QNativeInterface::QEGLContext::fromNative(
			reinterpret_cast<EGLContext>(windowContext),
			reinterpret_cast<EGLDisplay>(windowHandle)
		);
	} else {
		QOpenGLContext* extContext = QNativeInterface::QGLXContext::fromNative(
			reinterpret_cast<GLXContext>(windowContext)
		);
	}
#elif defined __APPLE__
	QOpenGLContext* extContext = QNativeInterface::QCocoaGLContext::fromNative(
		reinterpret_cast<NSOpenGLContext*>(windowContext)
	);
#endif
	
	QSurfaceFormat format;
	format.setDepthBufferSize(16);
	format.setStencilBufferSize(8);
	
	_openglContext = new QOpenGLContext();
	_openglContext->setFormat(format);
	_openglContext->setShareContext(extContext);
	_openglContext->create();
	
	_offscreenSurface = new QOffscreenSurface();
	_offscreenSurface->setFormat(_openglContext->format());
	_offscreenSurface->create();
}


QmlRenderer::~QmlRenderer() {
	_openglContext->makeCurrent(_offscreenSurface);
	
	delete _offscreenSurface;
	_offscreenSurface = nullptr;
	
	delete _openglContext;
	_openglContext = nullptr;
}
