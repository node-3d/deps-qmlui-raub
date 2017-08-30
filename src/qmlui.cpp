#include <QCoreApplication>
#include <QGuiApplication>
#include <QDebug>
#include <QVector>

#include "qml-renderer.hpp"
#include "qml-window.hpp"
#include "qmlui.hpp"
#include "platform.hpp"

// Main static objects
QGuiApplication *app = nullptr;
QmlRenderer *renderer = nullptr;

QVector<QmlWindow *> windowList;

inline bool isOpen(int i, char *name) {
	
	if ( renderer == nullptr ) {
		qDebug() << name << ": not inited.";
		return false;
	}
	
	if (windowList.size() > i) {
		return windowList[i] != nullptr;
	} else {
		qDebug() << name << i << ": no window.";
		return false;
	}
	
}

// -- C-API -- //

void qmlui_init(const char *cwdOwn, size_t wnd, size_t ctx, int w, int h, EventCb cb) {
	
	if (renderer) {
		qDebug() << "qmlui_init: already inited.";
		return;
	}
	
	QString cwdOwnStr = QString(cwdOwn);
	
	int c = 0;
	char* v = nullptr;
	app = new QGuiApplication(c, &v);
	
	renderer = new QmlRenderer(cwdOwnStr, wnd, ctx);
	
}


void qmlui_window(int *i, int w, int h, EventCb cb)
{
	
	if ( renderer == nullptr ) {
		qDebug() << "qmlui_window: not inited.";
		*i = -1;
		return;
	}
	
	QmlWindow *window = new QmlWindow(renderer, w, h, cb);
	window->confirm();
	
	*i = windowList.size();
	windowList.push(window);

}


void qmlui_close(int i) {
	
	if ( ! isOpen(i, "qmlui_close") ) {
		return;
	}
	
	delete windowList[i];
	windowList[i] = nullptr;
	
}


void qmlui_resize(int i, int w, int h) {
	
	if ( ! isOpen(i, "qmlui_resize") ) {
		return;
	}
	
	windowList[i]->resize(QSize(w, h));
	
}


void qmlui_mouse(int type, int button, int buttons, int x, int y) {
	
	if ( ! isOpen(i, "qmlui_mouse") ) {
		return;
	}
	
	windowList[i]->mouse(type, button, buttons, x, y);
	
}


void qmlui_keyboard(int type, int key, char text) {
	
	if ( ! isOpen(i, "qmlui_keyboard") ) {
		return;
	}
	
	windowList[i]->keyboard(type, key, text);
	
}


void qmlui_use(const char *str, bool isFile)
{
	
	if ( ! isOpen(i, "qmlui_use") ) {
		return;
	}
	
	if (isFile) {
		windowList[i]->useQml(QString(str));
	} else {
		windowList[i]->useText(QString(str));
	}
	
}

void qmlui_set(const char *obj, const char *prop, const char *json)
{
	
	if ( ! isOpen(i, "qmlui_set") ) {
		return;
	}
	
	windowList[i]->setProp(QString(obj), QByteArray(prop), QByteArray(json));
	
}

void qmlui_get(const char *obj, const char *prop)
{
	
	if ( ! isOpen(i, "qmlui_get") ) {
		return;
	}
	
	windowList[i]->getProp(QString(obj), QByteArray(prop));
	
}


void qmlui_invoke(const char *obj, const char *method, const char *json)
{
	
	if ( ! isOpen(i, "qmlui_get") ) {
		return;
	}
	
	windowList[i]->invoke(QString(obj), QByteArray(method), QByteArray(json));
	
}


void qmlui_libs(const char *libs)
{
	
	if ( ! isOpen(i, "qmlui_libs") ) {
		return;
	}
	FOR
	windowList[i]->libs(QString(libs));
	
}

void qmlui_plugins(const char *plugins)
{
	
	QCoreApplication::addLibraryPath(QString(plugins));
	
}
