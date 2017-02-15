#include <QCoreApplication>
#include <QGuiApplication>
#include <QDebug>

#include "qml-renderer.hpp"
#include "qmlui.hpp"
#include "platform.hpp"

// Main static objects
QGuiApplication *app = nullptr;
QmlRenderer *renderer = nullptr;


// -- C-API -- //


void qmlui_init(const char *cwdOwn, const char *cwdLib, int wnd, int ctx, int w, int h, EventCb cb)
{
	
	if (renderer) {
		qDebug() << "qmlui_init: already inited.";
		return;
	}
	
	QString cwdOwnStr = QString(cwdOwn);
	QString cwdLibStr = QString(cwdLib);
//	qputenv("path", (cwdLibStr + "/" + PLATFORM_BIN_DIR).toLatin1() + ";" + qgetenv("path"));
//	QCoreApplication::addLibraryPath(cwdLibStr + "/plugins");
	qDebug() << "LIB" << QCoreApplication::libraryPaths();
	
	int c = 0;
	char* v = nullptr;
	app = new QGuiApplication(c, &v);
	
	renderer = new QmlRenderer(cwdOwnStr, cwdLibStr, wnd, ctx, w, h, cb);
	renderer->confirm();
	
}


void qmlui_resize(int w, int h) {
	if ( renderer == nullptr ) {
		qDebug() << "qmlui_resize: not inited.";
		return;
	}
	renderer->resize(QSize(w, h));
}


void qmlui_mouse(int type, int button, int buttons, int x, int y) {
	if ( renderer == nullptr ) {
		qDebug() << "qmlui_mouse: not inited.";
		return;
	}
	renderer->mouse(type, button, buttons, x, y);
}


void qmlui_keyboard(int type, int key, char text) {
	if ( renderer == nullptr ) {
		qDebug() << "qmlui_keyboard: not inited.";
		return;
	}
//	qDebug() << "qmlui_keyboard" << type << key << text;
	renderer->keyboard(type, key, text);
}


void qmlui_use(const char *str, bool isFile)
{
	
	if ( renderer == nullptr ) {
		qDebug() << "qmlui_use: not inited.";
		return;
	}
	if ( ! renderer->isReady() ) {
		qDebug() << "qmlui_use: not ready.";
		return;
	}
	
	if (isFile) {
		renderer->useQml(QString(str));
	} else {
		renderer->useText(QString(str));
	}
	
}

void qmlui_set(const char *obj, const char *prop, const char *json)
{
	
	if ( renderer == nullptr ) {
		qDebug() << "qmlui_set: not inited.";
		return;
	}
	if ( ! renderer->isReady() ) {
		qDebug() << "qmlui_set: not ready.";
		return;
	}
	
	renderer->setProp(QString(obj), QByteArray(prop), QByteArray(json));
	
}

void qmlui_get(const char *obj, const char *prop)
{
	
	if ( renderer == nullptr ) {
		qDebug() << "qmlui_get: not inited.";
		return;
	}
	if ( ! renderer->isReady() ) {
		qDebug() << "qmlui_get: not ready.";
		return;
	}
	
	renderer->getProp(QString(obj), QByteArray(prop));
	
}


void qmlui_invoke(const char *obj, const char *method, const char *json)
{
	
	if ( renderer == nullptr ) {
		qDebug() << "qmlui_invoke: not inited.";
		return;
	}
	if ( ! renderer->isReady() ) {
		qDebug() << "qmlui_invoke: not ready.";
		return;
	}
	
	renderer->invoke(QString(obj), QByteArray(method), QByteArray(json));
	
}


void qmlui_libs(const char *libs)
{

	if ( renderer == nullptr ) {
		qDebug() << "qmlui_libs: not inited.";
		return;
	}
	
	renderer->libs(QString(libs));
	
}

void qmlui_plugins(const char *plugins)
{
	
	QCoreApplication::addLibraryPath(QString(plugins));
	
}
