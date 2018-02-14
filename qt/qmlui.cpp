#include <QCoreApplication>
#include <QGuiApplication>
#include <QDebug>
#include <QVector>

#include "qml-renderer.hpp"
#include "qml-view.hpp"
#include "qmlui.hpp"
#include "platform.hpp"

// Main static objects
int argc = 1;
char argvData[] = "qmlui";
char *argv = &argvData[0];

QGuiApplication *app  = nullptr;
QmlRenderer *renderer = nullptr;

QVector<QmlWindow *> windowList;


static void QmlUi::init(const char *cwdOwn, size_t wnd, size_t ctx) {
	
	if (renderer) {
		return;
	}
	
	app = new QGuiApplication(argc, &argv);
	
	renderer = new QmlRenderer(QString(cwdOwn), wnd, ctx);
	
}


static void QmlUi::plugins(const char *plugins) {
	
	QCoreApplication::addLibraryPath(QString(plugins));
	
}


void QmlUi::QmlUi(int w, int h, QmlUi::Cb cb) {
	
	if ( renderer == nullptr ) {
		throw "QmlUi: not inited.";
	}
	
	_view = new QmlView(renderer, w, h, *i, cb);
	_view->confirm();
	
}


void QmlUi::~QmlUi() {
	
	delete _view;
	_view = nullptr;
	
}


void QmlUi::resize(int w, int h) {
	
	_view->resize(QSize(w, h));
	
}


void QmlUi::mouse(int type, int button, int buttons, int x, int y) {
	
	_view->mouse(type, button, buttons, x, y);
	
}


void QmlUi::keyboard(int type, int key, char text) {
	
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
	
	_view->setProp(QString(obj), QByteArray(prop), QByteArray(json));
	
}


void QmlUi::get(const char *obj, const char *prop) {
	
	_view->getProp(QString(obj), QByteArray(prop));
	
}


void QmlUi::invoke(const char *obj, const char *method, const char *json) {
	
	_view->invoke(QString(obj), QByteArray(method), QByteArray(json));
	
}


void QmlUi::libs(const char *libs) {
	
	_view->addLibsDir(QString(libs));
	
}
