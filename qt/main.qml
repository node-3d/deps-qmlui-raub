import QtQuick 2.7

import "events.js" as Events


MouseArea {
	
	id : __mouse
	focus : true
	
	anchors.fill : parent
	
	acceptedButtons : Qt.AllButtons
	hoverEnabled    : true
	
	
	onClicked : {
		focus = true;
		cb.call('_qml_mouse', new Events.MouseEvent('click', mouse));
	}
	
	onDoubleClicked   : cb.call('_qml_mouse', new Events.MouseEvent('dblclick', mouse))
	onPositionChanged : cb.call('_qml_mouse', new Events.MouseMoveEvent(mouse))
	onPressed         : cb.call('_qml_mouse', new Events.MouseEvent('mousedown', mouse))
	onReleased        : cb.call('_qml_mouse', new Events.MouseEvent('mouseup', mouse))
	onWheel           : cb.call('_qml_mouse', new Events.MouseWheelEvent(wheel))
	
	
	Keys.onPressed  : cb.call('_qml_key', new Events.KeyEvent('keydown', event))
	Keys.onReleased : cb.call('_qml_key', new Events.KeyEvent('keyup', event))
	
	
	Rectangle {
		objectName   : "__root"
		anchors.fill : parent
		color        : "transparent"
	}
	
}
