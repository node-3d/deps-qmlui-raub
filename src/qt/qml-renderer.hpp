#ifndef QML_RENDERER_HPP
#define QML_RENDERER_HPP

#include <QWindow>


class QOpenGLContext;
class QOffscreenSurface;


class QmlRenderer : public QWindow {
	Q_OBJECT
	
public:
	QmlRenderer(const QString &workingDir, size_t windowHandle, size_t windowContext, size_t display);
	~QmlRenderer();
	
	QOpenGLContext *context() const { return _openglContext; }
	QOffscreenSurface *surface() const { return _offscreenSurface; }
	
	QString cwd() const { return _directoryPath; }
	
private:
	QOpenGLContext *_openglContext;
	QString _directoryPath;
	QOffscreenSurface *_offscreenSurface;
};


#endif // QML_RENDERER_HPP
