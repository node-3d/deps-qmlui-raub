CONFIG(debug, debug|release): CONFIG -= release
else: CONFIG -= debug

QT += core gui qml quick
CONFIG += c++11
#CONFIG += qml_debug

TEMPLATE = lib
TARGET = qmlui
DEFINES += QMLUI_SHARED QT_DEPRECATED_WARNINGS


DESTDIR = $$PWD/../bin_win32
contains(QMAKE_TARGET.arch, x86_64):{
	DESTDIR = $$PWD/../bin_win64
}

macx {
	DESTDIR = $$PWD/../bin_mac64
}

linux-g++ {
	DESTDIR = $$PWD/../bin_linux32
	contains(QMAKE_TARGET.arch, x86_64):{
		DESTDIR = $$PWD/../bin_linux64
	}
}


WPWD = $${PWD}

WPWD ~= s,/,\\,g
QMAKE_POST_LINK += $$quote(cmd /c copy /y $$WPWD\\qmlui.hpp $$WPWD\\..\\include)

macx,linux-g++ {
	QMAKE_POST_LINK += $$quote(cp $$PWD/qmlui.hpp $$PWD/../include)
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
