#include "keyconv.hpp"


#define REPLACE(FROM, RESULT) case FROM: result = RESULT; break;


Qt::Key keyconv(int from) {
	
	Qt::Key result = static_cast<Qt::Key>(from);
	
	switch (from) {
		REPLACE(27, Qt::Key_Escape)
		REPLACE(9, Qt::Key_Tab)
		REPLACE(8, Qt::Key_Backspace)
		REPLACE(13, Qt::Key_Enter)
		REPLACE(45, Qt::Key_Insert)
		REPLACE(46, Qt::Key_Delete)
		REPLACE(36, Qt::Key_Home)
		REPLACE(35, Qt::Key_End)
		REPLACE(37, Qt::Key_Left)
		REPLACE(38, Qt::Key_Up)
		REPLACE(39, Qt::Key_Right)
		REPLACE(40, Qt::Key_Down)
		REPLACE(33, Qt::Key_PageUp)
		REPLACE(34, Qt::Key_PageDown)
		REPLACE(16, Qt::Key_Shift)
		REPLACE(17, Qt::Key_Control)
		REPLACE(18, Qt::Key_Alt)
		REPLACE(20, Qt::Key_CapsLock)
		REPLACE(144, Qt::Key_NumLock)
		REPLACE(145, Qt::Key_ScrollLock)
		REPLACE(112, Qt::Key_F1)
		REPLACE(113, Qt::Key_F2)
		REPLACE(114, Qt::Key_F3)
		REPLACE(115, Qt::Key_F4)
		REPLACE(116, Qt::Key_F5)
		REPLACE(117, Qt::Key_F6)
		REPLACE(118, Qt::Key_F7)
		REPLACE(119, Qt::Key_F8)
		REPLACE(120, Qt::Key_F9)
		REPLACE(121, Qt::Key_F10)
		REPLACE(122, Qt::Key_F11)
		REPLACE(123, Qt::Key_F12)
		REPLACE(91, Qt::Key_Super_L)
		REPLACE(93, Qt::Key_Super_R)
		default: break;
	}
	
	return result;
	
}

