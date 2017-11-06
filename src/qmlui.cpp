#include <QCoreApplication>
#include <QGuiApplication>
#include <QDebug>
#include <QVector>

#include "qml-renderer.hpp"
#include "qml-window.hpp"
#include "qmlui.hpp"
#include "platform.hpp"

// Main static objects
int argc = 1;
char argvData[] = "qmlui";
char *argv = &argvData[0];

QGuiApplication *app  = nullptr;
QmlRenderer *renderer = nullptr;

QVector<QmlWindow *> windowList;

inline bool isOpen(int i, const char *name) {
	
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

void qmlui_init(const char *cwdOwn, size_t wnd, size_t ctx) {
	
	if (renderer) {
		qDebug() << "qmlui_init: already inited.";
		return;
	}
	
	app = new QGuiApplication(argc, &argv);
	
	renderer = new QmlRenderer(QString(cwdOwn), wnd, ctx);
	
}


void qmlui_view(volatile int *i, int w, int h, EventCb cb) {
	
	if ( renderer == nullptr ) {
		qDebug() << "qmlui_window: not inited.";
		*i = -1;
		return;
	}
	
	QmlWindow *window = new QmlWindow(renderer, w, h, cb);
	window->confirm();
	
	*i = windowList.size();
	windowList.push_back(window);

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


void qmlui_mouse(int i, int type, int button, int buttons, int x, int y) {
	
	if ( ! isOpen(i, "qmlui_mouse") ) {
		return;
	}
	
	windowList[i]->mouse(type, button, buttons, x, y);
	
}


void qmlui_keyboard(int i, int type, int key, char text) {
	
	if ( ! isOpen(i, "qmlui_keyboard") ) {
		return;
	}
	
	windowList[i]->keyboard(type, key, text);
	
}


void qmlui_load(int i, const char *str, bool isFile)
{
	
	if ( ! isOpen(i, "qmlui_load") ) {
		return;
	}
	
	if (isFile) {
		windowList[i]->loadQml(QString(str));
	} else {
		windowList[i]->loadText(QString(str));
	}
	
}

void qmlui_set(int i, const char *obj, const char *prop, const char *json)
{
	
	if ( ! isOpen(i, "qmlui_set") ) {
		return;
	}
	
	windowList[i]->setProp(QString(obj), QByteArray(prop), QByteArray(json));
	
}

void qmlui_get(int i, const char *obj, const char *prop)
{
	
	if ( ! isOpen(i, "qmlui_get") ) {
		return;
	}
	
	windowList[i]->getProp(QString(obj), QByteArray(prop));
	
}


void qmlui_invoke(int i, const char *obj, const char *method, const char *json)
{
	
	if ( ! isOpen(i, "qmlui_get") ) {
		return;
	}
	
	windowList[i]->invoke(QString(obj), QByteArray(method), QByteArray(json));
	
}


void qmlui_libs(int i, const char *libs)
{
	
	if ( ! isOpen(i, "qmlui_libs") ) {
		return;
	}
	
	windowList[i]->addLibsDir(QString(libs));
	
}

void qmlui_plugins(const char *plugins)
{
	
	QCoreApplication::addLibraryPath(QString(plugins));
	
}
