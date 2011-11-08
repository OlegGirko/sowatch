import QtQuick 1.0

Rectangle {
	width: 96
	height: 96

	color: "white"

	ListView {
		id: notifs
		anchors.fill: parent
		model: watch.notifications
		delegate: Column {
			width: parent.width
			Text {
				width: parent.width
				text: model.modelData.title
				wrapMode: Text.Wrap
			}
			Text {
				width: parent.width
				text: model.modelData.body
				wrapMode: Text.Wrap
			}
			Rectangle {
				width: parent.width
				height: 1
				color: "black"
			}
		}
		visible: count > 0;
	}

	Text {
		anchors.fill: parent
		text: qsTr("No notifications");
		visible: notifs.count == 0;
		wrapMode: Text.Wrap
	}

	Connections {
		target: watch
		onButtonPressed : {
			var increment = (3 * (notifs.width / 4));
			var maxy = notifs.contentHeight - notifs.height;
			var newy;

			if (maxy < 0) maxy = 0;

			switch (button) {
			case 1:
				newy = notifs.contentY - increment;
				if (newy < 0) newy = 0;
				notifs.contentY = newy;
				break;
			case 2:
				newy = notifs.contentY + increment;
				if (newy > maxy) newy = maxy;
				notifs.contentY = newy;
				break;
			}
		}
	}
}
