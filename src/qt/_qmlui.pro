CONFIG(debug, debug|release): CONFIG -= release
else: CONFIG -= debug

QT += core gui qml quick
CONFIG += c++11
#CONFIG += qml_debug

TEMPLATE = lib
TARGET = qmlui

INCLUDEPATH += $$PWD/../../include


DEFINES += QMLUI_SHARED QT_DEPRECATED_WARNINGS

DESTDIR = $$PWD/../../bin-windows

unix {
	CONFIG += unversioned_libname unversioned_soname
	DESTDIR = $$PWD/../../bin-linux
	QMAKE_LFLAGS_RPATH=
}

macx {
	DESTDIR = $$PWD/../../bin-osx
}

SOURCES += \
	qml-renderer.cpp \
	qml-cb.cpp \
	keyconv.cpp \
	qml-view.cpp \
	qml-ui.cpp

HEADERS += \
	qml-renderer.hpp \
	platform.hpp \
	qml-cb.hpp \
	keyconv.hpp \
	qml-view.hpp

RESOURCES += qml.qrc
