#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOffscreenSurface>
#include <QScreen>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQuickRenderControl>
#include <QCoreApplication>
#include <QByteArray>
#include <QMap>
#include <QJsonDocument>

#include "qml-renderer.hpp"
#include "qml-window.hpp"
#include "qml-cb.hpp"
#include "keyconv.hpp"


QmlWindow::QmlWindow(QmlRenderer *renderer, int w, int h, EventCb eventCb) {
	
	// Initial values all zero
	_customItem      = nullptr;
	_customComponent = nullptr;
	
	_hasChanged   = false;
	_isReady      = false;
	_hasConfirmed = false;
	
	// Wrap JS event callback
	_cb = new QmlCb(this, eventCb);
	
	// Tell window it's gonna be OpenGL based
	setSurfaceType(QSurface::OpenGLSurface);
	
	// Grab GL context and surface from owner renderer
	_openglContext    = renderer->context();
	_offscreenSurface = renderer->surface();
	
	// Create a window with custom RenderControl to render offscreen
	_renderControl   = new QQuickRenderControl();
	_offscreenWindow = new QQuickWindow( _renderControl );
	
	// Set window looks accordingly
	_offscreenWindow->setGeometry(0, 0, w, h);
	_offscreenWindow->contentItem()->setSize( QSizeF(_offscreenWindow->size()) );
	_offscreenWindow->setColor(Qt::transparent);
	
	// Create a separate instance of QML Engine for this window
	_qmlEngine = new QQmlEngine;
	if ( ! _qmlEngine->incubationController() ) {
		_qmlEngine->setIncubationController(_offscreenWindow->incubationController());
	}
	
	// Apply additional library paths
	foreach (const QString &dir, QCoreApplication::libraryPaths()) {
		QString replaced = dir;
		replaced = replaced.replace(QRegExp("plugins$"), "qml");
		_qmlEngine->addImportPath(replaced);
	}
	
	// Provide globals for QML to be on the same page with us
	_qmlEngine->rootContext()->setContextProperty("CWD", renderer->cwd());
	_qmlEngine->rootContext()->setContextProperty("cb", _cb);
	
	// Debounced rendering with singleshot timer
	_renderTimer.setSingleShot(true);
	_renderTimer.setInterval(5);
	connect(&_renderTimer, &QTimer::timeout, this, &QmlWindow::_render);
	
	// Debounced resizing, must be enough time, resize is costly
	_resizeTimer.setSingleShot(true);
	_resizeTimer.setInterval(300);
	connect(&_resizeTimer, &QTimer::timeout, this, &QmlWindow::_applySize);
	
	// Set up event listeners
	connect(
		_offscreenWindow, &QQuickWindow::sceneGraphInitialized,
		this,             &QmlWindow::_createFramebuffer
	);
	connect(
		_offscreenWindow, &QQuickWindow::sceneGraphInvalidated,
		this,             &QmlWindow::_destroyFramebuffer
	);
	connect(
		_renderControl, &QQuickRenderControl::renderRequested,
		this,           &QmlWindow::_requestUpdate
	);
	connect(
		_renderControl, &QQuickRenderControl::sceneChanged,
		this,           &QmlWindow::_syncScene
	);
	
	// Init system component, ASYNC
	_systemComponent = new QQmlComponent( _qmlEngine );
	connect( _systemComponent, &QQmlComponent::statusChanged, this, &QmlWindow::_rootStatusUpdate );
	_systemComponent->loadUrl(QString("qrc:/main.qml"), QQmlComponent::Asynchronous);
	
}

QmlWindow::~QmlWindow() {

	// Make sure the context is current while doing cleanup. Note that we use the
	// offscreen surface here because passing 'this' at this point is not safe: the
	// underlying platform window may already be destroyed. To avoid all the trouble, use
	// another surface that is valid for sure.
	_openglContext->makeCurrent(_offscreenSurface);
	
	// Delete the render control first since it will free the scenegraph resources.
	// Destroy the QQuickWindow only afterwards.
	delete _renderControl;
	delete _offscreenWindow;
	
	// Now that scene is clear (no component based items) - delete the components
	delete _customComponent;
	delete _systemComponent;
	
	// Free the engine and FBO
	delete _qmlEngine;
	delete _framebuffer;
	
	_openglContext->doneCurrent();
	
}


// Create a new FBO
void QmlWindow::_createFramebuffer() {
	
	// Get hold of a new FBO
	_framebuffer = new QOpenGLFramebufferObject(
		_offscreenWindow->size(),
		QOpenGLFramebufferObject::CombinedDepthStencil
	);
	
	// Set the Window to render into it
	_offscreenWindow->setRenderTarget(_framebuffer);
	
	// Respond to JS: GL texture id of a new FBO
	QVariantMap pmap;
	pmap["texture"] = _framebuffer->texture();
	_cb->call("fbo", pmap);
	
}


// Dispose FBO
void QmlWindow::_destroyFramebuffer() {
	delete _framebuffer;
	_framebuffer = nullptr;
}


// Update and render the scene
void QmlWindow::_render() {
	
	if ( ! _openglContext->makeCurrent(_offscreenSurface) ) {
		return;
	}
	
	// Only sync scene content if it has some changes
	if (_hasChanged) {
		_hasChanged = false;
		_renderControl->polishItems();
		_renderControl->sync();
	}
	
	_renderControl->render();
	
	// Finish OpenGL business
	_offscreenWindow->resetOpenGLState();
	_openglContext->functions()->glFlush();
	_openglContext->doneCurrent();
	
}


// Activate/reactivate resize timer for debounce
void QmlWindow::resize(const QSize &size) {
	
	_currentSize = size;
	
	if ( _resizeTimer.isActive() ) {
		_resizeTimer.stop();
	}
	
	_resizeTimer.start();
	
}


// Update the size of the Window
void QmlWindow::_applySize() {
	
	// Both window and content
	_offscreenWindow->setGeometry(0, 0, _currentSize.width(), _currentSize.height());
	_offscreenWindow->contentItem()->setSize( QSizeF(_currentSize) );
	
	// Also reset render target
	if ( _openglContext && _openglContext->makeCurrent( _offscreenSurface ) ) {
		_destroyFramebuffer();
		_createFramebuffer();
		_openglContext->doneCurrent();
	}
	
}


// Call for render if not called yet
void QmlWindow::_requestUpdate() {
	
	if ( ! _renderTimer.isActive() ) {
		_renderTimer.start();
	}
	
}


// Scene changed, set changed flag and call for render
void QmlWindow::_syncScene() { _hasChanged = true; _requestUpdate(); }


// Report an error message to JS
void QmlWindow::_qmlReport(const QString &message, const QString &type) const {
	QVariantMap pmap;
	pmap["message"] = message;
	pmap["type"] = type;
	_cb->call("error", pmap);
}


// Check if the given component has some errors. Report if any.
bool QmlWindow::_qmlCheckErrors(const QQmlComponent *component) const {
	
	// If there are errors, report each of them
	if (component->isError()) {
		QList<QQmlError> errorList = component->errors();
		foreach (const QQmlError &error, errorList) {
			_qmlReport(error.toString());
		}
		return true;
	}
	
	return false;
	
}


// Status listener for System Component
void QmlWindow::_rootStatusUpdate(QQmlComponent::Status status) {
	
	// If not ready yet, then only check for errors
	if( QQmlComponent::Ready != status ) {
		_qmlCheckErrors(_systemComponent);
		return;
	}
	
	// Prevent further status changes
	disconnect(_systemComponent, &QQmlComponent::statusChanged, this, &QmlWindow::_rootStatusUpdate);
	
	// If any errors - quit
	if (_qmlCheckErrors(_systemComponent)) {
		return;
	}
	
	// Instantiate the system component
	QObject *rootObject = _systemComponent->create();
	if (_qmlCheckErrors(_systemComponent)) {
		return;
	}
	
	// Now extract the root QML Item
	_systemItem = qobject_cast<QQuickItem *>(rootObject);
	if ( ! _systemItem ) {
		_qmlReport("Not a QQuickItem: main.qml");
		delete rootObject;
		return;
	}
	
	// Attach the root Item to the Window
	_systemItem->setParentItem(_offscreenWindow->contentItem());
	_systemItem->setSize( QSizeF(_offscreenWindow->size()) );
	
	// Find the mounting point
	_systemRoot = qobject_cast<QQuickItem *>(_systemItem->findChild<QObject*>("__root"));
	
	// Initialize the render control and our OpenGL resources.
	_openglContext->makeCurrent( _offscreenSurface );
	_renderControl->initialize( _openglContext );
	_openglContext->doneCurrent();
	
	// Now system is ready to load user ui
	_isReady = true;
	if (_hasConfirmed) {
		_cb->call("ready", QVariantMap());
	}
	
}


// Status listener for User UI Component
void QmlWindow::_customStatusUpdate(QQmlComponent::Status status) {
	
	QVariantMap result;
	result["source"] = _currentQml;
	result["status"] = "error";
	
	// If not ready yet, then only check for errors
	if( QQmlComponent::Ready != status ) {
		_qmlCheckErrors(_customComponent);
		_cb->call("load", result);
		return;
	}
	
	// Prevent further status changes
	disconnect(_customComponent, &QQmlComponent::statusChanged, this, &QmlWindow::_customStatusUpdate);
	
	// If any errors - quit
	if (_qmlCheckErrors(_customComponent)) {
		_cb->call("load", result);
		return;
	}
	
	// Instantiate user component
	QObject *rootObject = _customComponent->create();
	if (_qmlCheckErrors(_customComponent)) {
		_cb->call("load", result);
		return;
	}
	
	// Check validity of the root Item
	_customItem = qobject_cast<QQuickItem *>(rootObject);
	if ( ! _customItem ) {
		_qmlReport("Not a QQuickItem: " + _currentQml);
		delete rootObject;
		_cb->call("load", result);
		return;
	}
	
	// Attach user's UI to the mounting point
	_customItem->setParentItem(_systemRoot);
	
	// Report success to JS
	result["status"] = "success";
	_cb->call("load", result);
	
}


// Load a QML file from the given source. Source must have an absolute path.
void QmlWindow::loadQml(const QString &fileName) {
	
	unload();
	
	// Setup the new component
	_currentQml = fileName;
	_customComponent = new QQmlComponent( _qmlEngine );
	connect( _customComponent, &QQmlComponent::statusChanged, this, &QmlWindow::_customStatusUpdate );
	_customComponent->loadUrl(QUrl::fromLocalFile(_currentQml), QQmlComponent::Asynchronous);
	
}


// Parse the given text as QML file
void QmlWindow::loadText(const QString &source) {
	
	unload();
	
	// Setup the new component
	_currentQml = source;
	_customComponent = new QQmlComponent( _qmlEngine );
	connect( _customComponent, &QQmlComponent::statusChanged, this, &QmlWindow::_customStatusUpdate );
	_customComponent->setData( source.toUtf8(), QString("loadText") );
	
}


// If something is currently loaded, disregard that
void QmlWindow::unload() {
	if (_customComponent) {
		if (_customItem) {
			_customItem->deleteLater();
			_customItem = nullptr;
		}
		_customComponent->deleteLater();
		_customComponent = nullptr;
	}
}


// Set the property value on a given object, by "objectName", not "id"
void QmlWindow::setProp(const QString &objname, const QByteArray &propname, const QByteArray &json) {
	
	if ( ! _customItem ) {
		_qmlReport("Qml setProp() failed. Do a proper loadQml() first.", "prop");
		return;
	}
	
	QQuickItem *obj = _findItem(_customItem, objname);
	
	if ( ! obj ) {
		_qmlReport(QString("Qml setProp() failed. Object not found: ") + objname, "prop");
		return;
	}
	QByteArray enclosed = QByteArray("[") + json + QByteArray("]");
	QList<QVariant> parsed = QJsonDocument::fromJson(enclosed).toVariant().toList();
	
	if ( ! parsed.size() ) {
		_qmlReport(QString("Qml setProp() failed. Invalid value: '") + json + QString("'"), "prop");
		return;
	}
	
	obj->setProperty(propname.constData(), parsed.at(0));
	
}


void QmlWindow::getProp(const QString &objname, const QByteArray &propname) {
	
	if ( ! _customItem ) {
		_qmlReport("Qml getProp() failed. Do a proper loadQml() first.", "prop");
		return;
	}
	
	QQuickItem *obj = _findItem(_customItem, objname);
	
	if ( ! obj ) {
		_qmlReport(QString("Qml getProp() failed. Object not found: ") + objname, "prop");
		return;
	}
	
	QVariant prop = obj->property(propname.constData());
	QVariantMap e;
	e["name"] = objname;
	e["key"] = propname;
	e["value"] = prop;
	_cb->call("get", e);
	
}


void QmlWindow::invoke(const QString &objname, const QByteArray &method, const QByteArray &json) {
	
	if ( ! _customItem ) {
		_qmlReport("Qml invoke() failed. Do a proper loadQml() first.", "invoke");
		return;
	}
	
	QQuickItem *obj = _findItem(_customItem, objname);
	
	if ( ! obj ) {
		_qmlReport(QString("Qml invoke() failed. Object not found: ") + objname, "invoke");
		return;
	}
	
	QByteArray enclosed = QByteArray("[") + json + QByteArray("]");
	QList<QVariant> parsed = QJsonDocument::fromJson(enclosed).toVariant().toList();
	
	if ( ! parsed.size() ) {
		_qmlReport(QString("Qml invoke() failed. Invalid value: '") + json + QString("'"), "prop");
		return;
	}
	
	QMetaObject::invokeMethod(obj, method.constData(), Q_ARG(QVariant, parsed.at(0)));
	
}


// Confirm Window status (in case it's ready too soon)
void QmlWindow::confirm() {
	_hasConfirmed = true;
	if (_isReady) {
		_cb->call("ready");
	}
}


// Apply mouse event
void QmlWindow::mouse(int type, int button, int buttons, int x, int y) {
	
	QPointF mousePoint(x, y);
	
	Qt::MouseButton qbutton = static_cast<Qt::MouseButton>(1 << button);
	Qt::MouseButton qbuttons = static_cast<Qt::MouseButton>(buttons);
	
	QMouseEvent mouseEvent = QMouseEvent(
					QEvent::MouseMove,
					mousePoint, mousePoint,
					Qt::NoButton, Qt::NoButton, 0 );
	switch (type) {
	default:
	case 0: mouseEvent = QMouseEvent(
					QEvent::MouseMove,
					mousePoint, mousePoint,
					qbutton, qbuttons, 0 );
			break;
	case 1: mouseEvent = QMouseEvent(
					QEvent::MouseButtonPress,
					mousePoint, mousePoint,
					qbutton, qbuttons, 0 );
			break;
	case 2: mouseEvent = QMouseEvent(
					QEvent::MouseButtonRelease,
					mousePoint, mousePoint,
					qbutton, qbuttons, 0 );
			break;
	}
	
	QCoreApplication::sendEvent( _offscreenWindow, &mouseEvent );
	
}


// Apply keyboard events
// FIXME: eliminate incorrect key codes
void QmlWindow::keyboard(int type, int key, char text) {
	
	Qt::Key qtKey = keyconv(key);
	QKeySequence seq = QKeySequence(qtKey);
	
	//qDebug() << "KEY" << (type ? "KeyPress" : "KeyRelease") << seq << qtKey;
	
	QKeyEvent keyEvent( type ? QEvent::KeyPress : QEvent::KeyRelease, qtKey, Qt::NoModifier, QString(text), text != 0);
	QCoreApplication::sendEvent( _offscreenWindow, &keyEvent );
	
}


// Tell QML Engine where extra files are
void QmlWindow::addLibsDir(const QString &dirName) {
	_qmlEngine->addImportPath(dirName);
}


// Find Item by name
QQuickItem* QmlWindow::_findItem(QObject* node, const QString& name, int depth) const {
	
	if (node && node->objectName() == name){
	
		return qobject_cast<QQuickItem *>(node);
	
	} else if (node && node->children().size() > 0) {
	
		for (int i = node->children().size() - 1; i >= 0; i--) {
			QQuickItem* item = _findItem(node->children().at(i), name, depth + 1);
			if (item) {
				return item;
			}
		}
	
	}
	
	// not found
	return nullptr;
	
}
