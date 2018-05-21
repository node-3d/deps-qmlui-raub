'use strict';

/* global Qt */


var MOUSE_BUTTONS = {};
MOUSE_BUTTONS[Qt.LeftButton] = 1;
MOUSE_BUTTONS[Qt.RightButton] = 2;
MOUSE_BUTTONS[Qt.MiddleButton] = 4;


function getJsMouseButton(buttons, QtMouseButton) {
	return (buttons & QtMouseButton) && MOUSE_BUTTONS[QtMouseButton];
}

function combineJsMouseButtons(buttons) {
	
	return (
		getJsMouseButton(buttons, Qt.LeftButton) |
		getJsMouseButton(buttons, Qt.RightButton) |
		getJsMouseButton(buttons, Qt.MiddleButton)
	);
	
}


function MouseEvent(type, mouse) {
	
	this.type = type;
	
	this.button = MOUSE_BUTTONS[mouse.button];
	this.buttons = combineJsMouseButtons(mouse.buttons);
	
	this.x = mouse.x;
	this.y = mouse.y;
	this.clientX = mouse.x;
	this.clientY = mouse.y;
	this.offsetX = mouse.x;
	this.offsetY = mouse.y;
	this.pageX = mouse.x;
	this.pageY = mouse.y;
	this.screenX = mouse.x;
	this.screenY = mouse.y;
	
	this.region = null;
	this.relatedTarget = null;
	
	this.altKey = mouse.modifiers & Qt.AltModifier;
	this.ctrlKey = mouse.modifiers & Qt.ControlModifier;
	this.metaKey = mouse.modifiers & Qt.MetaModifier;
	this.shiftKey = mouse.modifiers & Qt.ShiftModifier;
	
}


function MouseMoveEvent(mouse) {
	
	MouseEvent.call(this, 'mousemove', mouse);
	
	this.movementX = MouseMoveEvent._prevX !== null ? mouse.x - MouseMoveEvent._prevX : 0;
	this.movementY = MouseMoveEvent._prevY !== null ? mouse.x - MouseMoveEvent._prevY : 0;
	
	MouseMoveEvent._prevX = mouse.x;
	MouseMoveEvent._prevY = mouse.y;
	
}

MouseMoveEvent._prevX = null;
MouseMoveEvent._prevY = null;


function normalizeDelta(dv) {
	return dv > 0 ? 1 : (dv < 0 ? -1 : 0);
}

function MouseWheelEvent(mouse) { // eslint-disable-line no-unused-vars
	
	MouseEvent.call(this, 'wheel', mouse);
	
	this.deltaX = normalizeDelta(mouse.angleDelta.x);
	this.deltaY = normalizeDelta(mouse.angleDelta.y);
	this.deltaZ = 0;
	
	this.deltaMode = 1;
	
}


function KeyEvent(type, event) { // eslint-disable-line no-unused-vars
	
	this.type = type;
	
	this.location = event.key;
	this.keyCode = event.key;
	this.which = event.key;
	
	this.code = event.key;
	this.key = event.text[0] || '';
	
	this.repeat = event.isAutoRepeat;
	this.isComposing = false;
	this.composed = true;
	this.locale = '';
	
	this.altKey = event.modifiers & Qt.AltModifier;
	this.ctrlKey = event.modifiers & Qt.ControlModifier;
	this.metaKey = event.modifiers & Qt.MetaModifier;
	this.shiftKey = event.modifiers & Qt.ShiftModifier;
	
}
