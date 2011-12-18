import Qt 4.7
import com.javispedro.sowatch.metawatch 1.0

Rectangle {
	width: 96
	height: 96

	color: "white"

	MWTitle {
		id: title
		text: qsTr("Notifications")
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
	}

	MWListView {
		id: notifs
		anchors.top: title.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		model: watch.notifications
		delegate: Rectangle {
			property bool selected: ListView.isCurrentItem
			width: notifs.width
			height: childrenRect.height
			color: ListView.isCurrentItem ? "black" : "white"
			Text {
				width: 96
				text: "<b>" + model.modelData.title + "</b><br>" + model.modelData.body
				wrapMode: Text.WrapAtWordBoundaryOrAnywhere
				color: parent.selected ? "white" : "black"
			}
		}
		visible: count > 0;
	}

	Text {
		anchors.top: title.bottom
		anchors.horizontalCenter: parent.horizontalCenter
		text: qsTr("No notifications");
		visible: notifs.count == 0;
		wrapMode: Text.Wrap
	}

	Connections {
		target: watch
		onButtonPressed : {
			switch (button) {
			case 1:
				notifs.scrollUp();
				break;
			case 2:
				notifs.scrollDown();
				break;
			}
		}
	}
}
