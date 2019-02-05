CONFIG(debug, debug|release): CONFIG -= release
else: CONFIG -= debug

QT += core gui qml quick
CONFIG += c++11
#CONFIG += qml_debug

TEMPLATE = lib
TARGET = qmlui

INCLUDEPATH += $$PWD/../include
LIBS += -L$$PWD

DEFINES += QMLUI_SHARED QT_DEPRECATED_WARNINGS

DESTDIR = $$PWD/../bin-win32
contains(QMAKE_TARGET.arch, x86_64):{
	DESTDIR = $$PWD/../bin-win64
}

unix {
	CONFIG += unversioned_libname unversioned_soname
	DESTDIR = $$PWD/../bin-linux64
	QMAKE_LFLAGS_RPATH=
	QMAKE_LFLAGS += "-Wl,-rpath,\'\'"
}

macx {
	DESTDIR = $$PWD/../bin-mac64
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
