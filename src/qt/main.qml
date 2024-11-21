import QtQuick

import "events.js" as Events


MouseArea {
	id: __mouse
	focus: true
	
	anchors.fill: parent
	
	acceptedButtons: Qt.AllButtons
	hoverEnabled: true
	
	onClicked: (mouse) => {
		focus = true;
		eventEmit('_qml_mouse', new Events.MouseEvent('click', mouse));
	}
	
	onDoubleClicked: (mouse) => {
		eventEmit('_qml_mouse', new Events.MouseEvent('dblclick', mouse));
	}
	onPositionChanged: (mouse) => {
		eventEmit('_qml_mouse', new Events.MouseMoveEvent(mouse));
	}
	onPressed: (mouse) => {
		eventEmit('_qml_mouse', new Events.MouseEvent('mousedown', mouse));
	}
	onReleased: (mouse) => {
		eventEmit('_qml_mouse', new Events.MouseEvent('mouseup', mouse));
	}
	onWheel: (wheel) => {
		eventEmit('_qml_mouse', new Events.MouseWheelEvent(wheel));
	}
	
	Keys.onPressed: (event) => {
		eventEmit('_qml_key', new Events.KeyEvent('keydown', event));
	}
	Keys.onReleased: (event) => {
		eventEmit('_qml_key', new Events.KeyEvent('keyup', event));
	}
	
	Rectangle {
		objectName: "__root"
		anchors.fill: parent
		color: "transparent"
		
		function __invoke(object, method, args) {
			if (!object[method]) {
				console.error(`There is no method "${method}".`);
				return;
			}
			
			try {
				return object[method](...JSON.parse(args));
			} catch (e) {
				console.error(e);
			}
		}
	}
	
	Rectangle {
		property alias text: errorText.text
		
		objectName: "__error"
		visible: false
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		height: errorText.height + 60
		color: "#ccffffff"
		
		Text {
			id: errorText
			anchors.top: parent.top
			anchors.left: parent.left
			anchors.right: parent.right
			wrapMode: Text.WordWrap
			color: "#ff002e"
			font.family: "Courier New"
			font.bold: true
			style: Text.Outline
			font.pointSize: 16
			styleColor: "#5c1d00"
			anchors.margins: 30
		}
	}
}
