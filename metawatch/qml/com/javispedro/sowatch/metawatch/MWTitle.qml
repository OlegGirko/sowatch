import QtQuick 1.0

Rectangle {
	width: 96
	height: 16

	property alias text: label.text

	Row {
		anchors.fill: parent
		Text {
			id: label
			font.bold: true
		}
	}
}
