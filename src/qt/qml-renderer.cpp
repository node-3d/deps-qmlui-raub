#include <QOffscreenSurface>
// #include <QDebug>

#include "qml-renderer.hpp"
#include "platform.hpp"


QmlRenderer::QmlRenderer(
	const QString &workingDir, size_t windowHandle, size_t windowContext, size_t display
) {
	_directoryPath = workingDir;
	
	// Native context takes different argument sets per platform
	QOpenGLContext* extContext = NativeContext::fromNative(
		reinterpret_cast<CtxHandle>(windowContext)
		#if defined __linux__
		, reinterpret_cast<Display*>(display), reinterpret_cast<WndHandle>(windowHandle), 0
		#elif defined WIN32
		, reinterpret_cast<WndHandle>(windowHandle)
		#endif
	);
	// extContext->create();
	
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
