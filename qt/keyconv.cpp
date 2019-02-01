#include "keyconv.hpp"


Qt::Key keyconv(int from) {
	
	Qt::Key result = static_cast<Qt::Key>(from);
	
	if (from == 27) {
		result = Qt::Key_Escape;
	} else if (from == 9) {
		result = Qt::Key_Tab;
	} else if (from == 8) {
		result = Qt::Key_Backspace;
	} else if (from == 13) {
		result = Qt::Key_Enter;
	} else if (from == 45) {
		result = Qt::Key_Insert;
	} else if (from == 46) {
		result = Qt::Key_Delete;
	} else if (from == 17) {
		result = Qt::Key_Pause;
	} else if (from == 36 || from == 103) {
		result = Qt::Key_Home;
	} else if (from == 35 || from == 97) {
		result = Qt::Key_End;
	} else if (from == 37 || from == 100) {
		result = Qt::Key_Left;
	} else if (from == 38 || from == 104) {
		result = Qt::Key_Up;
	} else if (from == 39 || from == 102) {
		result = Qt::Key_Right;
	} else if (from == 40 || from == 98) {
		result = Qt::Key_Down;
	} else if (from == 33 || from == 105) {
		result = Qt::Key_PageUp;
	} else if (from == 34 || from == 99) {
		result = Qt::Key_PageDown;
	} else if (from == 16) {
		result = Qt::Key_Shift;
	} else if (from == 17) {
		result = Qt::Key_Control;
	} else if (from == 18) {
		result = Qt::Key_Alt;
	} else if (from == 20) {
		result = Qt::Key_CapsLock;
	} else if (from == 144) {
		result = Qt::Key_NumLock;
	} else if (from == 145) {
		result = Qt::Key_ScrollLock;
	} else if (from == 112) {
		result = Qt::Key_F1;
	} else if (from == 113) {
		result = Qt::Key_F2;
	} else if (from == 114) {
		result = Qt::Key_F3;
	} else if (from == 115) {
		result = Qt::Key_F4;
	} else if (from == 116) {
		result = Qt::Key_F5;
	} else if (from == 117) {
		result = Qt::Key_F6;
	} else if (from == 118) {
		result = Qt::Key_F7;
	} else if (from == 119) {
		result = Qt::Key_F8;
	} else if (from == 120) {
		result = Qt::Key_F9;
	} else if (from == 121) {
		result = Qt::Key_F10;
	} else if (from == 122) {
		result = Qt::Key_F11;
	} else if (from == 123) {
		result = Qt::Key_F12;
	} else if (from == 91) {
		result = Qt::Key_Super_L;
	} else if (from == 93) {
		result = Qt::Key_Super_R;
	} else if (from == 18) {
		result = Qt::Key_Menu;
	}
	
	return result;
	
}

