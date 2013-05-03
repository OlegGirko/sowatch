import QtQuick 1.0
import QtMobility.organizer 1.1
import com.javispedro.sowatch.metawatch 1.0

MWPage {
	MWTitle {
		id: title
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		text: qsTr("Calendar")
		icon.source: "icon.png"
	}

	MWListView {
		id: list
		anchors.top: title.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		clip: true
		model: OrganizerModel {
			id: model
			manager: "qtorganizer:mkcal"
		}
		delegate: Rectangle {
			id: itemDelegate
			property bool selected: ListView.isCurrentItem
			width: parent.width
			height: childrenRect.height
			color: ListView.isCurrentItem ? "black" : "white"
			Column {
				width: parent.width
				visible: typeof display !== "undefined"
				MWLabel {
					width: parent.width
					text: typeof item !== "undefined" ? _formatEventTime(item) : ""
					wrapMode: Text.WrapAtWordBoundaryOrAnywhere
					color: itemDelegate.selected ? "white" : "black"
					font.family: "MetaWatch Large caps 8pt"
					font.pixelSize: 8
				}
				MWLabel {
					width: parent.width
					text: typeof display !== "undefined" ? display : ""
					wrapMode: Text.WrapAtWordBoundaryOrAnywhere
					color: itemDelegate.selected ? "white" : "black"
					font.pixelSize: 16
				}
			}
		}
	}

	function update() {
		var now = new Date();
		var end = new Date(now.getFullYear(), now.getMonth() , now.getDate() + 7);

		model.startPeriod = now;
		model.endPeriod = end;
		model.update();
	}

	function _isSameDay(date1, date2) {
		return date1.getYear() === date2.getYear() &&
				date1.getMonth() === date2.getMonth() &&
				date1.getDate() === date2.getDate();
	}

	function _daysTo(date1, date2) {
		var ms_per_day = 24 * 3600 * 1000;
		var ts1 = date1.getTime();
		var ts2 = date2.getTime();
		var diff = ts2 - ts1;
		return Math.round(diff / ms_per_day);
	}

	function _formatEventTime(item) {
		var now = new Date();
		var itemStart = item.itemStartTime;
		var itemEnd = item.itemEndTime;
		if (_isSameDay(now, itemStart) && _isSameDay(now, itemEnd)) {
			return Qt.formatTime(itemStart) + " - " + Qt.formatTime(itemEnd);
		} else if (_isSameDay(itemStart, itemEnd)) {
			if (_daysTo(now, itemStart) < 7) {
				return Qt.formatDate(itemStart, "dddd") + "\n" +
						Qt.formatTime(itemStart) + " - " + Qt.formatTime(itemEnd);
			}
			return Qt.formatDate(itemStart) + "\n" +
					Qt.formatTime(itemStart) + " - " + Qt.formatTime(itemEnd);
		} else {
			return Qt.formatDateTime(itemStart) + " -\n" +
					Qt.formatDateTime(itemEnd);
		}
	}

	Connections {
		target: watch
		onButtonPressed: {
			switch (button) {
			case 1:
				list.scrollUp();
				break;
			case 2:
				list.scrollDown();
				break;
			}
		}
		onActiveChanged: {
			if (watch.active) {
				update();
				list.scrollTop();
			}
		}
	}
}
