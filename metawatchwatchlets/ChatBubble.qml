import QtQuick 1.0

Item {
	id: container
	height: tip.height + bubble.height

	default property alias children: childContainer.children

	Image {
		id: tip
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
			topMargin: 9;
		}
		border { left: 14; top: 14; right: 14; bottom: 14; }
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

