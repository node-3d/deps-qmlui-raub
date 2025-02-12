#include <QByteArray>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QOffscreenSurface>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickRenderControl>
#include <QQuickWindow>
#include <QThread>
#include <QDebug>
#include <QQuickOpenGLUtils>
#include <QQuickGraphicsDevice>
#include <QQuickRenderTarget>

#include "qml-renderer.hpp"
#include "qml-view.hpp"
#include "qml-cb.hpp"


QRegularExpression regexPlugins = QRegularExpression("plugins$");


QmlView::QmlView(QmlRenderer *renderer, int w, int h, QmlCb *cb) {
	// Initial values all zero
	_systemItem = nullptr;
	_systemComponent = nullptr;
	_systemRoot = nullptr;
	_systemError = nullptr;
	
	_customItem = nullptr;
	_customComponent = nullptr;
	
	_hasChanged = false;
	_isReady = false;
	
	_cb = cb;
	
	_undefined = std::string("[null]");
	
	// Tell window it's gonna be OpenGL based
	setSurfaceType(QSurface::OpenGLSurface);
	
	// Grab GL context and surface from owner renderer
	_openglContext = renderer->context();
	
	_offscreenSurface = renderer->surface();
	
	// Create a window with custom RenderControl to render offscreen
	_renderControl = new QQuickRenderControl();
	_offscreenWindow = new QQuickWindow(_renderControl);
	_framebuffer = nullptr;
	
	// Set window looks accordingly
	_offscreenWindow->setGeometry(0, 0, w, h);
	_offscreenWindow->setGraphicsApi(QSGRendererInterface::OpenGL);
	_offscreenWindow->setGraphicsDevice(
		QQuickGraphicsDevice::fromOpenGLContext(_openglContext)
	);
	
	_currentSize = QSize(w, h);
	_offscreenWindow->contentItem()->setSize(QSizeF(_offscreenWindow->size()));
	_offscreenWindow->setColor(Qt::transparent);
	
	// Create a separate instance of QML Engine for this window
	_qmlEngine = new QQmlEngine();
	if (!_qmlEngine->incubationController()) {
		_qmlEngine->setIncubationController(
			_offscreenWindow->incubationController()
		);
	}
	
	// Apply additional library paths
	foreach (const QString &dir, QCoreApplication::libraryPaths()) {
		QString replaced = dir;
		replaced = replaced.replace(regexPlugins, "qml");
		_qmlEngine->addImportPath(replaced);
	}
	
	// Provide globals for QML to be on the same page with us
	_qmlEngine->rootContext()->setContextProperty("CWD", renderer->cwd());
	QJSValue wrapobj = _qmlEngine->newQObject(_cb);
	_qmlEngine->globalObject().setProperty(
		"__eventWrapper",
		wrapobj
	);
	_qmlEngine->globalObject().setProperty(
		"eventEmit",
		wrapobj.property("eventEmit")
	);
	
	// Debounced rendering with singleshot timer
	_renderTimer.setSingleShot(true);
	_renderTimer.setInterval(5);
	connect(&_renderTimer, &QTimer::timeout, this, &QmlView::_render);
	
	// Debounced resizing, must be enough time, resize is costly
	_resizeTimer.setSingleShot(true);
	_resizeTimer.setInterval(300);
	connect(&_resizeTimer, &QTimer::timeout, this, &QmlView::_applySize);
	
	// Set up event listeners
	connect(
		_offscreenWindow, &QQuickWindow::sceneGraphInitialized,
		this, &QmlView::_createFramebuffer
	);
	
	connect(
		_offscreenWindow, &QQuickWindow::sceneGraphInvalidated,
		this, &QmlView::_destroyFramebuffer
	);
	connect(
		_renderControl, &QQuickRenderControl::renderRequested,
		this, &QmlView::_requestUpdate
	);
	connect(
		_renderControl, &QQuickRenderControl::sceneChanged,
		this, &QmlView::_syncScene
	);
	
	// Init system component, ASYNC
	_systemComponent = new QQmlComponent(_qmlEngine);
	connect(
		_systemComponent, &QQmlComponent::statusChanged,
		this, &QmlView::_rootStatusUpdate
	);
	_systemComponent->loadUrl(QString("qrc:/main.qml"), QQmlComponent::Asynchronous);
}

QmlView::~QmlView() {
	if (QThread::currentThread() != this->thread()) {
		// Well, it's not my business!
		return;
	}
	
	// Make sure the context is current while doing cleanup. Note that we use the
	// offscreen surface here because passing 'this' at this point is not safe: the
	// underlying platform window may already be destroyed. To avoid all the trouble, use
	// another surface that is valid for sure.
	_openglContext->makeCurrent(_offscreenSurface);
	
	// Delete the render control first since it will free the scenegraph resources.
	// Destroy the QQuickWindow only afterwards.
	_renderControl->deleteLater();
	_offscreenWindow->deleteLater();
	
	// Now that scene is clear (no component based items) - delete the components
	_customComponent->deleteLater();
	_systemComponent->deleteLater();
	
	// Free the engine and FBO
	_qmlEngine->deleteLater();
	delete _framebuffer;
	
	_openglContext->doneCurrent();
}


// Create a new FBO
void QmlView::_createFramebuffer() {
	if (!_openglContext->makeCurrent(_offscreenSurface)) {
		return;
	}
	
	// Only delete after new FBO was allocated
	QOpenGLFramebufferObject *_oldFramebuffer = _framebuffer;
	
	// Get hold of a new FBO
	_framebuffer = new QOpenGLFramebufferObject(
		_offscreenWindow->size(),
		QOpenGLFramebufferObject::CombinedDepthStencil
	);
	
	// HACK: forces the new FBO to (CERTAINLY) have a different GL id
	delete _oldFramebuffer;
	
	// Set the Window to render into it
	_offscreenWindow->setRenderTarget(QQuickRenderTarget::fromOpenGLTexture(
		_framebuffer->texture(), _offscreenWindow->size(), 1
	));
	
	// Respond to JS: GL texture id of a new FBO
	QVariantMap pmap;
	pmap["texture"] = _framebuffer->texture();
	_cb->eventEmit("_qml_fbo", pmap);
}


// Dispose FBO
void QmlView::_destroyFramebuffer() {
	delete _framebuffer;
	_framebuffer = nullptr;
}


// Update and render the scene
void QmlView::_render() {
	if (!_openglContext->makeCurrent(_offscreenSurface)) {
		return;
	}
	
	_renderControl->beginFrame();
	
	// Only sync scene content if it has some changes
	if (_hasChanged) {
		_hasChanged = false;
		_renderControl->polishItems();
		_renderControl->sync();
	}
	
	_renderControl->render();
	_renderControl->endFrame();
	
	// Finish OpenGL business
	QQuickOpenGLUtils::resetOpenGLState();
	_framebuffer->bind();
	_openglContext->functions()->glFlush();
}


// Activate/reactivate resize timer for debounce
void QmlView::resize(const QSize &size) {
	_currentSize = size;
	
	if (_resizeTimer.isActive()) {
		_resizeTimer.stop();
	}
	
	_resizeTimer.start();
}


// Update the size of the Window
void QmlView::_applySize() {
	// Both window and content
	_offscreenWindow->setGeometry(0, 0, _currentSize.width(), _currentSize.height());
	_offscreenWindow->contentItem()->setSize(QSizeF(_currentSize));
	
	// Also reset render target
	if (_openglContext && _openglContext->makeCurrent(_offscreenSurface)) {
		_createFramebuffer();
	}
}


// Call for render if not called yet
void QmlView::_requestUpdate() {
	if (!_renderTimer.isActive()) {
		_renderTimer.start();
	}
}


// Scene changed, set changed flag and call for render
void QmlView::_syncScene() {
	_hasChanged = true; _requestUpdate();
}


// Reports the error as an event and shows it on the screen if possible
void QmlView::_qmlReport(const QString &message, const QString &type, bool critical) const {
	if (critical && _systemError) {
		_systemError->setVisible(true);
		QString currentMessage = _systemError->property("text").toString();
		_systemError->setProperty(
			"text",
			currentMessage + (currentMessage.length() ? "\n" : "") + message
		);
	}
	
	QVariantMap pmap;
	pmap["message"] = message;
	pmap["type"] = type;
	_cb->eventEmit("_qml_error", pmap);
}


bool QmlView::_qmlCheckErrors(const QQmlComponent *component) const {
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
void QmlView::_rootStatusUpdate(QQmlComponent::Status status) {
	// If not ready yet, then only check for errors
	if( QQmlComponent::Ready != status ) {
		_qmlCheckErrors(_systemComponent);
		return;
	}
	
	// Prevent further status changes
	disconnect(_systemComponent, &QQmlComponent::statusChanged, this, &QmlView::_rootStatusUpdate);
	
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
	_systemError = qobject_cast<QQuickItem *>(_systemItem->findChild<QObject*>("__error"));
	
	// Initialize the render control and our OpenGL resources.
	_openglContext->makeCurrent(_offscreenSurface);
	_renderControl->initialize();
	// _renderControl->initialize( _openglContext );
	
	// Now system is ready to load user ui
	_isReady = true;
	_cb->eventEmit("_qml_ready", QVariantMap());
}


// Status listener for User UI Component
void QmlView::_customStatusUpdate(QQmlComponent::Status status) {
	QVariantMap result;
	result["source"] = _currentQml;
	result["status"] = "error";
	
	// If not ready yet, then only check for errors
	if (QQmlComponent::Ready != status ) {
		if (!_qmlCheckErrors(_customComponent)) {
			result["status"] = "loading";
		}
		
		_cb->eventEmit("_qml_load", result);
		return;
	}
	
	// Prevent further status changes
	disconnect(
		_customComponent, &QQmlComponent::statusChanged,
		this, &QmlView::_customStatusUpdate
	);
	
	// If any errors - quit
	if (_qmlCheckErrors(_customComponent)) {
		_cb->eventEmit("_qml_load", result);
		return;
	}
	
	// Instantiate user component
	QObject *rootObject = _customComponent->create();
	if (_qmlCheckErrors(_customComponent)) {
		_cb->eventEmit("_qml_load", result);
		return;
	}
	
	// Check validity of the root Item
	_customItem = qobject_cast<QQuickItem *>(rootObject);
	if (!_customItem) {
		_qmlReport("Not a QQuickItem: " + _currentQml);
		delete rootObject;
		rootObject = nullptr;
		_cb->eventEmit("_qml_load", result);
		return;
	}
	
	// Attach user's UI to the mounting point
	_customItem->setParentItem(_systemRoot);
	
	// Report success to JS
	result["status"] = "success";
	_cb->eventEmit("_qml_load", result);
}


// Load a QML file from the given source. Source must have an absolute path.
void QmlView::loadQml(const QString &fileName) {
	unload();
	
	// Setup the new component
	_currentQml = fileName;
	_customComponent = new QQmlComponent(_qmlEngine);
	connect(
		_customComponent, &QQmlComponent::statusChanged,
		this, &QmlView::_customStatusUpdate
	);
	_customComponent->loadUrl(
		QUrl::fromLocalFile(_currentQml), QQmlComponent::Asynchronous
	);
}


// Parse the given text as QML file
void QmlView::loadText(const QString &source) {
	unload();
	
	// Setup the new component
	_currentQml = source;
	_customComponent = new QQmlComponent( _qmlEngine );
	connect(
		_customComponent, &QQmlComponent::statusChanged,
		this, &QmlView::_customStatusUpdate
	);
	_customComponent->setData( source.toUtf8(), QString("loadText") );
}


// If something is currently loaded, disregard that
void QmlView::unload() {
	if (_systemError) {
		_systemError->setVisible(false);
	}
	
	if (!_customComponent) {
		return;
	}
	
	if (_customItem) {
		_customItem->deleteLater();
		_customItem = nullptr;
	}
	_customComponent->deleteLater();
	_customComponent = nullptr;
}


// Set the property value on a given object, by "objectName", not "id"
void QmlView::setProp(
	const char *objname,
	const char *propname,
	const char *json
) {
	if (!_customItem) {
		_qmlReport(
			"Qml setProp() failed. Do a proper loadQml() first.",
			"prop",
			false
		);
		return;
	}
	
	QString strName = QString(objname);
	QObject *obj = _findItem(_customItem, strName);
	
	if (!obj) {
		_qmlReport(
			QString("Qml setProp() failed. Object not found: ") + strName,
			"prop",
			false
		);
		return;
	}
	
	QList<QVariant> parsed = QJsonDocument::fromJson(json)
		.toVariant()
		.toList();
	
	if (!parsed.size()) {
		_qmlReport(
			QString(
				"Qml setProp() failed. Invalid value: '%1'"
			).arg(
				json
			),
			"prop",
			false
		);
		return;
	}
	
	obj->setProperty(propname, parsed.at(0));
}


std::string QmlView::getProp(const char *objname, const char *propname) {
	if (!_customItem) {
		_qmlReport(
			"Qml getProp() failed. Do a proper loadQml() first.",
			"prop",
			false
		);
		return _undefined;
	}
	
	QString strName = QString(objname);
	QObject *object = _findItem(_customItem, strName);
	
	if (!object) {
		_qmlReport(
			QString("Qml getProp() failed. Object not found: ") + strName,
			"prop",
			false
		);
		return _undefined;
	}
	
	QVariant prop = object->property(propname);
	
	if (!prop.isValid()) {
		_qmlReport(
			QString("Qml getProp() failed. Can't get: ") + QString(propname),
			"prop",
			false
		);
		return _undefined;
	}
	
	QVariantList listHelper;
	listHelper.push_back(prop);
	return std::string(
		QJsonDocument::fromVariant(listHelper)
			.toJson(QJsonDocument::Compact)
			.constData()
	);
}


std::string QmlView::invoke(
	const char *objname,
	const char *method,
	const char *json
) {
	if (!_customItem) {
		_qmlReport(
			"Qml invoke() failed. Do a proper loadQml() first.",
			"invoke",
			false
		);
		return _undefined;
	}
	
	QString strName = QString(objname);
	QQuickItem *object = _findItem(_customItem, strName);
	
	if (!object) {
		_qmlReport(
			QString("Qml invoke() failed. Object not found: ") + strName,
			"invoke",
			false
		);
		return _undefined;
	}
	
	QVariant returnValue = QVariant();
	QVariant varObject;
	varObject.setValue<QObject*>(object);
	
	bool isOk = _systemRoot->metaObject()->invokeMethod(
		_systemRoot,
		"__invoke",
		Qt::DirectConnection,
		Q_RETURN_ARG(QVariant, returnValue),
		Q_ARG(QVariant, varObject),
		Q_ARG(QVariant, QVariant(method)),
		Q_ARG(QVariant, QVariant(json))
	);
	
	if (!isOk) {
		_qmlReport(
			QString(
				"Qml invoke() failed. Invalid call to %1.%2() with %3"
			).arg(
				objname, method, json
			),
			"invoke",
			false
		);
		return _undefined;
	}
	
	if (returnValue.isNull()) {
		return _undefined;
	}
	
	QVariantList listHelper;
	listHelper.push_back(returnValue);
	return std::string(
		QJsonDocument::fromVariant(listHelper)
			.toJson(QJsonDocument::Compact)
			.constData()
	);
}


// Apply mouse event
void QmlView::mouse(int type, int button, int buttons, int x, int y) {
	QPointF mousePoint(x, y);
	
	Qt::MouseButton qbutton = static_cast<Qt::MouseButton>(Qt::NoButton);
	
	if (type != 3) {
		// Fix button
		if (button == 1) {
			qbutton = Qt::MiddleButton;
		} else if (button == 2) {
			qbutton = Qt::RightButton;
		} else {
			qbutton = static_cast<Qt::MouseButton>(1 << button);
		}
	}
	
	Qt::MouseButton qbuttons = static_cast<Qt::MouseButton>(buttons);
	Qt::KeyboardModifiers modifiers = Qt::NoModifier;
	
	if (type == 0) {
		QMouseEvent event = QMouseEvent(
			QEvent::MouseMove, mousePoint, mousePoint, Qt::NoButton, qbuttons, modifiers
		);
		QCoreApplication::sendEvent(_offscreenWindow, &event);
	} else if (type == 1) {
		QMouseEvent event = QMouseEvent(
			QEvent::MouseButtonPress,
			mousePoint, mousePoint,
			qbutton, qbuttons, modifiers
		);
		QCoreApplication::sendEvent(_offscreenWindow, &event);
	} else if (type == 2) {
		QMouseEvent event = QMouseEvent(
			QEvent::MouseButtonRelease,
			mousePoint, mousePoint,
			qbutton, qbuttons, modifiers
		);
		QCoreApplication::sendEvent(_offscreenWindow, &event);
	} else if (type == 3) {
		QWheelEvent event = QWheelEvent(
			mousePoint, mousePoint, QPoint(0, button), QPoint(0, button),
			Qt::NoButton, modifiers, Qt::ScrollPhase::NoScrollPhase, false
		);
		QCoreApplication::sendEvent(_offscreenWindow, &event);
	}
}


void QmlView::keyboard(int type, int key, unsigned text) {
	Qt::Key qtKey = static_cast<Qt::Key>(key);
	
	#define REPLACE(FROM, RESULT) case FROM: qtKey = RESULT; break;
	switch (key) {
		REPLACE(27, Qt::Key_Escape)
		REPLACE(9, Qt::Key_Tab)
		REPLACE(8, Qt::Key_Backspace)
		REPLACE(13, Qt::Key_Enter)
		REPLACE(45, Qt::Key_Insert)
		REPLACE(46, Qt::Key_Delete)
		REPLACE(36, Qt::Key_Home)
		REPLACE(35, Qt::Key_End)
		REPLACE(37, Qt::Key_Left)
		REPLACE(38, Qt::Key_Up)
		REPLACE(39, Qt::Key_Right)
		REPLACE(40, Qt::Key_Down)
		REPLACE(33, Qt::Key_PageUp)
		REPLACE(34, Qt::Key_PageDown)
		REPLACE(16, Qt::Key_Shift)
		REPLACE(17, Qt::Key_Control)
		REPLACE(18, Qt::Key_Alt)
		REPLACE(20, Qt::Key_CapsLock)
		REPLACE(144, Qt::Key_NumLock)
		REPLACE(145, Qt::Key_ScrollLock)
		REPLACE(112, Qt::Key_F1)
		REPLACE(113, Qt::Key_F2)
		REPLACE(114, Qt::Key_F3)
		REPLACE(115, Qt::Key_F4)
		REPLACE(116, Qt::Key_F5)
		REPLACE(117, Qt::Key_F6)
		REPLACE(118, Qt::Key_F7)
		REPLACE(119, Qt::Key_F8)
		REPLACE(120, Qt::Key_F9)
		REPLACE(121, Qt::Key_F10)
		REPLACE(122, Qt::Key_F11)
		REPLACE(123, Qt::Key_F12)
		REPLACE(91, Qt::Key_Super_L)
		REPLACE(93, Qt::Key_Super_R)
		default: break;
	}
	
	unsigned finalText = text;
	
	if (key == 32) { // Space
		finalText = 32;
	} else if (key == 9) { // Tab
		finalText = 9;
	}
	
	QKeyEvent keyEvent(
		type ? QEvent::KeyPress : QEvent::KeyRelease,
		qtKey,
		Qt::NoModifier,
		QString(QChar(finalText)),
		text != 0
	);
	QCoreApplication::sendEvent(_offscreenWindow, &keyEvent);
}


void QmlView::addLibsDir(const QString &dirName) {
	_qmlEngine->addImportPath(dirName);
}


QQuickItem* QmlView::_findItem(QQuickItem* node, const QString& name, int depth) const {
	if (node && node->objectName() == name) {
		return node;
	}
	
	if (node && node->children().size() > 0) {
		for (int i = node->children().size() - 1; i >= 0; i--) {
			QQuickItem *asItem = qobject_cast<QQuickItem *>(node->children().at(i));
			if (!asItem) {
				continue;
			}
			QQuickItem* item = _findItem(asItem, name, depth + 1);
			if (item) {
				return item;
			}
		}
	}
	
	// not found
	return nullptr;
}
