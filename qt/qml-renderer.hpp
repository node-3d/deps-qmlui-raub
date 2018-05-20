#ifndef _QML_RENDERER_HPP_
#define _QML_RENDERER_HPP_

#include <QWindow>


class QOpenGLContext;
class QOffscreenSurface;


class QmlRenderer : public QWindow {
	Q_OBJECT
	
public:
	
	QmlRenderer(const QString &workingDir, size_t windowHandle, size_t windowContext);
	~QmlRenderer();
	
	QOpenGLContext *context() const;
	QOffscreenSurface *surface() const;
	
	QString cwd() const;
	
	
private:
	
	QOpenGLContext *_openglContext;
	QString _directoryPath;
	QOffscreenSurface *_offscreenSurface;
	
};


#endif // _QML_RENDERER_HPP_
