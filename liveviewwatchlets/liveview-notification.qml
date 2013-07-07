import Qt 4.7
import com.javispedro.sowatch.liveview 1.0

LVPage {
	id: page

	property QtObject curNotification: null;

	Column {
		id: container

		anchors.left: parent.left
		anchors.right: parent.right
		anchors.leftMargin: 1
		anchors.rightMargin: 1

		LVLabel {
			text: curNotification ? curNotification.title : ""
			anchors.left: parent.left
			anchors.right: parent.right
			font.pixelSize: 22
			wrapMode: Text.WordWrap
		}

		LVLabel {
			text: curNotification ? curNotification.body : ""
			anchors.left: parent.left
			anchors.right: parent.right
			wrapMode: Text.WordWrap
		}
	}

	function handlesNotification(notification) {
		return false;
	}

	function openNotification(notification) {
		//scrollable.scrollTop();
		curNotification = notification;
	}
}
