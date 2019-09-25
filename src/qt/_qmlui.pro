CONFIG(debug, debug|release): CONFIG -= release
else: CONFIG -= debug

QT += core gui qml quick opengl
CONFIG += c++11
#CONFIG += qml_debug

TEMPLATE = lib
TARGET = qmlui

DESTDIR = $$PWD/../../bin-windows

INCLUDEPATH += $$PWD/../../include

DEFINES += QMLUI_SHARED QT_DEPRECATED_WARNINGS

unix {
	CONFIG += unversioned_libname unversioned_soname
	DESTDIR = $$PWD/../../bin-linux
	QMAKE_LFLAGS_RPATH=
	LIBS += -L$$PWD
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
