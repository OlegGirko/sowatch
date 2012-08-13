import Qt 4.7
import com.javispedro.sowatch.metawatch 1.0

MWPage {
	MWTitle {
		id: title
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		text: qsTr("Notifications")
		icon.source: "icon.png"
	}

	MWListView {
		id: notifs
		anchors.top: title.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		clip: true
		model: watch.notifications

		delegate: Rectangle {
			id: notifDelegate
			property bool selected: ListView.isCurrentItem
			width: parent.width
			height: childrenRect.height
			color: ListView.isCurrentItem ? "black" : "white"
			Column {
				width: parent.width
				MWLabel {
					width: parent.width
					text: model.modelData.title
					wrapMode: Text.WrapAtWordBoundaryOrAnywhere
					color: notifDelegate.selected ? "white" : "black"
					font.pointSize: 12
				}
				MWLabel {
					width: parent.width
					text: model.modelData.body
					wrapMode: Text.WrapAtWordBoundaryOrAnywhere
					color: notifDelegate.selected ? "white" : "black"
				}
			}
		}
		visible: count > 0;
	}

	MWLabel {
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
