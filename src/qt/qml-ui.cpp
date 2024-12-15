#include <QGuiApplication>
#include <QQuickStyle>

#include <qml-ui.hpp>

#include "qml-renderer.hpp"
#include "qml-cb.hpp"
#include "qml-view.hpp"


// Main static objects
static int argc = 1;
static char argvData[] = "qmlui";
static char *argv = &argvData[0];

static QGuiApplication *app  = nullptr;
static QmlRenderer *renderer = nullptr;


void QmlUi::init(const char *cwdOwn, size_t wnd, size_t ctx, QmlUi::Cb cb) {
	QmlUi::init2(cwdOwn, wnd, ctx, 0, cb);
}

void QmlUi::init2(const char *cwdOwn, size_t wnd, size_t ctx, size_t device, QmlUi::Cb cb) {
	if (renderer) {
		return;
	}
	
	QmlCb::init(cb);
	
	QGuiApplication::setAttribute(Qt::AA_PluginApplication);
	QGuiApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	QGuiApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
	QGuiApplication::setAttribute(Qt::AA_DontCheckOpenGLContextThreadAffinity);
	app = new QGuiApplication(argc, &argv);
	
	renderer = new QmlRenderer(QString(cwdOwn), wnd, ctx, device);
}


void QmlUi::plugins(const char *plugins) {
	QCoreApplication::addLibraryPath(QString(plugins));
}


void QmlUi::update() {
	QCoreApplication::processEvents();
}


void QmlUi::style(const char *name, const char *fallback) {
	QQuickStyle::setStyle(QString(name));
	if (fallback) {
		QQuickStyle::setFallbackStyle(QString(fallback));
	}
}


QmlUi::QmlUi(int w, int h) {
	if ( renderer == nullptr ) {
		throw "QmlUi: not inited.";
	}
	// Wrap JS event callback
	_qmlCb = new QmlCb(this);
	_view = new QmlView(renderer, w, h, _qmlCb);
}


QmlUi::~QmlUi() {
	delete _view;
	delete _qmlCb;
	_view = nullptr;
	_qmlCb = nullptr;
}


void QmlUi::resize(int w, int h) {
	_view->resize(QSize(w, h));
}


void QmlUi::mouse(int type, int button, int buttons, int x, int y) {
	_view->mouse(type, button, buttons, x, y);
}


void QmlUi::keyboard(int type, int key, unsigned text) {
	_view->keyboard(type, key, text);
}


void QmlUi::load(const char *str, bool isFile) {
	if (isFile) {
		_view->loadQml(QString(str));
	} else {
		_view->loadText(QString(str));
	}
}


void QmlUi::set(const char *obj, const char *prop, const char *json) {
	_view->setProp(obj, prop, json);
}


std::string QmlUi::get(const char *obj, const char *prop) {
	return _view->getProp(obj, prop);
}


std::string QmlUi::invoke(const char *obj, const char *method, const char *json) {
	return _view->invoke(obj, method, json);
}


void QmlUi::libs(const char *libs) {
	_view->addLibsDir(QString(libs));
}
