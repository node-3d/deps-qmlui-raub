#pragma once

#include <QWindow>


class QOpenGLContext;
class QOffscreenSurface;


class QmlRenderer : public QWindow {
	Q_OBJECT
	
public:
	QmlRenderer(const QString &workingDir, size_t windowHandle, size_t windowContext);
	~QmlRenderer();
	
	QOpenGLContext *context() const { return _openglContext; }
	QOffscreenSurface *surface() const { return _offscreenSurface; }
	
	QString cwd() const { return _directoryPath; }
	
private:
	QOpenGLContext *_openglContext;
	QString _directoryPath;
	QOffscreenSurface *_offscreenSurface;
};
