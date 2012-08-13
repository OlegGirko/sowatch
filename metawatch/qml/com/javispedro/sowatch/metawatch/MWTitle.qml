import Qt 4.7

Rectangle {
	width: 96
	height: 16

	property alias text: label.text
	property alias font: label.font
	property alias icon: image

	Image {
		id: image
		anchors.left: parent.left
		anchors.top: parent.top
	}

	MWLabel {
		id: label
		anchors.left: image.right
		anchors.leftMargin: 2
		anchors.verticalCenter: parent.verticalCenter
		font.pointSize: 12
	}
}
