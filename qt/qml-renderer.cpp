#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QDebug>

#include "qml-renderer.hpp"
#include "platform.hpp"


QmlRenderer::QmlRenderer(
	const QString &workingDir, size_t windowHandle, size_t windowContext
) {
	
	_directoryPath = workingDir;
	qDebug() << ">>>> CONTEXT RECEIVED";
	// Native context takes different argument sets per platform
	NativeContext nativeContext(
		reinterpret_cast<CtxHandle>(windowContext)
		#if defined __linux__
		, 0, 0, 0
		#elif defined WIN32
		, reinterpret_cast<WndHandle>(windowHandle)
		#endif
	);
	qDebug() << ">>>> CONTEXT ADOPTED";
	QOpenGLContext* extContext = new QOpenGLContext();
	extContext->setNativeHandle(QVariant::fromValue(nativeContext));
	extContext->create();
	qDebug() << ">>>> EXT CONTEXT CREATED";
	QSurfaceFormat format;
	format.setDepthBufferSize(16);
	format.setStencilBufferSize(8);
	
	_openglContext = new QOpenGLContext();
	_openglContext->setFormat(format);
	_openglContext->setShareContext(extContext);
	_openglContext->create();
	qDebug() << ">>>> OWN CONTEXT CREATED";
	_offscreenSurface = new QOffscreenSurface();
	_offscreenSurface->setFormat(_openglContext->format());
	_offscreenSurface->create();
	qDebug() << ">>>> SURFACE CREATED";
}


QmlRenderer::~QmlRenderer() {
	
	_openglContext->makeCurrent(_offscreenSurface);
	
	delete _offscreenSurface;
	_offscreenSurface = nullptr;
	
	delete _openglContext;
	_openglContext = nullptr;
	
}
