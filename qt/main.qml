import QtQuick 2.7

import "events.js" as Events


MouseArea {
	
	id : __mouse
	
	anchors.fill : parent
	
	acceptedButtons : Qt.AllButtons
	hoverEnabled    : true
	
	
	onClicked         : cb.call('_mouse', new Events.MouseEvent('click', mouse))
	onDoubleClicked   : cb.call('_mouse', new Events.MouseEvent('dblclick', mouse))
	onPositionChanged : cb.call('_mouse', new Events.MouseMoveEvent(mouse))
	onPressed         : cb.call('_mouse', new Events.MouseEvent('mousedown', mouse))
	onReleased        : cb.call('_mouse', new Events.MouseEvent('mouseup', mouse))
	onWheel           : cb.call('_mouse', new Events.MouseWheelEvent(mouse))
	
	
	Keys.onPressed  : cb.call('_key', new Events.KeyEvent('keydown', event))
	Keys.onReleased : cb.call('_key', new Events.KeyEvent('keyup', event))
	
	
	Rectangle {
		objectName   : "__root"
		anchors.fill : parent
		color        : "transparent"
	}
	
}
