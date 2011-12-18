import Qt 4.7

Rectangle {
	width: 96
	height: 16

	property alias text: label.text

	Row {
		anchors.fill: parent
		anchors.margins: 1
		Text {
			id: label
			font.bold: true
		}
	}
}
