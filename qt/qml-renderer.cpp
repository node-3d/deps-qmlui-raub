#include <QOpenGLContext>
#include <QOffscreenSurface>

#include <QMap>
#include <QJsonDocument>

#include "qml-renderer.hpp"
#include "qml-cb.hpp"
#include "keyconv.hpp"
#include "platform.hpp"


QmlRenderer::QmlRenderer(const QString &workingDir, size_t windowHandle, size_t windowContext) {
	
	keyfill();
	
	_directoryPath = workingDir;
	
	QWGLNativeContext nativeContext(
		reinterpret_cast<CtxHandle>(windowContext),
		reinterpret_cast<WndHandle>(windowHandle)
	);
	QOpenGLContext* extContext = new QOpenGLContext;
	extContext->setNativeHandle(QVariant::fromValue(nativeContext));
	extContext->create();
	
	QSurfaceFormat format;
	format.setDepthBufferSize(16);
	format.setStencilBufferSize(8);
	
	_openglContext = new QOpenGLContext;
	_openglContext->setFormat( format );
	_openglContext->setShareContext( extContext );
	_openglContext->create();
	
	_offscreenSurface = new QOffscreenSurface;
	_offscreenSurface->setFormat(_openglContext->format());
	_offscreenSurface->create();
	
}


QmlRenderer::~QmlRenderer() {

	_openglContext->makeCurrent(_offscreenSurface);
	
	delete _offscreenSurface;
	
	delete _openglContext;
	
}


QOpenGLContext *QmlRenderer::context() const {
	return _openglContext;
}


QOffscreenSurface *QmlRenderer::surface() const {
	return _offscreenSurface;
}


QString QmlRenderer::cwd() const {
	return _directoryPath;
}
