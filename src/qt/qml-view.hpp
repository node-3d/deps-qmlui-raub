#ifndef QML_VIEW_HPP
#define QML_VIEW_HPP

#include <QWindow>
#include <QTimer>
#include <QQmlComponent>

#include <qml-ui.hpp>


class QOpenGLContext;
class QOpenGLFramebufferObject;
class QOffscreenSurface;
class QQuickRenderControl;
class QQuickWindow;
class QQmlEngine;
class QQuickItem;

class QmlCb;
class QmlRenderer;


class QmlView : public QWindow {
	Q_OBJECT
	
public:
	
	QmlView(QmlRenderer * renderer, int w, int h, QmlCb *cb);
	~QmlView();
	
	// Add an import path to QML Engine
	void addLibsDir(const QString &dirName);
	
	// Resize the window
	void resize(const QSize& size);
	
	// Induce a mouse event: button/wheel
	void mouse(int type, int button, int buttons, int x, int y);
	
	// Induce a keyboard event: up/down
	void keyboard(int type, int key, unsigned text);
	
	// Check if the view has loaded the QML
	bool isReady() const { return _isReady; }
	
	// Load QML from filesystem
	void loadQml(const QString &fileName);
	
	// Load QML from string
	void loadText(const QString &source);
	
	// Clear QML scene
	void unload();
	
	// Assign the value to a nested property
	void setProp(
		const char *objname,
		const char *propname,
		const char *json
	);
	
	// Request the value of a nested property
	std::string getProp(
		const char *objname,
		const char *propname
	);
	
	// Invoke a nested method
	std::string invoke(
		const char *objname,
		const char *method,
		const char *json
	);
	
	
private slots:
	
	// React to the status of `main.qml`
	void _rootStatusUpdate(QQmlComponent::Status status);
	
	// React to tse status of user-provided QML
	void _customStatusUpdate(QQmlComponent::Status status);
	
	void _createFramebuffer();
	void _destroyFramebuffer();
	
	// Render QML scene into the framebuffer
	void _render();
	
	// Resize the window components and framebuffer
	void _applySize();
	
	// Schedule re-render in 5msec due to window logic
	void _requestUpdate();
	
	// Schedule re-render in 5msec due to scene change
	void _syncScene();
	
	
private:
	
	// Find an Item by name
	QObject* _findItem(
		QObject* node,
		const QString& name,
		int depth = 0
	) const;
	
	
	// Report an error message to JS
	void _qmlReport(
		const QString &message,
		const QString &type = QString("qml"),
		bool critical = true
	) const;
	
	
	// Check if the given component has some errors. Report if any.
	bool _qmlCheckErrors(const QQmlComponent *component) const;
	
	
	// OpenGL
	QOpenGLContext *_openglContext;
	QOffscreenSurface *_offscreenSurface;
	QQuickRenderControl *_renderControl;
	QQuickWindow *_offscreenWindow;
	QOpenGLFramebufferObject *_framebuffer;
	
	
	// QML
	QQmlEngine *_qmlEngine;
	QQmlComponent *_systemComponent;
	QQuickItem *_systemItem;
	QQuickItem *_systemRoot;
	QQuickItem *_systemError;
	QQmlComponent *_customComponent;
	QQuickItem *_customItem;
	
	
	// States
	bool _hasChanged;
	bool _isReady;
	
	
	// Timers
	QTimer _renderTimer;
	QTimer _resizeTimer;
	
	
	// Callback router
	QmlCb *_cb;
	
	
	// Cache
	QString _currentQml;
	QSize _currentSize;
	std::string _undefined;
};

#endif // QML_VIEW_HPP

