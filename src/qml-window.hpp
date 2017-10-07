#ifndef QML_WINDOW_HPP
#define QML_WINDOW_HPP

#include <QWindow>
#include <QTimer>
#include <QQmlComponent>

#include "qmlui.hpp"
#include "platform.hpp"


class QOpenGLContext;
class QOpenGLFramebufferObject;
class QOffscreenSurface;
class QQuickRenderControl;
class QQuickWindow;
class QQmlEngine;
class QQuickItem;

class QmlCb;
class QmlRenderer;


class QmlWindow : public QWindow {
	Q_OBJECT
	
public:
	QmlWindow(QmlRenderer * renderer, int w, int h, EventCb eventCb);
	~QmlWindow();
	
	void addLibsDir(const QString &dirName);
	
	void resize(const QSize& size);
	void mouse(int type, int button, int buttons, int x, int y);
	void keyboard(int type, int key, char text);
	
	bool isReady() { return _isReady; }
	void loadQml(const QString &fileName);
	void loadText(const QString &source);
	void unload();
	
	void setProp(const QString &objname, const QByteArray &propname, const QByteArray &json);
	void getProp(const QString &objname, const QByteArray &propname);
	void invoke(const QString &objname, const QByteArray &method, const QByteArray &json);
	
	void confirm();
	
	
private slots:
	void _rootStatusUpdate(QQmlComponent::Status status);
	void _customStatusUpdate(QQmlComponent::Status status);
	
	void _createFramebuffer();
	void _destroyFramebuffer();
	
	void _render();
	void _applySize();
	
	void _requestUpdate();
	void _syncScene();
	
	
private:
	void _resizeFbo();
	
	QQuickItem* _findItem(QObject* node, const QString& name, int depth = 0) const;
	
	void _qmlReport(const QString &message, const QString &type = QString("qml")) const;
	bool _qmlCheckErrors(const QQmlComponent *component) const;
	
	
	
	QOpenGLContext           *_openglContext;
	QOffscreenSurface        *_offscreenSurface;
	QQuickRenderControl      *_renderControl;
	QQuickWindow             *_offscreenWindow;
	QOpenGLFramebufferObject *_framebuffer;

	QQmlEngine    *_qmlEngine;

	QQmlComponent *_systemComponent;
	QQuickItem    *_systemItem;
	QQuickItem    *_systemRoot;

	QQmlComponent *_customComponent;
	QQuickItem    *_customItem;

	bool _hasChanged;
	bool _isReady;
	bool _hasConfirmed;

	QTimer _renderTimer;
	QTimer _resizeTimer;

	QmlCb *_cb;

	QString _currentQml;

	QSize _currentSize;

};

#endif // QML_WINDOW_HPP

