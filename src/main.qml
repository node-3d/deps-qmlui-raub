import QtQuick 2.7


MouseArea {

	id: __mouse
	anchors.fill: parent
	
	function eventData(action, mouse) {
		return {
			action   : action,
			accepted : mouse.accepted,
			button   : mouse.button,
			buttons  : mouse.buttons,
			modifiers: mouse.modifiers,
			wasHeld  : mouse.wasHeld,
			x        : mouse.x,
			y        : mouse.y,
		};
	}
	
	onClicked        : cb.call('mouse', eventData('click', mouse))
	onDoubleClicked  : cb.call('mouse', eventData('double', mouse))
	onPositionChanged: cb.call('mouse', eventData('move', mouse))
	onPressed        : cb.call('mouse', eventData('press', mouse))
	onReleased       : cb.call('mouse', eventData('release', mouse))
	onWheel          : cb.call('mouse', eventData('wheel', mouse))
	
	Rectangle {
		objectName: "__root"
		anchors.fill: parent
		color: "transparent"
	}
	
}
