CONFIG(debug, debug|release): CONFIG -= release
else: CONFIG -= debug

QT += core gui qml quick opengl quickcontrols2
CONFIG += c++17
#CONFIG += qml_debug

TEMPLATE = lib
TARGET = qmlui

DESTDIR = $$PWD/..

INCLUDEPATH += $$PWD/../../include

DEFINES += QMLUI_SHARED QT_DEPRECATED_WARNINGS

unix {
	CONFIG += unversioned_libname unversioned_soname
	DESTDIR = $$PWD/..
	QMAKE_LFLAGS_RPATH=
	LIBS += -L$$PWD
}

macx {
	DESTDIR = $$PWD/..
}

SOURCES += \
	qml-renderer.cpp \
	qml-cb.cpp \
	qml-view.cpp \
	qml-ui.cpp

HEADERS += \
	qml-renderer.hpp \
	platform.hpp \
	qml-cb.hpp \
	qml-view.hpp \
	../../include/qml-ui.hpp

RESOURCES += qml.qrc
