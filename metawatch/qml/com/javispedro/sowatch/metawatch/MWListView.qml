import Qt 4.7

ListView {
	id: list

	property bool selectable: true
	property bool indicator: true

	interactive: false
	highlightFollowsCurrentItem: false
	keyNavigationWraps: false
	boundsBehavior: Flickable.StopAtBounds
	flickableDirection: Flickable.VerticalFlick

	property real currentItemTop: currentItem !== null ? currentItem.y - contentY : 0
	property real currentItemBottom: currentItem !== null ? currentItemTop + currentItem.height : 0

	function scrollDown() {
		if (count == 0) {
			return; // No items
		}
		if (selectable) {
			if (currentIndex === -1) {
				// If no item is selected, select the first one.
				currentItem = 0;
				return;
			}
			if (currentIndex < count && currentItemBottom < height) {
				// If the next item is visible, highlight it
				incrementCurrentIndex();
			}
			if (currentItemBottom >= height) {
				// If the next item now is still not visible, scroll
				contentY += 96/3;
			}
		}
	}

	function scrollUp() {
		if (count == 0) {
			return; // No items
		}
		if (selectable) {
			if (currentIndex === -1) {
				// If no item is selected, select the last one.
				currentIndex = count - 1;
				return;
			}
			if (currentIndex >= 0 && currentItemTop > 0) {
				var prevContentY = contentY;
				// If the previous item is visible, highlight it
				decrementCurrentIndex();
				// ListView will "smoothtly scroll the list" even if hightlightFollowsCurrentItem is false,
				// so we have to add the following ugly workaround:
				contentY = prevContentY;
			}
			if (currentItemTop <= 0) {
				// If the previous item now is still not visible, scroll
				var newContentY = contentY - 96/3;

				if (newContentY < 0) {
					contentY = 0; // Never overscroll.
				} else {
					contentY = newContentY;
				}
			}
		}
	}

	Rectangle {
		id: indicatorCont
		visible: list.indicator && (list.contentHeight > list.height)
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

			y: list.visibleArea.yPosition * indicatorCont.height
			height: Math.max(minHeight, list.visibleArea.heightRatio * indicatorCont.height)

			color: "black"
		}
	}
}
