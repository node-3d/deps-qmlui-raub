CONFIG(debug, debug|release): CONFIG -= release
else: CONFIG -= debug

QT += core gui qml quick
CONFIG += c++11
#CONFIG += qml_debug

TEMPLATE = lib
TARGET = qmlui
DEFINES += QMLUI_SHARED QT_DEPRECATED_WARNINGS


DESTDIR = $$PWD/../bin-win32
contains(QMAKE_TARGET.arch, x86_64):{
	DESTDIR = $$PWD/../bin-win64
}

unix {
	DESTDIR = $$PWD/../bin-linux32
	contains(QMAKE_TARGET.arch, x86_64):{
		DESTDIR = $$PWD/../bin-linux64
	}
}

macx {
	DESTDIR = $$PWD/../bin-mac64
}


SOURCES += \
	qmlui.cpp \
	qml-renderer.cpp \
	qml-cb.cpp \
	keyconv.cpp \
	qml-window.cpp

HEADERS += qmlui.hpp \
	qml-renderer.hpp \
	platform.hpp \
	qml-cb.hpp \
	keyconv.hpp \
	qml-window.hpp

RESOURCES += qml.qrc
