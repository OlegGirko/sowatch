import QtQuick 1.0

ListView {
	id: list

	property bool selectable: true

	interactive: false
	highlightFollowsCurrentItem: false
	boundsBehavior: Flickable.StopAtBounds

	property real currentItemTop: currentItem !== null ? currentItem.y - contentY : 0
	property real currentItemBottom: currentItem !== null ? currentItemTop + currentItem.height : 0

	function scrollDown() {
		if (count == 0) {
			return; // No items
		}
		if (selectable) {
			if (currentIndex == -1) {
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
			if (currentIndex == -1) {
				// If no item is selected, select the last one.
				currentIndex = count - 1;
				return;
			}
			if (currentIndex >= 0 && currentItemTop > 0) {
				// If the previous item is visible, highlight it
				decrementCurrentIndex();
			}
			if (currentItemTop <= 0) {
				// If the previous item now is still not visible, scroll
				contentY -= 96/3;
			}
		}
	}
}
