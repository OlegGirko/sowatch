import QtQuick 1.0

ListView {
	id: list

	property bool selectable: true

	//interactive: false
	highlightFollowsCurrentItem: false
	boundsBehavior: Flickable.StopAtBounds

	property real currentItemTop: currentItem.y - contentY
	property real currentItemBottom: currentItemTop + currentItem.height

	function _scrollSelectable(dir) {
		var maxy = contentHeight - height;
		if (maxy < 0) maxy = 0;

		console.log(contentHeight);

		var newy = contentY;

		if (dir > 0) {
			if (currentIndex < count && currentItemBottom < height) {
				// If the next item is visible, highlight it
				list.incrementCurrentIndex();
			}
			if (currentItemBottom >= height) {
				// If the next item now is still not visible, scroll
				newy += 96/3;
			}
		} else {
			if (currentIndex >= 0 && currentItemTop > 0) {
				// If the previous item is visible, highlight it
				list.decrementCurrentIndex();
			}
			if (currentItemTop <= 0) {
				// If the previous item now is still not visible, scroll
				newy -= 96/3;
			}
		}

		//if (newy < 0) newy = 0;
		//if (newy > maxy) newy = maxy;
		list.contentY = newy;
	}

	function scrollDown() {
		if (selectable) {
			_scrollSelectable(+1);
		}
	}

	function scrollUp() {
		if (selectable) {
			_scrollSelectable(-1);
		}
	}
}
