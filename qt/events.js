'use strict';

/* global Qt MouseMoveEvent MouseWheelEvent KeyEvent */


var MOUSE_BUTTONS = {};
MOUSE_BUTTONS[Qt.LeftButton] = 0;
MOUSE_BUTTONS[Qt.RightButton] = 2;
MOUSE_BUTTONS[Qt.MiddleButton] = 1;

var KEY_CODES = {};
KEY_CODES[Qt.Key_Alt] = 18;
KEY_CODES[Qt.Key_Backspace] = 8;
KEY_CODES[Qt.Key_CapsLock] = 20;
KEY_CODES[Qt.Key_Control] = 17;
KEY_CODES[Qt.Key_Delete] = 46;
KEY_CODES[Qt.Key_Down] = 40;
KEY_CODES[Qt.Key_End] = 35;
KEY_CODES[Qt.Key_Enter] = 13;
KEY_CODES[Qt.Key_Escape] = 27;
KEY_CODES[Qt.Key_F1] = 112;
KEY_CODES[Qt.Key_F10] = 121;
KEY_CODES[Qt.Key_F11] = 122;
KEY_CODES[Qt.Key_F12] = 123;
KEY_CODES[Qt.Key_F2] = 113;
KEY_CODES[Qt.Key_F3] = 114;
KEY_CODES[Qt.Key_F4] = 115;
KEY_CODES[Qt.Key_F5] = 116;
KEY_CODES[Qt.Key_F6] = 117;
KEY_CODES[Qt.Key_F7] = 118;
KEY_CODES[Qt.Key_F8] = 119;
KEY_CODES[Qt.Key_F9] = 120;
KEY_CODES[Qt.Key_Home] = 36;
KEY_CODES[Qt.Key_Insert] = 45;
KEY_CODES[Qt.Key_Left] = 37;
KEY_CODES[Qt.Key_NumLock] = 144;
KEY_CODES[Qt.Key_PageDown] = 34;
KEY_CODES[Qt.Key_PageUp] = 33;
KEY_CODES[Qt.Key_Right] = 39;
KEY_CODES[Qt.Key_ScrollLock] = 145;
KEY_CODES[Qt.Key_Shift] = 16;
KEY_CODES[Qt.Key_Super_L] = 91;
KEY_CODES[Qt.Key_Super_R] = 93;
KEY_CODES[Qt.Key_Tab] = 9;
KEY_CODES[Qt.Key_Up] = 38;


var KEY_NAMES = {};
KEY_NAMES[Qt.Key_Alt] = 'Alt';
KEY_NAMES[Qt.Key_Backspace] = 'Backspace';
KEY_NAMES[Qt.Key_CapsLock] = 'CapsLock';
KEY_NAMES[Qt.Key_Control] = 'Control';
KEY_NAMES[Qt.Key_Delete] = 'Delete';
KEY_NAMES[Qt.Key_Down] = 'Down';
KEY_NAMES[Qt.Key_End] = 'End';
KEY_NAMES[Qt.Key_Enter] = 'Enter';
KEY_NAMES[Qt.Key_Escape] = 'Escape';
KEY_NAMES[Qt.Key_F1] = 'F1';
KEY_NAMES[Qt.Key_F10] = 'F10';
KEY_NAMES[Qt.Key_F11] = 'F11';
KEY_NAMES[Qt.Key_F12] = 'F12';
KEY_NAMES[Qt.Key_F2] = 'F2';
KEY_NAMES[Qt.Key_F3] = 'F3';
KEY_NAMES[Qt.Key_F4] = 'F4';
KEY_NAMES[Qt.Key_F5] = 'F5';
KEY_NAMES[Qt.Key_F6] = 'F6';
KEY_NAMES[Qt.Key_F7] = 'F7';
KEY_NAMES[Qt.Key_F8] = 'F8';
KEY_NAMES[Qt.Key_F9] = 'F9';
KEY_NAMES[Qt.Key_Home] = 'Home';
KEY_NAMES[Qt.Key_Insert] = 'Insert';
KEY_NAMES[Qt.Key_Left] = 'Left';
KEY_NAMES[Qt.Key_NumLock] = 'NumLock';
KEY_NAMES[Qt.Key_PageDown] = 'PageDown';
KEY_NAMES[Qt.Key_PageUp] = 'PageUp';
KEY_NAMES[Qt.Key_Right] = 'Right';
KEY_NAMES[Qt.Key_ScrollLock] = 'ScrollLock';
KEY_NAMES[Qt.Key_Shift] = 'Shift';
KEY_NAMES[Qt.Key_Super_L] = 'Super_L';
KEY_NAMES[Qt.Key_Super_R] = 'Super_R';
KEY_NAMES[Qt.Key_Tab] = 'Tab';
KEY_NAMES[Qt.Key_Up] = 'Up';


function MouseEvent(type, mouse) {
	
	this.type = type;
	
	this.button = MOUSE_BUTTONS[mouse.button] || 0;
	this.buttons = mouse.buttons;
	
	this.x = mouse.x;
	this.y = mouse.y;
	this.clientX = mouse.x;
	this.clientY = mouse.y;
	this.pageX = mouse.x;
	this.pageY = mouse.y;
	
	this.altKey = (mouse.modifiers & Qt.AltModifier) > 0;
	this.ctrlKey = (mouse.modifiers & Qt.ControlModifier) > 0;
	this.metaKey = (mouse.modifiers & Qt.MetaModifier) > 0;
	this.shiftKey = (mouse.modifiers & Qt.ShiftModifier) > 0;
	
}


var prevX = 0;
var prevY = 0;

function MouseMoveEvent(mouse) {
	
	MouseEvent.call(this, 'mousemove', mouse);
	
	this.movementX = mouse.x - prevX;
	this.movementY = mouse.x - prevY;
	
	prevX = mouse.x;
	prevY = mouse.y;
	
}


function MouseWheelEvent(mouse) {
	
	MouseEvent.call(this, 'wheel', mouse);
	
	this.wheelDeltaX = mouse.angleDelta.x > 0 ? 120 : -120;
	this.wheelDeltaY = mouse.angleDelta.y > 0 ? 120 : -120;
	this.wheelDelta = this.wheelDeltaY;
	this.deltaX = mouse.angleDelta.x > 0 ? 100 : -100;
	this.deltaY = mouse.angleDelta.y > 0 ? 100 : -100;
	this.deltaZ = 0;
	
}


function KeyEvent(type, event) {
	
	this.type = type;
	
	this.which = KEY_CODES[event.key] || event.key;
	this.keyCode = this.which;
	
	this.charCode = event.text.charCodeAt(0);
	
	this.key = event.text || KEY_NAMES[event.key] || ' ';
	event.code = (
		KEY_NAMES[event.key] ||
		(event.text && ('Key' + event.text)) ||
		'UNKNOWN'
	);
	
	this.repeat = event.isAutoRepeat;
	
	this.altKey = (event.modifiers & Qt.AltModifier) > 0;
	this.ctrlKey = (event.modifiers & Qt.ControlModifier) > 0;
	this.metaKey = (event.modifiers & Qt.MetaModifier) > 0;
	this.shiftKey = (event.modifiers & Qt.ShiftModifier) > 0;
	
}
