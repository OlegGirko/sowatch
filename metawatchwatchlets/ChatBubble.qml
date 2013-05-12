import QtQuick 1.0

Item {
	id: container
	height: bubble.height + 4

	default property alias children: childContainer.children

	Image {
		anchors {
			top: parent.top; left: parent.left;
			leftMargin: 18;
		}
		source: "bubble_tip.png"
		z: 1
	}

	BorderImage {
		id: bubble
		anchors {
			top: parent.top; left: parent.left; right: parent.right;
			topMargin: 8;
		}
		border { left: 16; top: 16; right: 16; bottom: 16; }
		height: childContainer.height + 16
		source: "bubble.png"
		Item {
			id: childContainer
			height: childrenRect.height
			anchors {
				top: parent.top; left: parent.left; right: parent.right;
				margins: 16 / 2
			}
		}
	}
}

