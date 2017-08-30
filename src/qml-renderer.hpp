#ifndef QML_RENDERER_HPP
#define QML_RENDERER_HPP

#include <QWindow>

class QOpenGLContext;
class QOffscreenSurface;


class QmlRenderer : public QWindow {
	Q_OBJECT
	
public:
	QmlRenderer(const QString &workingDir, size_t windowHandle, size_t windowContext);
	~QmlRenderer();
	
	void addLibsDirectory(const QString &dirName);
	
	QOpenGLContext *glContext() const;
	QString cwd() const;
	
private:
	QOpenGLContext    *_openglContext;
	QString            _directoryPath;
	QOffscreenSurface *_offscreenSurface;
	
};

#endif // QML_RENDERER_HPP
