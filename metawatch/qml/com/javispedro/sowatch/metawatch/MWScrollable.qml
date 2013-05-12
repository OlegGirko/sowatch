import Qt 4.7

Flickable {
	id: flickable

	property bool selectable: true
	property bool indicator: true

	interactive: false
	boundsBehavior: Flickable.StopAtBounds
	flickableDirection: Flickable.VerticalFlick

	contentWidth: contentItem.childrenRect.width
	contentHeight: contentItem.childrenRect.height

	function scrollDown() {
		console.log(contentHeight + " " + height);
		console.log(childrenRect.height);
		var maxY = Math.max(0, contentHeight - height);
		var newContentY = contentY + 96/3;
		console.log(maxY + " " + newContentY);

		if (newContentY > maxY) {
			contentY = maxY; // Never overscroll.
		} else {
			contentY = newContentY;
		}
	}

	function scrollUp() {
		var newContentY = contentY - 96/3;

		if (newContentY < 0) {
			contentY = 0; // Never overscroll.
		} else {
			contentY = newContentY;
		}
	}

	function scrollTop() {
		contentY = 0;
	}

	children: Rectangle {
		id: indicatorCont
		visible: flickable.indicator && (flickable.contentHeight > flickable.height)
		anchors.top: parent.top
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		width: 4

		color: "white"

		Rectangle {
			id: indicatorRect

			property int minHeight: 10

			anchors.right: parent.right
			anchors.left: parent.left
			anchors.leftMargin: 1

			y: flickable.visibleArea.yPosition * indicatorCont.height
			height: Math.max(minHeight, flickable.visibleArea.heightRatio * indicatorCont.height)

			color: "black"
		}
	}
}
